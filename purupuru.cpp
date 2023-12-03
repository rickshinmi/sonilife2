



#include <M5Stack.h>

#include "ServoEasing.hpp"
#include <Arduino.h>
#define SERVO1_PIN  5
#define SERVO2_PIN 18
#define SERVO3_PIN 19
#define SPEED_IN_PIN A0 // 36/VP
#define MODE_ANALOG_INPUT_PIN A3 // 39
ServoEasing Servo1;
ServoEasing Servo2;



void setup() {
    M5.begin();
    M5.Lcd.printf("START %s from %s\nUsing library version %s\n", __FILE__, __DATE__, VERSION_SERVO_EASING);
    delay(2000);

    Serial.begin(115200);


    Servo1.attachWithTrim(SERVO1_PIN, 110, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
    Servo2.attach(SERVO2_PIN, 90, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
}


void loop()
{
    performpurupuru();
    
}



void performpurupuru(int servo1num, int servo2num) {
  Servo1.setEasingType(EASE_LINEAR);
  Servo2.setEasingType(EASE_LINEAR);
  setSpeedForAllServos(180);
  Serial.write("purupuru");
  for (int i = 0; i < 5; ++i) {
    Servo1.setEaseTo(servo1num + 10);
    delay(180);
    Servo2.setEaseTo(servo2num + 10);
    delay(180);
    Servo1.setEaseTo(servo1num - 10);
    delay(180);
    Servo1.setEaseTo(servo2num - 10);
    delay(180);
  }
}