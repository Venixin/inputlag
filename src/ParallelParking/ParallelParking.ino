#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <NewPing.h>
HUSKYLENS huskylens;

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
int x = 10;
int y = 1;
int t;
bool left = false;
bool right = false;
bool turned = false;
const unsigned int TRIG_PIN = 13;
const unsigned int ECHO_PIN = 12;
int dists[5] = { -1, -2, -3, -4, -5 };
int curr = 0;
long distance;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

float speed = 2000;
int parkingStartTime;



void setup() {
  // Set the maximum speed and acceleration:
  Serial.begin(9600);
  Wire.begin();
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);
  stepper.setSpeed(speed);

  // Setup steering servo
  myservo.attach(9);
  // reset to forward
  myservo.write(100);
  delay(1000);
  t = millis();


  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1000);
  }
  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }


}

void turn_left() {
  left = true;
  myservo.write(75);
  stepper.runSpeed();
}

void turn_right() {
  right = true;
  myservo.write(125);
  stepper.runSpeed();
}


void parallelParkLeft() {
  delay(400);
  stepper.runSpeed();
  parkingStartTime = millis();
  delay(2);
  stepper.setPinsInverted(true, false, false);
  myservo.write(70);
  stepper.runSpeed();
  while (millis() - parkingStartTime < 1200) {
    stepper.runSpeed();
  }
  delay(2);
  stepper.setPinsInverted(false, false, false);
  myservo.write(65);
  delay(500);
  parkingStartTime = millis();
  while (millis() - parkingStartTime < 600) {
    stepper.runSpeed();
  }
  delay(2);
  stepper.setPinsInverted(true, false, false);
  myservo.write(135);
  delay(500);
  parkingStartTime = millis();
  while (millis() - parkingStartTime < 690) {
    stepper.runSpeed();
  }
  myservo.write(100);
  while (true) {
    delayMicroseconds(5);
  }
}



void loop() {
  stepper.runSpeed();
  myservo.write(100);
  if (millis() - t > 2500) {
    parallelParkLeft();
  }
}
