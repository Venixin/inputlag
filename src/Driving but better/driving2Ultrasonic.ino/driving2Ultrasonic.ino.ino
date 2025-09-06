#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <NewPing.h>
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
  #define startBtn 7

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
long distance;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

//Create ultrasonic object
NewPing sonar(TRIG_PIN, ECHO_PIN);
unsigned int pingSpeed = 29;
unsigned long pingTimer;

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
float speed = 2000;


void setup() {
  pingTimer = millis();
  // Set the maximum speed and acceleration:
  Serial.begin(9600);
  Wire.begin();
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);
  stepper.setSpeed(speed);

  myservo.write(100);
  // Setup steering servo
  myservo.attach(9);
  // reset to forward
  myservo.write(100);
  delay(1000);
  t = millis();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1000);
  }
  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }
  pinMode(startBtn, INPUT_PULLUP);
  while (digitalRead(startBtn) == LOW) { myservo.write(100); }
  Serial.println("start");
}


bool distcheck(int arr[]) {
  if (arr[4] < 0) {
    return false;
  }
  int high = 0;
  int low = 0;
  for (int i = 0; i < 4; i++) {
    stepper.runSpeed();
    if ((arr[i + 1] - arr[i]) > 0) {
      high += 1;
    } else if (arr[i] < 20) {
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

long echoCheck() {  // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) {  // This is how you check to see if the ping was received.
    // Here's where you can add code.
    distance = sonar.ping_result / US_ROUNDTRIP_CM;
    Serial.print("Ping: ");
    Serial.print(sonar.ping_result / US_ROUNDTRIP_CM);  // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");
  }
}


//myservo.write(100) is straight
void loop() {

    //Ultrasonic sensor ping
    if (millis() >= pingTimer) {    // pingSpeed milliseconds since last ping, do another ping.
      pingTimer += pingSpeed;       // Set the next ping time.
      sonar.ping_timer(echoCheck);  // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
    }

    if (curr >= 5) {
      curr = 0;
    }
    dists[curr] = distance;
    curr += 1;
    // Serial.println(distance);

    stepper.runSpeed();
    if (distcheck(dists) && !turned) {
      myservo.write(50);
      // Serial.println("turn");
      turned = true;
      t = millis();
    }

    if (millis() - t >= 900 && !distcheck(dists)) {
      myservo.write(100);
      stepper.runSpeed();
      turned = false;
    }
  

  stepper.runSpeed();
}