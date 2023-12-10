#include <M5Stack.h>
#include "ServoEasing.hpp"
#include <Arduino.h>
#include <ArduinoOSCWiFi.h>

#define SERVO1_PIN  18
#define SERVO2_PIN 5
#define SERVO3_PIN 19
#define SPEED_IN_PIN A0
#define MODE_ANALOG_INPUT_PIN A3

ServoEasing Servo1;
ServoEasing Servo2;

#define START_DEGREE_VALUE  40
int servo1CurrentPos = 40;
int servo2CurrentPos = 40;

void performservoActions(int servo1Pre, int servo2Pre, int servo1Post, int servo2Post) {
    Servo1.setEasingType(EASE_QUADRATIC_IN_OUT);
    Servo2.setEasingType(EASE_QUADRATIC_IN_OUT);
    int servoSpeed = 20;
    int movingGap1 = servo1Pre - servo1Post;
    int movingGap2 = servo2Pre - servo2Post;

    int largerMovingGap = (movingGap1 > movingGap2) ? movingGap1 : movingGap2;

    Servo1.startEaseTo(servo1Post);
    Servo2.startEaseTo(servo2Post);

    delay(largerMovingGap * 1000 / servoSpeed);

    servo1CurrentPos = servo1Post;
    servo2CurrentPos = servo2Post;
}

void setup() {
    M5.begin();
    M5.Power.begin();  // Init Power module

    Serial.begin(115200);

    Servo1.attachWithTrim(SERVO1_PIN, 0, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);

    if (Servo2.attach(SERVO2_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO) {
        M5.Lcd.println("Error attaching servo");
        while (true) {
            delay(1000);
        }
    }
}


void loop() {

    // シリアルデータを受信する
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
     int receivedChar = Serial.read();
    performservoActions(servo1CurrentPos, servo2CurrentPos, receivedChar, receivedChar);
      delay(200);
      M5.Lcd.println(receivedChar);
    }
  } 
}