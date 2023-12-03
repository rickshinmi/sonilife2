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

String ssid = "rikiのiPhone";
String pwd = "12345678";
int port = 6667;



//初回接続時にシリアルモニタにて確認し、書き込む
IPAddress ip(172, 20, 10, 4);
IPAddress gateway(172, 20, 10, 1);
IPAddress subnet(255, 255, 255, 240);

//引数にM4Lに送信したい値を入れて呼び出すと送信される
void sendOSCData(int d) {
    OscWiFi.send("172.20.10.3", 5556, "/distance", d);
}

//void receiveOSCData() {
    //oscServo1Pos = 10/* obtain new position from OSC */;
    //oscServo2Pos = 20/* obtain new position from OSC */;
    //oscDataReceived = true;
//}

void performpurupuru(int servo1num, int servo2num) {
    Servo1.setEasingType(EASE_CUBIC_IN_OUT);
    Servo2.setEasingType(EASE_CUBIC_IN_OUT);
    int servoSpeed = 50;
    int movingGap = 35;

    setSpeedForAllServos(servoSpeed);
    
    Serial.write("pu");
    Servo1.startEaseTo(servo1num + movingGap);
    Servo2.startEaseTo(servo2num + movingGap);
    delay(movingGap * 1000/(servoSpeed* 2));
    Serial.write("ru");
    Servo1.startEaseTo(servo1num);
    Servo2.startEaseTo(servo2num);
    delay(movingGap * 1000/(servoSpeed* 2));
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

void oscConnect()
{
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  M5.Lcd.println("");
  M5.Lcd.println("WiFi connected.");
  M5.Lcd.println("IP address: ");
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.println("Gateway address: ");
  M5.Lcd.println(WiFi.gatewayIP());
  M5.Lcd.println("Subnet address: ");
  M5.Lcd.println(WiFi.subnetMask());

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Gateway address: ");
  Serial.println(WiFi.gatewayIP());
  Serial.println("Subnet address: ");
  Serial.println(WiFi.subnetMask());

  OscWiFi.subscribe(port, "/light2/servo1/angle", [&](int i) //port(6666)の"/light1/servo1/angleでサーボの値を受け取る
  {
    oscServo1Pos = i; //値をoscServo1Posに代入
    oscDataReceived = true;
    delay(10);            
  });


  OscWiFi.subscribe(port, "/light2/servo2/angle", [&](int i) //port(6666)の"/light1/servo2/angleでサーボの値を受け取る
  {
    oscServo2Pos = i; //値をoscServo2Posに代入
    oscDataReceived = true; 
    delay(10);             
  });
}

void setup() {
    M5.begin();
    M5.Power.begin();  // Init Power module

    Serial.begin(115200);

    WiFi.begin(ssid, pwd);
    oscConnect();

    Servo1.attachWithTrim(SERVO1_PIN, 0, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);

    if (Servo2.attach(SERVO2_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO) {
        M5.Lcd.println("Error attaching servo");
        while (true) {
            delay(1000);
        }
    }
}


void loop() {
  OscWiFi.update(); // 自動的に送受信するために必須
  Serial.println();
  Serial.print(oscDataReceived);
  Serial.print(" ");
  Serial.print(servo1CurrentPos);
  Serial.print(" ");
  Serial.println(servo2CurrentPos);
  if (!oscDataReceived) {
      // If OSC data has not been received, continue performing purupuru
      performpurupuru(servo1CurrentPos, servo2CurrentPos);
  } else {
      // Call receiveOSCData to get the new positions
      // receiveOSCData();
      
        
      // Perform servo actions with new positions
      performservoActions(servo1CurrentPos, servo2CurrentPos, oscServo1Pos, oscServo2Pos);

      // Reset the flag after performing servo actions
      oscDataReceived = false;
  }
}
