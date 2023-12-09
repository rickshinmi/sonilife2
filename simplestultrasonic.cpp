#include "Ultrasonic.h"
#include <M5Stack.h>
Ultrasonic ultrasonic(22);



void setup() {
    Ultrasonic ultrasonic(22);
}

void loop() {
  int RangeInCentimeters = ultrasonic.MeasureInCentimeters();
  Serial.print(RangeInCentimeters);
}