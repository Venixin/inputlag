#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
HUSKYLENS huskylens;

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
   Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
    }
}
void display(int id){
  int count = huskylens.count(id);
    for (int i=0; i<count;i++){
      HUSKYLENSResult result = huskylens.getBlock(id, i);
      printResult(result);
    }
}
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

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
float speed = 2000;


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

void turn_left(){
  left = true;
  myservo.write(75);
  stepper.runSpeed();
}

void turn_right(){
  right = true;
  myservo.write(125);
  stepper.runSpeed();
}
//myservo.write(100) is straight
void loop() { 
  if (huskylens.request()) {
    if (huskylens.count(1) > 0 && left == false){
        Serial.println("RED");
        turn_left();
        t = millis();
      }
      if (huskylens.count(2) > 0 && right == false){
        Serial.println("BLUE");
        turn_right();
        t = millis();
      }
  }


  if (millis() - t >= 1800 && turned) {
    myservo.write(100);
    stepper.runSpeed();
    turned = false;
    left = false;
    right = false;
    Serial.println("1");
    t = millis();
  }

  if (millis() - t >= 900 && millis()-t < 1800) {
    if (left){
      myservo.write(125);
      stepper.runSpeed();
      Serial.println("2");
      turned = true;
    }
    else if (right){
      myservo.write(75);
      stepper.runSpeed();
      Serial.println("3");
      turned = true;
    }
  }
  if (millis()-t > 200){
    Serial.println("RED");
    display(4);
    Serial.println("GREEN");
    display(3);
    Serial.println();
    t = millis();
  }
  stepper.runSpeed();
  
}