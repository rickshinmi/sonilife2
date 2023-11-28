

#include <M5Stack.h>

#include "ServoEasing.hpp"
#include <Arduino.h>

// Your additional includes here, if needed

#define SERVO1_PIN  5
#define SERVO2_PIN 18
#define SERVO3_PIN 19
#define SPEED_IN_PIN A0 // 36/VP
#define MODE_ANALOG_INPUT_PIN A3 // 39
ServoEasing Servo1;
ServoEasing Servo2;
unsigned long previousMillis = 0;

#define START_DEGREE_VALUE  0



void nonBlockingDelay(unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
}



void setup() {
    M5.begin();
    M5.Lcd.printf("START %s from %s\nUsing library version %s\n", __FILE__, __DATE__, VERSION_SERVO_EASING);
    delay(2000);

    Serial.begin(115200);


    Servo1.attachWithTrim(SERVO1_PIN, 110, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);

    if (Servo2.attach(SERVO2_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO) {
        M5.Lcd.println("Error attaching servo");
        while (true) {
            delay(1000);
        }
    }

    setSpeedForAllServos(30);
    Servo1.setEasingType(EASE_CUBIC_IN_OUT);
    Servo2.setEasingType(EASE_CUBIC_IN_OUT);
    delay(500);
}

void loop() {
    Serial.print("関数1");
    setSpeedForAllServos(30);
    Servo1.setEaseTo(130.0f);
    Servo2.setEaseTo(130.0f);
    synchronizeAllServosStartAndWaitForAllServosToStop();

    Serial.println("関数2");
    setSpeedForAllServos(60);
    Servo1.setEaseTo(100.0f);
    Servo2.startEaseTo(60);

    while (ServoEasing::areInterruptsActive()) {
        nonBlockingDelay(10);
        Serial.print("関数3");
    }

    Serial.println("関数4");

    Servo1.setEasingType(EASE_CUBIC_IN_OUT);
    ServoEasing::ServoEasingNextPositionArray[0] = 30.0f;
    ServoEasing::ServoEasingNextPositionArray[1] = 90.0f;
    setEaseToForAllServosSynchronizeAndStartInterrupt(90);

    while (ServoEasing::areInterruptsActive()) {
        nonBlockingDelay(10);
    }
    Servo1.setEasingType(EASE_LINEAR);

    delay(300);
    Serial.println("関数5");
    Servo1.setEaseTo(130.0f);
    Servo2.startEaseTo(160.0f);

    while (ServoEasing::areInterruptsActive()) {
        delay(10);
    }

    delay(500);
}
