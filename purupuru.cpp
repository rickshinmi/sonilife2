#include <M5Stack.h>
#include "ServoEasing.hpp"
#include <Arduino.h>

#define SERVO1_PIN  5
#define SERVO2_PIN 18
#define SERVO3_PIN 19
#define SPEED_IN_PIN A0
#define MODE_ANALOG_INPUT_PIN A3

ServoEasing Servo1;
ServoEasing Servo2;

#define START_DEGREE_VALUE  0

void setup() {
    M5.begin();
    M5.Lcd.printf("START %s from %s\nUsing library version %s\n", __FILE__, __DATE__, VERSION_SERVO_EASING);
    delay(2000);

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
    performpurupuru(10, 20);
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
        delay(movingGap * 1000/servoSpeed);
        Serial.write("ru");
        Servo1.startEaseTo(servo1num);
        Servo2.startEaseTo(servo2num);
        delay(movingGap * 1000/servoSpeed);
    }
}
