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
#define yellowID 3
#define blueID 4
#define redID 2
#define greenID 1
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

bool recheck = true;
int n = 0;
int pattern = 0;
int pos = 0;
int target = -1;
bool leftcheck = false;
bool initial = false;
int cycle = 0;
int state = 0;
bool red = false;
bool green = false;
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

Servo myservo;  // create servo object to control a servo
float speed = 2000;

void setup() {
  // Set the maximum speed and acceleration:
  // Serial.begin(115200);
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
  // while (!leftcheck) {
  //   stepper.runSpeed();
  //   if (huskylens.requestBlocks()) {
  //     yellowCount = huskylens.count(yellowID);
  //     blueCount = huskylens.count(blueID);
  //     redCount = huskylens.count(redID);
  //     greenCount = huskylens.count(greenID);
  //     if (yellowCount == 1 && blueCount == 1 && (greenCount > 0 || redCount > 0)) {
  //       Serial.println("found");
  //       HUSKYLENSResult yellowResult = huskylens.getBlock(yellowID, 0);
  //       HUSKYLENSResult blueResult = huskylens.getBlock(blueID, 0);
  //       if (yellowResult.yCenter < blueResult.yCenter) {
  //         left = false;
  //       } else {
  //         left = true;
  //       }
  //       leftcheck = true;
  //       initial = greenCount > 0;
  //       state = 0;
  //       t = millis();
  //     }
  //   }
  // }

  // if (leftcheck && cycle == -1) {
  //   if (state == 0) {
  //     if (initial) {
  //       myservo.write(55);
  //     } else {
  //       myservo.write(145);
  //     }
  //     state += 1;
  //   }
  //   if (millis() - t >= 1050 && state == 1) {
  //     state += 1;
  //     initial = !initial;
  //   }
  //   if (millis() - t >= 2100 && state == 2) {
  //     state += 1;
  //     initial = !initial;
  //   }
  //   if (millis() - t >= 3150 && state == 3) {
  //     myservo.write(100);
  //     cycle = 0;
  //   }
  // }


  stepper.runSpeed();
  if (cycle >= 0) {
    n += 1;

    if (n >= 10000 == 0 && n != 0) {
      n = 0;
      if (huskylens.requestBlocks()) {
        redCount = huskylens.count(redID);
        greenCount = huskylens.count(greenID);
        if (greenCount > 0) {
          if (red == false) {
            Serial.println("green");
            myservo.write(65);
            green = true;
          }
        }
        else{
          green = false;
        }

        if (redCount > 0) {
          if (green == false) {
            Serial.println("red");
            myservo.write(135);
            red = true;
          }
        }
        else{
          red = false;
        }
      }
    }


    // if (pos == 0 && recheck == false) {
    //   if (pattern == 1 || pattern == 4 || pattern == 6) {
    //     myservo.write(55);
    //   }
    //   if (pattern == 2 || pattern == 3 || pattern == 5) {
    //     myservo.write(145);
    //   }
    //   pos += 1;
    //   t = millis();
    //   target = 525;
    //   Serial.println(myservo.read());
    // }
    // if (millis() - t >= target && pos < 6) {
    //   if (pos == 1) {
    //     myservo.write(100);
    //     pos += 1;
    //     target += 1000;
    //   } else if (pos == 2) {
    //     if (pattern == 1 || pattern == 4 || pattern == 6) {
    //       myservo.write(145);
    //     }
    //     if (pattern == 2 || pattern == 3 || pattern == 5) {
    //       myservo.write(55);
    //     }
    //     pos += 1;
    //     target += 850;
    //   } else if (pos == 3 && (pattern == 3 || pattern == 4 || pattern == 5 || pattern == 6)) {
    //     myservo.write(100);
    //     target += 525;
    //   } else if (pos == 3 && (pattern == 1 || pattern == 2)) {
    //     myservo.write(100);
    //     pos += 1;
    //     target += 1000;
    //   } else if (pos == 4) {
    //     if (pattern == 1) {
    //       myservo.write(55);
    //     } else if (pattern == 2) {
    //       myservo.write(145);
    //     }
    //     pos += 1;
    //     target += 425;
    //   } else if (pos == 5) {
    //     myservo.write(100);
    //     pos += 1;
    //   }
    //   Serial.println(myservo.read());
    // }
    // // if (pos == 6) {
    // //   Serial.println(6);
    // //   pos += 1;
    // //   if (left) {
    // //     myservo.write(55);
    // //   } else {
    // //     myservo.write(145);
    // //   }
    // //   t = millis();
    // //   // go forward then turn left or right based on the lines
    // // }
    // // if (pos == 7 && millis()-t >= 1050){
    // //   myservo.write(100);
    // //   pos = 0;
    // // cycle += 1;
    // // }
    // if (cycle == 12) {
    //   // reverse
    //   return;
    // }

    stepper.runSpeed();
  }
}
