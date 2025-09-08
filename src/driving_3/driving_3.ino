#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <MobaTools.h>
HUSKYLENS huskylens;

#define dirPin 2
#define stepPin 3
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
bool leftcheck = true;
int turn = 0;
const int stepsPerRev = 4000;
Servo myservo;                              // create servo object to control a servo
MoToStepper stepper(stepsPerRev, STEPDIR);  // create a stepper instance

void setup() {
  Wire.begin();
  Serial.begin(9600);
  // Setup steering servo
  myservo.attach(9);
  // reset to forward
  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1250);
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
  stepper.attach(stepPin, dirPin);
  stepper.setSpeed(2000);  // 30 rev/min (if stepsPerRev is set correctly)

  stepper.rotate(1);
}

void turn_right() {
  myservo.write(135);
  delay(1000);
  myservo.write(100);
  delay(450);
  myservo.write(65);
  delay(1450);
  myservo.write(135);
  delay(900);
  myservo.write(100);
}

void turn_left() {
  myservo.write(65);
  delay(1000);
  myservo.write(100);
  delay(450);
  myservo.write(135);
  delay(1450);
  myservo.write(65);
  delay(900);
  myservo.write(100);
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

// myservo.write(100) is straight
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
      if (greenResult.yCenter < redResult.yCenter) {
        Serial.println("red");
        turn_right();
      } else {
        turn_left();
      }
    } else if (greenCount > 0) {
      turn_left();


    } else if (redCount > 0) {
      turn_right();
    }
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
    }
    if (yellowCount == 0 && redCount == 0 && greenCount == 0 && blueCount > 0) {
      if (left && turn < 12) {
        myservo.write(65);
        delay(2000);
        myservo.write(100);
        turn += 1;
      }
    } else if (yellowCount > 0 && redCount == 0 && greenCount == 0 && blueCount == 0) {
      if (!left && turn < 12) {
        myservo.write(135);
        delay(2000);
        myservo.write(100);
        turn += 1;
      }
    }
  }
  if (turn == 13) {
    stepper.rotate(-1);
    delay(3250);
    stepper.rotate(0);
    delay(100);
    parallelParkLeft();
  }
}