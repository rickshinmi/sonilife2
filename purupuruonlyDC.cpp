#include <M5Stack.h>
#include <Arduino.h>
#include <ArduinoOSCWiFi.h>

const int DC_MOTERPIN = 9; // モータードライバーに接続されたピン番号
const int DIRECTIONPIN = 8; // モーターの回転方向を制御するピン番号
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

void DCmoter(int speed) {
  if (speed >= 0) {
    digitalWrite(DIRECTIONPIN, HIGH); // 正転
  } else {
    digitalWrite(DIRECTIONPIN, LOW); // 逆転
    speed = abs(speed); // speedが負の場合、正の値に変換
  }
  analogWrite(DC_MOTERPIN, speed);
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
  pinMode(DC_MOTERPIN, OUTPUT);
  pinMode(DIRECTIONPIN, OUTPUT);
}

void loop() {
  OscWiFi.update(); // 自動的に送受信するために必須
  Serial.println();
  Serial.print(oscDataReceived);
  Serial.print(" ");
  Serial.print(servo1CurrentPos);
  Serial.print(" ");
  Serial.println(servo2CurrentPos);
  if (oscDataReceived) {
      // Call receiveOSCData to get the new positions
      // receiveOSCData();
      DCmoter(oscServo1Pos);
      // Reset the flag after performing servo actions
      oscDataReceived = false;
  }
}