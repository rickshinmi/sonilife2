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
    int servoSpeed = 20; //ここでスピードを調整
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
// 変数の宣言
int receivedValue1 = 0;
int receivedValue2 = 0;
char previousChar = '\0';
bool newvaluerecieved = false;

if (Serial.available() > 0) {
    while (Serial.available() > 0) {
        char receivedChar = Serial.read();

        // 直前の値と異なる場合にフラグを立てる
        if (receivedChar != previousChar) {
            newvaluerecieved = true;
            previousChar = receivedChar;
            // ここで新しい文字が受信されたときに実行する処理
            // 例えば、数値を格納するなど
        }

        // ここで受信した値を利用する処理を追加
        // 注意: この行では受信した文字ではなく、前回の文字を使っています

        // 新しい値が受信された場合
        if (newvaluerecieved) {
            // 文字列を構築する
            String receivedString;
            receivedString += receivedChar;

            // 次の文字があるか確認
            while (Serial.available() > 0) {
                receivedChar = Serial.read();
                if (receivedChar == ' ') {
                    // 半角スペースが来たら数値1の終わり
                    break;
                }
                receivedString += receivedChar;
            }

            // 文字列を数値に変換
            receivedValue1 = receivedString.toInt();

            // 次の数値（数値2）を取得
            receivedString = "";
            while (Serial.available() > 0) {
                receivedChar = Serial.read();
                if (receivedChar == '\n') {
                    // 改行が来たら終了
                    break;
                }
                receivedString += receivedChar;
            }

            // 文字列を数値に変換
            receivedValue2 = receivedString.toInt();
            performservoActions(servo1CurrentPos, servo2CurrentPos, receivedValue1, receivedValue2);
            newvaluerecieved = false;
        }

        delay(200);
    }
}

}

