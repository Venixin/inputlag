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
int dists[10];

float duration, distance;
int curr = 0;
unsigned int pingSpeed = 29;
unsigned long pingTimer;
unsigned long t;
bool left = true;
bool leftcheck = true;
bool turned = false;
const int stepsPerRev = 720;
int turn = 0;
int yellowCount = 0;
int blueCount = 0;

MoToStepper stepper(stepsPerRev, STEPDIR);
HUSKYLENS huskylens;
Servo myservo;
void setup() {
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
  stepper.setSpeed(7200);
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(startBtn, INPUT_PULLUP);
  while (digitalRead(startBtn) == HIGH) { myservo.write(100); }
  Serial.println("start");

  stepper.rotate(1);
}

bool distcheck(int arr[]) {
  if (arr[4] < 0) {
    return false;
  }
  int high = 0;
  int low = 0;
  for (int i = 0; i < 9; i++) {
    if (arr[i] < 50) {
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
bool start = true;

bool dircheck = false;
int turndir;

void loop() {
  if (leftcheck) {
    if (huskylens.request()) {
      yellowCount = huskylens.count(yellowID);
      blueCount = huskylens.count(blueID);
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
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 29 / 2;

  dists[curr] = distance;
  curr += 1;
  if (curr == 10) {
    curr = 0;
  }
  delay(100);
  if (distcheck(dists)) {
    if (left) {
      myservo.write(65);
    } else {
      myservo.write(135);
    }
    turned = true;
  } else if (turned) {
    turn += 1;
    turned = false;
    Serial.println(turn);
    delay(450);
    myservo.write(100);
  }

  if (turn >= 13) {
    stepper.rotate(-1);
    delay(3000);
    stepper.rotate(0);
    while (true) {}
  }
}