#include "AccelStepper.h"
#include <SoftwareSerial.h>

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


void setup() {
  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(1000);
  Serial.begin(115200);
  myservo.attach(9);  
  stepper.setSpeed(800);
}


void loop() {
  stepper.runSpeed();
  Serial.println("Running");
}