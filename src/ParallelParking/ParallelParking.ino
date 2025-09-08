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
int turn = 0;
const int stepsPerRev = 4000;
Servo myservo;                               // create servo object to control a servo
MoToStepper stepper(stepsPerRev, STEPDIR);

void setup() {
  // Set the maximum speed and acceleration:
  Serial.begin(9600);
  Wire.begin();


  // Setup steering servo
  myservo.attach(9);
  // reset to forward
  myservo.write(100);
  pinMode(startBtn, INPUT_PULLUP);
  while (digitalRead(startBtn) == 1) {
    myservo.write(100);
    // Serial.println(digitalRead(startBtn));
  }
  Serial.println("start");
  stepper.attach(stepPin, dirPin);
  stepper.setSpeed(2000);   // 30 rev/min (if stepsPerRev is set correctly)

  stepper.rotate(1);
  parallelParkLeft();
}

void parallelParkLeft() {
  stepper.rotate(-1);
  myservo.write(50);
  delay(1650);
  stepper.rotate(0);
  delay(100);
  myservo.write(150);
  stepper.rotate(-1);
  delay(1650);
  stepper.rotate(0);
  delay(100);
  myservo.write(100);
  stepper.rotate(1);
  delay(500);
  while (true) {
    stepper.rotate(0);
  }
}



void loop() {

}
