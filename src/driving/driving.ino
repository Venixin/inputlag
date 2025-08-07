#include "AccelStepper.h"
#include <SoftwareSerial.h>

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
int x = 10;
int y = 1;
int t;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
float speed = 2000;


void setup() {
  // Set the maximum speed and acceleration:
  Serial.begin(115200);
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);
  stepper.setSpeed(speed);

  // Setup steering servo
  myservo.attach(9);  
  // reset to forward
  myservo.write(100);
  // delay(2000);
  t = millis();
}


//myservo.write(100) is straight
void loop() { 
  if (millis() - t >= 1500 && y <= 4) { //Turns right every 2 seconds for 4 times
    Serial.println(t);
    myservo.write(50); //Going clockwise around the square (Turning right)
    stepper.runSpeed();
    y = y + 1;
    t = millis();
  }

  if (millis() - t >= 900 && y <= 5) { //Turns wheel straight again after 1000ms after turning
    myservo.write(100);
    stepper.runSpeed();
  }

  stepper.runSpeed();
  
}