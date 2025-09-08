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
bool leftcheck = true;
bool red = false;
bool green = false;
bool turn = false;
int seen = 0;
bool count = true;
bool timer = false;
int t1 = 0;
bool twice = false;
bool seenred = false;
bool seengreen = false;
// Create a new instance of the AccelStepper class:
// AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
const int stepsPerRev = 4000;
Servo myservo;  // create servo object to control a servo
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
      if (greenResult.yCenter < redResult.yCenter) {
        myservo.write(120);
        red = true;
      } else {
        myservo.write(75);
        green = true;
      }
    } else if (greenCount > 0 || seengreen) {
      if (turn) {
        if (timer) {
          if (green) {
            myservo.write(75);
            twice = true;
          } else if (red) {
            myservo.write(100);
          }
          t = millis();
          count = false;
          timer = false;
          seengreen = true;
        }
        if (millis() - t >= 450) {
          if (green && twice) {
            myservo.write(100);
          } else if (red) {
            myservo.write(75);
          }
          red = false;
          green = true;
          count = true;
          seengreen = false;
        }
      } else {
        myservo.write(75);
        green = true;
      }

    } else if (redCount > 0 || seenred) {
      if (turn) {
        if (timer) {
          if (green) {
            myservo.write(100);
          } else if (red) {
            myservo.write(120);
            twice = true;
          }
          t = millis();
          count = false;
          timer = false;
          seenred = true;
        }
        if (millis() - t >= 450) {
          if (green) {
            myservo.write(75);

          } else if (red && twice) {
            myservo.write(100);
            twice = false;
          }
          red = true;
          count = true;
          green = false;
          seenred = false;
        }
      } else {

        myservo.write(120);
        red = true;
      }

    } else if (redCount == 0 && greenCount == 0) {
      yellowCount = huskylens.count(yellowID);
      blueCount = huskylens.count(blueID);
      if (yellowCount > 0 && blueCount > 0 && !(seenred || seengreen)) {
        if (seen % 2 == 1) {
          seen += 1;
        }
        if (left) {
          myservo.write(50);
        } else {
          myservo.write(150);
        }

        turn = false;
        timer = true;
      } else if (green && !timer) {
        if (count) {
          count = false;
          myservo.write(100);
          t1 = millis();
        }
        if (millis() - t1 >= 350) {
          myservo.write(120);
          turn = true;
          timer = true;
          seen += 1;
        }
      } else if (red && !timer) {
        if (count) {
          count = false;
          myservo.write(100);
          t1 = millis();
        }
        if (millis() - t1 >= 350) {
          myservo.write(75);
          turn = true;
          timer = true;
          seen += 1;
        }
      } else {
        if (!turn) { myservo.write(100); }
      }
    }

    if (seen == 26) {
      Serial.println("end");
      while (true)
        ;
    }
  }
}