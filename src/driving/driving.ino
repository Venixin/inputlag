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
#define redID 4
#define greenID 3
#define lineID 5
int x = 10;
int y = 1;
int t;
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
bool left90 = false;
bool right90 = false;
bool middle = false;
int n=0;
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
  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }
  delay(2000);
  Serial.println("PLEASE");
}

void turn_left() {

  Serial.println("l");
  left = true;
  myservo.write(75);
  stepper.runSpeed();
}

void turn_right() {
  Serial.println("r");
  right = true;
  myservo.write(125);
  stepper.runSpeed();
}

void right_90() {
  Serial.println("aaa");
  myservo.write(150);
  stepper.runSpeed();
}

void left_90() {
  Serial.println("bbb");
  myservo.write(50);
  stepper.runSpeed();
}
// myservo.write(100) is straight
void loop() {
  stepper.runSpeed();
  n+=1;


  if (recheck && (n % 2000 == 0 && n != 0)) {
    Serial.println(n);
    if (huskylens.requestBlocks()) {
      redCount = huskylens.count(redID);
      greenCount = huskylens.count(greenID);
      if (redCount > 1) {
        turn_right();
        recheck = false;
        t = millis();
      } else if (greenCount > 1) {
        turn_left();
        recheck = false;
        t = millis();
      } else if (redCount == 1 && greenCount == 1) {
        //get results and check which is closer
        HUSKYLENSResult redResult = huskylens.getBlock(redID, 0);
        HUSKYLENSResult greenResult = huskylens.getBlock(greenID, 0);
        int redArea = redResult.width * redResult.height;
        int greenArea = greenResult.width * greenResult.height;
        if (redResult.yCenter < greenResult.yCenter && redArea < greenArea) {
          turn_left();
          left90 = true;
          recheck = false;
          t = millis();
        } else if (redResult.yCenter > greenResult.yCenter && redArea < greenArea) {
          turn_right();
          right90 = true;
          recheck = false;
          t = millis();
        } else {
          recheck = true;
        }
      } else {
        recheck = true;
      }
    }
  }
  if (millis() - t >= 5000 && !middle) {
    Serial.println(left90);
    if (left) {
      myservo.write(100);
    } else if (right) {
      myservo.write(100);
    } else if (left90) {
      myservo.write(50);
    } else if (right90) {
      myservo.write(150);
    }
    middle = true;
  }
  if (millis() - t >= 10000 && !turned) {
    Serial.println("b");
    if (left || right) {
      myservo.write(75);
    }
  }
  if (millis() - t >= 15000) {
    Serial.println("c");
    if (left90) {
      myservo.write(150);
    } else if (right90) {
      myservo.write(50);
    }
    else{
      myservo.write(100);
    }
    turned = true;
  }
  if (millis() - t >= 20000 && turned) {
    Serial.println("d");
    if (left90) {
      myservo.write(125);
    } else if (right90) {
      myservo.write(75);
    }
    turned = false;
  }

  if (millis() - t >= 25000) {
    Serial.println("e");
    left = false;
    right = false;
    left90 = false;
    right90 = false;
    turned = false;
    // go forward then turn left or right based on the lines
  }

  stepper.runSpeed();
}