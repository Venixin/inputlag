#include "AccelStepper.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <HUSKYLENS.h>
HUSKYLENS huskylens;

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
int x = 10;
int y = 1;
unsigned long t;
bool left = true;
bool check = true;
int yellowCount = 0;
int blueCount = 0;
int blueID = 2;
int yellowID = 1;
bool turning = false;
int startBtn = 7;
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
float speed = 2000;

unsigned long n = 0;
void setup() {
  // Set the maximum speed and acceleration:
  Serial.begin(115200);
  Wire.begin();

  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);
  stepper.setSpeed(speed);

  // Setup steering servo
  myservo.attach(9);  
  // reset to forward
  myservo.write(100);
  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1000);
  }
  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }
  // delay(2000);
  t = millis();
  pinMode(startBtn, INPUT_PULLUP);
  while(digitalRead(startBtn) == LOW){}
  Serial.println("start");
}


//myservo.write(100) is straight
// 150cm in 3500ms
void loop() { 
  
  if (check && n%5000 == 0){
    n = 0;
    if (huskylens.requestBlocks()){
      Serial.println("checking");
      int yellowCount = huskylens.count(yellowID);
      int blueCount = huskylens.count(blueID);
      if (yellowCount > 0 && blueCount > 0){
        HUSKYLENSResult yellowResult = huskylens.getBlock(yellowID, 0);
        HUSKYLENSResult blueResult = huskylens.getBlock(blueID, 0);
        
        left = yellowResult.yCenter > blueResult.yCenter;
        Serial.println(left);
        check = false;
      }
    }
  }
  n+= 1;
  if (millis() - t >= 3500 && !turning) { //Turns right every 2 seconds for 4 times
    if (left){
      myservo.write(55);
    }
    else{
      myservo.write(145);
    }
    stepper.runSpeed();
    t = millis();
    turning = true;
  }

  if (millis() - t >= 1050 && turning) { //Turns wheel straight again after 1000ms after turning
    myservo.write(100);
    stepper.runSpeed();
    turning = false;
  }
  // myservo.write(70);
  stepper.runSpeed();
  
}