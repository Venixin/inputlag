#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <MobaTools.h>
HUSKYLENS huskylens;

// Define stepper motor connections and motor interface type.
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define yellowID 1
#define blueID 2
#define redID 3
#define greenID 4
#define lineID 5
#define startBtn 7

unsigned long t;
bool left = false;
const unsigned int TRIG_PIN = 13;
const unsigned int ECHO_PIN = 12;
int redCount = 0;
int greenCount = 0;
int yellowCount = 0;
int blueCount = 0;
bool leftcheck = true;+
bool seen = false;
int turn = 0;
bool count = true;
// Create a new instance of the AccelStepper class:
// AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
const int stepsPerRev = 4000;
Servo myservo;                               // create servo object to control a servo
MoToStepper stepper1(stepsPerRev, STEPDIR);  // create a stepper instance

void setup() {


  // Set the maximum speed and acceleration:
  Wire.begin();
  Serial.begin(9600);
  // Setup steering servo
  myservo.attach(9);
  // reset to forward
  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1000);
  }

  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }
  Serial.println("huskylens done");
  pinMode(startBtn, INPUT_PULLUP);
  while (digitalRead(startBtn) == 1) {
    myservo.write(100);
    // Serial.println(digitalRead(startBtn));
  }
  Serial.println("start");
  stepper1.attach(stepPin, dirPin);
  stepper1.setSpeed(2000);   // 30 rev/min (if stepsPerRev is set correctly)
  stepper1.setRampLen(200);  // Ramp length is 1/2 revolution
  stepper1.rotate(1);
}

void parallelParkLeft() {
  stepper.rotate(-1);
  myservo.write(50);
  delay(1650);
  stepper.rotate(0);
  delay(100);
  myservo.write(150);
  stepper.rotate(-1);
  delay(1650);
  stepper.rotate(0);
  delay(100);
  myservo.write(100);
  stepper.rotate(1);
  delay(500);
  while (true) {
    stepper.rotate(0);
  }
}

void parallelParkRight() {
  stepper.rotate(-1);
  myservo.write(150);
  delay(1650);
  stepper.rotate(0);
  delay(100);
  myservo.write(50);
  stepper.rotate(-1);
  delay(1650);
  stepper.rotate(0);
  delay(100);
  myservo.write(100);
  stepper.rotate(1);
  delay(500);
  while (true) {
    stepper.rotate(0);
  }
}

// myservo.write(100) is straight
// 150cm in 3500ms
void loop() {

  if (huskylens.request()) {
    if (leftcheck) {
      yellowCount = huskylens.count(yellowID);
      blueCount = huskylens.count(blueID);
      if (yellowCount >= 1 && blueCount >= 1) {
        HUSKYLENSResult yellowResult = huskylens.getBlock(yellowID, 0);
        HUSKYLENSResult blueResult = huskylens.getBlock(blueID, 0);
        if (yellowResult.yCenter < blueResult.yCenter) {
          left = false;
        } else {
          left = true;
        }
        leftcheck = false;
      }
    }
    redCount = huskylens.count(redID);
    greenCount = huskylens.count(greenID);

    if (greenCount >= 1 && redCount >= 1) {

      HUSKYLENSResult redResult = huskylens.getBlock(redID, 0);
      HUSKYLENSResult greenResult = huskylens.getBlock(greenID, 0);
      count = true;
      if (greenResult.yCenter < redResult.yCenter) {
        myservo.write(125);
      } else {
        myservo.write(75);
      }
      seen = true;
    } else if (greenCount > 0) {
      myservo.write(75);
      seen = true;
count = true;
    } else if (redCount > 0) {
      myservo.write(125);
      seen = true;
count = true;
    } else if (redCount == 0 && greenCount == 0) {
      yellowCount = huskylens.count(yellowID);
      blueCount = huskylens.count(blueID);
      if (yellowCount > 0 && blueCount > 0) {
        if (count){
          turn += 1;
          count = false;
        }
        if (left) {
          myservo.write(60);
        } else {
          myservo.write(140);
        }
        seen = false;

      } else if (myservo.read()==75 && seen) {
        myservo.write(100);
        delay(200);
        myservo.write(125);
        delay(500);
        myservo.write(100);
        delay(200);
        myservo.write(125);
        seen = false;
        count = true;
      } else if (myservo.read()==125 && seen) {
        myservo.write(100);
        delay(200);
        myservo.write(75);
        delay(500);
        myservo.write(100);
        delay(200);
        myservo.write(75);
        seen = false;
        count = true;
      } else {
        if (!seen) { myservo.write(100); }
      }
    }

    if (seen == 13) {
      stepper.rotate(-1);
    delay(3250);
    stepper.rotate(0);
    delay(100);
    if (!left){
      parallelParkLeft();
    }
    else{
      parallelParkRight();
    }
    }
  }
}