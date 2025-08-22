#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
HUSKYLENS huskylens;

void printResult(HUSKYLENSResult result) {
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  }
}
void display(int id) {
  int count = huskylens.count(id);
  for (int i = 0; i < count; i++) {
    HUSKYLENSResult result = huskylens.getBlock(id, i);
    printResult(result);
  }
}
// Define stepper motor connections and motor interface type.
// Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define redID 4
#define blueID 3
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
int redCount = 0;
int blueCount = 0;
int a = 0;
int blueCurr = 0;
int redCurr = 0;
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
  huskylens.setTimeOutDuration(1);
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


// myservo.write(100) is straight
void loop() {
  stepper.runSpeed();
  
  if (huskylens.requestBlocks()) {
    
    // double angle = tan(y/x);
    // double adjusted_x = y / atan(angle);
    // if (120 <= adjusted_x <= 200) {
    //   move();
    // }
    
    redCurr = huskylens.count(redID);
    blueCurr = huskylens.count(blueID);
    if (left == false) {
      if (redCurr > redCount) {
        turn_left();
        t = millis();
      }
      redCount = redCurr;
    }
    if (right == false) {
      if (blueCurr > blueCount) {
        turn_right();
        t = millis();
      }
      blueCount = blueCurr;
    }
  }
  else{
    stepper.runSpeed();
  }

  if (millis() - t >= 5000 && (left || right)) {
    myservo.write(100);
    stepper.runSpeed();
    turned = true;
  }
  if (turned){
    if (left){
      myservo.write(125);
    }
    else if (right){
      myservo.write(75);
    }
    if (millis() - t >= 10000){
      myservo.write(100);
      turned = false;
      left = false;
      right = false;
    }
  }
  stepper.runSpeed();
}