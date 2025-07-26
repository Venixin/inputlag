#include <HUSKYLENS.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>

HUSKYLENS huskylens;

SoftwareSerial mySerial(0, 1); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11

#define MAGENTA_ID 1

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);

    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(1000);
    }

    if (!huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION)) {
        Serial.print("Failed to set algorithm");
        while (true);
    }

    

}

int magentaBlocks = 0;

void loop() {
    if (huskylens.request()) {

        for (int i = 0; i < huskylens.count(); i++) {
            HUSKYLENSResult result = huskylens.get(i);
            magentaBlocks = huskylens.count(1);
            Serial.println(magentaBlocks);
        }
      }
    delay(100);
} 