#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
HUSKYLENS huskylens;

// Define stepper motor connections and motor interface type.
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define redID 3
#define greenID 4
#define lineID 5
int x = 10;
int y = 1;
unsigned long t;
bool left = false;
bool right = false;
bool turned = false;
const unsigned int TRIG_PIN = 13;
const unsigned int ECHO_PIN = 12;
int curr = 0;
int redCount = 0;
int greenCount = 0;
int lineCount = 0;
int a = 0;
bool recheck = true;
bool middle = false;
int n = 0;
int startBtn = 7;
int pattern = 0;
int pos = 0;
int target = -1;
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

Servo myservo;  // create servo object to control a servo
float speed = 4000;


void setup() {
  // Set the maximum speed and acceleration:
  // Serial.begin(115200);
  Wire.begin();
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);
  stepper.setSpeed(speed);

  // Setup steering servo
  myservo.attach(9);
  // reset to forward
  myservo.write(100);
  t = millis();

  Serial.begin(9600);
  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1000);
  }
  Serial.println("done");
  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }
  pinMode(startBtn, INPUT_PULLUP);
  while (digitalRead(startBtn) == LOW) { myservo.write(100); }
  Serial.println("start");
}

// myservo.write(100) is straight
// 150cm in 3500ms
void loop() {
  stepper.runSpeed();
  n += 1;

  if (recheck && (n % 2000 == 0 && n != 0)) {
    n = 0;
    if (huskylens.requestBlocks()) {

      redCount = huskylens.count(redID);
      greenCount = huskylens.count(greenID);
      Serial.print(redCount);
      Serial.print(" green: ");
      Serial.println(greenCount);
      if (redCount > 1) {
        pattern = 3;
        recheck = false;
        Serial.print("Pattern: ");
        Serial.println(pattern);
        t = millis();
      } else if (greenCount > 1) {
        pattern = 4;
        recheck = false;
        Serial.print("Pattern: ");
        Serial.println(pattern);
        t = millis();
      } else if (redCount == 1 && greenCount == 1) {
        //get results and check which is closer
        HUSKYLENSResult redResult = huskylens.getBlock(redID, 0);
        HUSKYLENSResult greenResult = huskylens.getBlock(greenID, 0);
        // green > red : 1
        // red > green : 2
        // red > red : 3
        // green > green : 4
        // red : 5
        // green : 6
        int redArea = redResult.width * redResult.height;
        int greenArea = greenResult.width * greenResult.height;
        if (redResult.yCenter < greenResult.yCenter && redArea < greenArea) {
          pattern = 1;
          recheck = false;
          Serial.print("Pattern: ");
          Serial.println(pattern);
          t = millis();
        } else if (redResult.yCenter > greenResult.yCenter && redArea > greenArea) {
          pattern = 2;
          recheck = false;
          Serial.print("Pattern: ");
          Serial.println(pattern);
          t = millis();
        } else if (redCount == 1 && greenCount == 0) {
          pattern = 6;
          recheck = false;
          Serial.print("Pattern: ");
          Serial.println(pattern);
        } else if (redCount == 0 && greenCount == 1) {
          pattern = 5;
          recheck = false;
          Serial.print("Pattern: ");
          Serial.println(pattern);
        } else {
          recheck = true;
        }
      } else {
        recheck = true;
      }
    }
  }

  if (pos == 0 && recheck == false) {
    Serial.print(t);
    Serial.print(" ");
    Serial.println(pos);
    if (pattern == 1 || pattern == 4 || pattern == 6) {
      myservo.write(55);
    }
    if (pattern == 2 || pattern == 3 || pattern == 5) {
      myservo.write(145);
    }
    pos += 1;
    t = millis();
    target = 525;
    Serial.println(myservo.read());
  }
  if (millis() - t >= target && pos < 6) {
    Serial.print(pattern);
    Serial.print(" ");
    Serial.println(pos);
    if (pos == 1) {

      myservo.write(100);
      pos += 1;
      target += 1000;
    } else if (pos == 2) {
      if (pattern == 1 || pattern == 4 || pattern == 6) {
      myservo.write(145);
    }
    if (pattern == 2 || pattern == 3 || pattern == 5) {
      myservo.write(55);
    }
      pos += 1;
      target += 850;
    } else if (pos == 3 && (pattern == 3 || pattern == 4 || pattern == 5 || pattern == 6)) {

      myservo.write(100);
      target += 525;
    } else if (pos == 3 && (pattern == 1 || pattern == 2)) {

      myservo.write(100);
      pos += 1;
      target += 1000;
    } else if (pos == 4) {

      if (pattern == 1){
          myservo.write(55);
      } else if (pattern == 2){
          myservo.write(145);
      }
      pos += 1;
      target += 425;
    } else if (pos == 5) {

      myservo.write(100);
      pos += 1;
    }
    Serial.println(myservo.read());
  }
  // if (pos == 6) {
  //   Serial.println(6);
  //   pos += 1;
  //   if (left) {
  //     myservo.write(55);
  //   } else {
  //     myservo.write(145);
  //   }
  //   t = millis();
  //   // go forward then turn left or right based on the lines
  // }
  // if (pos == 7 && millis()-t >= 1050){
  //   myservo.write(100);
  //   pos = 0;
  // }

  stepper.runSpeed();
}