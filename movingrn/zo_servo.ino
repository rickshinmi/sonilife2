#include <M5Stack.h>
#include "ServoEasing.hpp"
#include <Arduino.h>
#include <ArduinoOSCWiFi.h>
#include "Ultrasonic.h"

#define SERVO1_PIN  5
#define SERVO2_PIN 18
#define SERVO3_PIN 19
#define DC_MOTERPIN 12
#define SPEED_IN_PIN A0
#define MODE_ANALOG_INPUT_PIN A3
Ultrasonic ultrasonic(22);

ServoEasing Servo1;
ServoEasing Servo2;

#define START_DEGREE_VALUE  0
int servo1CurrentPos = 0;
int servo2CurrentPos = 0;
int oscServo1Pos = 0;
int oscServo2Pos = 0;

// Flag to indicate whether OSC data has been received
bool oscDataReceived = false;


void performpurupuru(int servo1num, int servo2num) {
    Servo1.setEasingType(EASE_QUADRATIC_OUT);
    Servo2.setEasingType(EASE_QUADRATIC_OUT);
    int servoSpeed = 20;
    int movingGap = random(50,90);
    int movingGapver = 50;

    setSpeedForAllServos(servoSpeed);
    Serial.write("purupuru");
    for (int i = 0; i < 5; ++i) {
        Serial.write("pu");
        Servo1.startEaseTo(servo1num + movingGapver);
        Servo2.startEaseTo(servo2num + movingGap);
        delay(movingGapver * 1000/servoSpeed);

        Serial.write("ru");
        Servo1.startEaseTo(servo1num);
        Servo2.startEaseTo(servo2num);
        delay(movingGapver * 1000/servoSpeed);
        
    }
}

void DCmoter(int speed){
  analogWrite(DC_MOTERPIN, speed);
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
    int RangeInCentimeters = ultrasonic.MeasureInCentimeters();
    
        // If OSC data has not been received, continue performing purupuru
        performpurupuru(40,40);
    }
    
