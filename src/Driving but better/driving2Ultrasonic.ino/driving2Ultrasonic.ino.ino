#include <Servo.h>
#include <Wire.h>
#include <MobaTools.h>
#include <NewPing.h>
#include <HUSKYLENS.h>

#define dirPin 2
#define stepPin 3
#define startBtn 7
#define yellowID 1
#define blueID 2
const unsigned int TRIG_PIN = 13;
const unsigned int ECHO_PIN = 12;
int dists[5];

float duration, distance;
int curr = 0;
unsigned int pingSpeed = 29;
unsigned long pingTimer;
unsigned long t;
bool left = true;
bool leftcheck = true;
bool turned = false;
const int stepsPerRev = 200;
int turn = 0;
int yellowCount = 0;
int blueCount = 0;

NewPing sonar(TRIG_PIN, ECHO_PIN);
MoToStepper stepper(stepsPerRev, STEPDIR);
HUSKYLENS huskylens;
Servo myservo;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1250);
  }

  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }
  Serial.println("huskylens done");
  pingTimer = millis();
  myservo.attach(9);
  stepper.attach(stepPin, dirPin);
  stepper.setSpeed(2000);
  stepper.setRampLen(stepsPerRev / 4);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(startBtn, INPUT_PULLUP);
  while (digitalRead(startBtn) == HIGH) { myservo.write(100); }

  myservo.write(100);
  stepper.rotate(1);
  Serial.println("start");
}

bool distcheck(int arr[]) {
  if (arr[4] < 0) {
    return false;
  }
  int high = 0;
  int low = 0;
  for (int i = 0; i < 4; i++) {
    if ((arr[i + 1] - arr[i]) > 0) {
      high += 1;
    } else if (arr[i] < 50) {
      low += 1;
    } else {
      high += 1;
    }
  }
  if (low >= high) {
    return true;
  } else {
    return false;
  }
}

bool check = true;
long echoCheck() {  // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) {  // This is how you check to see if the ping was received.
    // Here's where you can add code.
    distance = sonar.ping_result / US_ROUNDTRIP_CM;
    // Serial.print("Ping: ");
    // Serial.print(sonar.ping_result / US_ROUNDTRIP_CM);  // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    // Serial.println("cm");
  }
}
bool yellow = false;
bool blue = false;

//myservo.write(100) is straight
void loop() {

  //Ultrasonic sensor ping
  if (millis() >= pingTimer) {    // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;       // Set the next ping time.
    sonar.ping_timer(echoCheck);  // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
  if (huskylens.request()) {
    yellowCount = huskylens.count(yellowID);
    blueCount = huskylens.count(blueID);
   if (leftcheck){
      if (yellowCount >= 1) {
        left = false;
        yellow = true;
        leftcheck = false;
      } else if (blueCount >= 1) {
        left = true;
        leftcheck = false;
        blue = true;
      }
      if (yellowCount >= 1 && blueCount >= 1) {
        HUSKYLENSResult yellowResult = huskylens.getBlock(yellowID, 0);
        HUSKYLENSResult blueResult = huskylens.getBlock(blueID, 0);
        if (yellowResult.yCenter < blueResult.yCenter) {
          left = false;
        } else {
          left = true;
        }
        leftcheck = false;
      }
   }
  }
// if (check){if (yellow){
//   if (yellowCount == 0){
//     delay(200);
//     myservo.write(135);
//     check = false;
//   }
// }
// else if (blue){ 1
//   if (blueCount==0){
//     delay(200);
//     myservo.write(65);
//     check = false;
//   }
// }}
// if (yellowCount ==0 && blueCount ==0 ){
//   if (yellow || blue){
//     delay(500);
//     yellow = false;
//     blue = false;
//     check = true;
//   }
//   myservo.write(100);
// }
  if (curr >= 5) {
    curr = 0;
  }
  dists[curr] = distance;
  curr += 1;
  // Serial.println(distance);
  Serial.println(distance);
  Serial.println(distcheck(dists));
  if (distcheck(dists)&& !turned) {
    // Serial.println("turned");
    if (left) {
      myservo.write(65);
    } else {
      myservo.write(135);
    }
    turned = true;
  } else if (turned) {
    turn += 1;
    // Serial.println(turn);
    turned = false;
    delay(1300);
    if (left) {
      myservo.write(102);
    } else {
      myservo.write(98);
    }
  }

  if (turn >= 1300) {
    stepper.rotate(-1);
    delay(3000);
    stepper.rotate(0);
    while (true) {}
  }
}