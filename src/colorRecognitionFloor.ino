#include <HUSKYLENS.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include "AccelStepper.h"

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
Servo myservo;  // create servo object to control a servo
HUSKYLENS huskylens;

SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11

#define BLUE_ID 1
#define ORANGE_ID 2
//orange first = clockwise
//blue first = anticlockwise
int blueLines = 0;
int orangeLines = 0;
bool turnRight = true;
bool directionChecked = false;



void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(1000);
    stepper.setSpeed(500);
    myservo.attach(9);

    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
        Serial.print("Failed to set algorithm");
        while (true);
    }
}



void checkDir() {
    while (!huskylens.request()) {
    stepper.runSpeed();
    }
    if (huskylens.count(1) > 0) {
        turnRight = false;
        Serial.print("Turning left");
        directionChecked = true;

    }
    else {
        Serial.print("Turning right");
        directionChecked = false;
    }
}



void loop() {
    if (directionChecked = false) {
        checkDir();
    }
    stepper.runSpeed();
} 