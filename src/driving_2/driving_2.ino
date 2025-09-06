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
#define yellowID 1
#define blueID 2
#define redID 3
#define greenID 4
#define lineID 5
#define startBtn 7
int x = 10;
int y = 1;
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
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

Servo myservo;  // create servo object to control a servo
float speed = 2000;


void setup() {
  // Set the maximum speed and acceleration:
  // Serial.begin(120200);
  Wire.begin();
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);
  stepper.setSpeed(speed);
  Serial.begin(9600);
  // Setup steering servo
  myservo.attach(9);
  // reset to forward
  myservo.write(100);
  t = millis();


  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1000);
  }
  Serial.println("huskylens done");
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

  if (millis() % 500 == 0) {
    if (huskylens.request()) {
      if (leftcheck) {
        yellowCount = huskylens.count(yellowID);
        blueCount = huskylens.count(blueID);
        if (yellowCount >= 1 && blueCount >= 1) {
          HUSKYLENSResult yellowResult = huskylens.getBlock(yellowID, 0);
          HUSKYLENSResult blueResult = huskylens.getBlock(blueID, 0);
          if (yellowResult.yCenter < blueResult.yCenter) {
            left = false;
            Serial.println("right");
            leftcheck = false;
          } else {
            left = true;
            Serial.println("left");
            leftcheck = false;
          }
        }
      }
      redCount = huskylens.count(redID);
      greenCount = huskylens.count(greenID);
      
      Serial.println(seen);

      if (greenCount >= 1 && redCount >= 1) {

        HUSKYLENSResult redResult = huskylens.getBlock(redID, 0);
        HUSKYLENSResult greenResult = huskylens.getBlock(greenID, 0);
        if (greenResult.yCenter < redResult.yCenter) {
          myservo.write(115);
          red = true;
          green = false;
        } else {
          myservo.write(85);
          red = false;
          green = true;
        }
      } else if (greenCount > 0) {
        if (turn) {
          if (red || green) {
            if (green) {
              myservo.write(80);
            } else if (red) {
              myservo.write(100);
            }
            t = millis();
            count = true;
          }
          if (millis - t >= 600) {
            if (red) {
              myservo.write(80);
            } else if (green) {
              myservo.write(100);
            }
            turn = false;
          }
        } else {
          myservo.write(80);
        }
        red = false;
        green = true;
      } else if (redCount > 0) {
        if (turn) {
          if (red || green) {
            if (green) {
              myservo.write(100);
            } else if (red) {
              myservo.write(80);
            }
            t = millis();
            count = true;
          }
          if (millis - t >= 600) {
            if (red) {
              myservo.write(100);
            } else if (green) {
              myservo.write(80);
            }
            turn = false;
          }
        } else {
          myservo.write(120);
        }
        red = true;
        green = false;
      } else if (redCount == 0 && greenCount == 0) {
        if (green) {
          myservo.write(120);
          if (count) {
            seen += 1;
            count = false;
          }
          turn = true;
        } else if (red) {
          myservo.write(80);
          if (count) {
            seen += 1;
            count = false;
          }
          turn = true;
        } else {
          myservo.write(100);
        }
      }
      if (seen % 2 == 0) {
        if (redCount == 0 && greenCount == 0) {
          if (left) {
            myservo.write(80);
            Serial.println("turning left");
          } else {
            myservo.write(120);
            Serial.println("turning right");
          }
        }
      }
      if (seen == 26){
        Serial.println("end");
        while(true);
      }
    }
  }
}
