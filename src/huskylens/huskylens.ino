#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
HUSKYLENS huskylens;

void printResult(HUSKYLENSResult result);


void setup() {

  Serial.begin(115200);
  Wire.begin();
  Serial.println("Begin");
  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed!");
    Serial.println("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>> I2C)");
    Serial.println("2.Please recheck the connection.");
    delay(1000);
  }
  Serial.println("next");
  if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
    Serial.print("Failed to set algorithm");
  }
  Serial.println("Started");
}

int magentaBlocks = 0;

void loop() {
  if (huskylens.request()) {

    for (int i = 0; i < huskylens.count(); i++) {
      HUSKYLENSResult result = huskylens.get(i);
      magentaBlocks = huskylens.count(1);
      if (huskylens.count(1) > 0){
        Serial.println("RED");
      }
      if (huskylens.count(2) > 0){
        Serial.println("BLUE");
      }
      if (huskylens.count(3) > 0){
        Serial.println("GREEN");
      }
      Serial.println();
    }
  }
  delay(100);
}