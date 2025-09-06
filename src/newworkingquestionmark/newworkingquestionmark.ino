#include <Servo.h>
#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <NewPing.h>

#define yellowID 3
#define blueID 4
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define startBtn 7
const unsigned int TRIG_PIN = 13;
const unsigned int ECHO_PIN = 12;
int dists[5] = { -1, -2, -3, -4, -5 };
long distance;
int curr = 0;
NewPing sonar(TRIG_PIN, ECHO_PIN);
unsigned int pingSpeed = 29;
unsigned long pingTimer;
unsigned long t;
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
bool turned = false;
Servo myservo;
float speed = 2000;
int turn = 0;

void setup(){
  pingTimer = millis();
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);
  stepper.setSpeed(speed);

  myservo.attach(9);
  Serial.print(myservo.read());
  myservo.write(100);myservo.write(100);
  delay(1000);
  t = millis();

  Serial.begin(9600);
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
    if ((arr[i + 1] - arr[i]) > 0) {
      high += 1;
    } else if (arr[i] < 50
    ) {
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

bool dircheck = false;
int turndir;

void loop() {
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
    // for (int i = 0; i < 5; i++){
    //   Serial.print(dists[i]);
    //   Serial.print(" ");
    // }
    // Serial.print(distcheck(dists));Serial.print(" ");Serial.print(turned);Serial.print(" ");Serial.print(millis()-t);
    // Serial.println();
    // if (not dircheck){
    //   if (cycle >= 0) {
    //   n += 1;

    //   if (n >= 10000 == 0 && n != 0) {
    //     n = 0;
    //     if (huskylens.requestBlocks()) {
          

    // }


    if (distcheck(dists) && !turned && !start) {
      myservo.write(55);
      turn+=1;
      // Serial.println("turn");
      turned = true;
      t = millis();
    }

    if (millis() - t >= 933 && !distcheck(dists)) {
      myservo.write(100);
      stepper.runSpeed();
      turned = false;
    }
  stepper.runSpeed();
  start = false;
  if (turn >= 13){
  delay(300);
  stepper.stop();
  while(true){}
  }


}