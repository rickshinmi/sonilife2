#include <M5Stack.h>
#include <Arduino.h>

const int DC_MOTOR_PIN = 5;   // Assuming servo1 is connected to pin 2
const int START_DEGREE_VALUE = 0;
const int DEFAULT_MICROSECONDS_FOR_0_DEGREE = 1000;
const int DEFAULT_MICROSECONDS_FOR_180_DEGREE = 2000;
static bool newvaluerecieved = false;     // フラグの初期値を設定


int oscServo1Pos = 0;


unsigned long lastOscDataTime = 0;
unsigned long previousMillis = 0;
void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(115200);
    pinMode(DC_MOTOR_PIN, OUTPUT);
}


void nonBlockingDelay(unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
}

void renda(){
  const int numIterations = 20; // Change this value as needed
  Serial.print("renda");

for (int i = 0; i < numIterations; ++i) {
    analogWrite(DC_MOTOR_PIN, 250);
    delay(50);
    analogWrite(DC_MOTOR_PIN, 0);
    delay(100);
}
}

void DCmotorWithUpDown(int speed) {
    analogWrite(DC_MOTOR_PIN, speed - 40);
    delay(1000);
    analogWrite(DC_MOTOR_PIN, speed - 30);
    delay(1000);
    analogWrite(DC_MOTOR_PIN, speed - 10);
    delay(1000);
    analogWrite(DC_MOTOR_PIN, speed);
    while(newvaluerecieved = false){
          delay(500, 4000);
    }
      renda();
  }

void loop() {
  static int previousChar = -1; // 直前の受信した文字の初期値を設定
  // シリアルデータを受信する
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      int receivedChar = Serial.read();

      // 直前の値と異なる場合にフラグを立てる
      if (receivedChar != previousChar) {
        newvaluerecieved = true;
        previousChar = receivedChar;
      }

      // ここで受信した値を利用する処理を追加
      DCmotorWithUpDown(receivedChar);

      delay(200);
    }
  }
}
