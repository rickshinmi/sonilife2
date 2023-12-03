#include <M5Stack.h>
#include "ServoEasing.hpp"
#include <Arduino.h>
#include <ArduinoOSCWiFi.h>

#define SERVO1_PIN  5
#define SERVO2_PIN 18
#define SERVO3_PIN 19
#define SPEED_IN_PIN A0
#define MODE_ANALOG_INPUT_PIN A3

ServoEasing Servo1;
ServoEasing Servo2;

#define START_DEGREE_VALUE  0
int servo1CurrentPos = 0;
int servo2CurrentPos = 0;
int oscServo1Pos = 0;
int oscServo2Pos = 0;

// Flag to indicate whether OSC data has been received
bool oscDataReceived = false;

void receiveOSCData() {
    int oscServo1Pos = /* obtain new position from OSC */;
    int oscServo2Pos = /* obtain new position from OSC */;

    servo1CurrentPos = oscServo1Pos;
    servo2CurrentPos = oscServo2Pos;
    oscDataReceived = true;
}

void performpurupuru(int servo1num, int servo2num) {
    Servo1.setEasingType(EASE_CUBIC_IN_OUT);
    Servo2.setEasingType(EASE_CUBIC_IN_OUT);
    int servoSpeed = 180;
    int movingGap = 10;

    setSpeedForAllServos(servoSpeed);
    Serial.write("purupuru");
    for (int i = 0; i < 5; ++i) {
        Serial.write("pu");
        Servo1.startEaseTo(servo1num + movingGap);
        Servo2.startEaseTo(servo2num + movingGap);
        delay(movingGap * 1000/servospeed);
        Serial.write("ru");
        Servo1.startEaseTo(servo1num);
        Servo2.startEaseTo(servo2num);
        delay(movingGap * 1000/servospeed);
    }
}

void performservoActions(int servo1Pre, int servo2Pre, int servo1Post, int servo2Post) {
    Servo1.setEasingType(EASE_QUADRATIC_IN_OUT);
    Servo2.setEasingType(EASE_QUADRATIC_IN_OUT);
    int servoSpeed = 60;
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
      Serial.begin(115200);

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
    if (!oscDataReceived) {
        // If OSC data has not been received, continue performing purupuru
        performpurupuru();
    } else {
        // Call receiveOSCData to get the new positions
        receiveOSCData();
        
        // Perform servo actions with new positions
        performservoActions(servo1CurrentPos, servo2CurrentPos, oscServo1Pos, oscServo2Pos);

        // Reset the flag after performing servo actions
        oscDataReceived = false;
    }
}

