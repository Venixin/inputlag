#include "AccelStepper.h"
#include <SoftwareSerial.h>

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
int x = 10;
int y = 1;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

#include <Servo.h>

Servo myservo;  // create servo object to control a servo



void setup() {
  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(2500);
  stepper.setAcceleration(1000);
  Serial.begin(115200);
  stepper.setSpeed(1500);
  myservo.attach(9);  
  
  myservo.write(70);
}


//myservo.write(70) is straight 
//myservo.write(40) turns front wheels 30 degrees left
//myservo.write(100) turns front wheels 30 degrees right etc.
void loop() { 
  if (millis() > (4000*y) and y <= 4) { //Turns right every 2 seconds for 4 times
    myservo.write(100); //Going clockwise around the square (Turning right)
    stepper.runSpeed();
    y = y + 1;
  }

  if (millis() > (4000*(y-1) + 3000)) { //Turns wheel straight again after 900ms after turning
    myservo.write(70);
    stepper.runSpeed();
  }

  stepper.runSpeed();
  
}