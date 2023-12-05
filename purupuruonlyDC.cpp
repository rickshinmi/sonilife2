#include <M5Stack.h>
#include <Arduino.h>
#include <ArduinoOSCWiFi.h>

const int DC_MOTOR_PIN = 5;   // Assuming servo1 is connected to pin 2
const int DIRECTION_PIN = 21; // Assuming a direction pin for the motor
const int START_DEGREE_VALUE = 0;
const int DEFAULT_MICROSECONDS_FOR_0_DEGREE = 1000;
const int DEFAULT_MICROSECONDS_FOR_180_DEGREE = 2000;

int oscServo1Pos = 0;
bool oscDataReceived = false;

String ssid = "rikiのiPhone";
String pwd = "12345678";
int port = 6667;

IPAddress ip(172, 20, 10, 4);
IPAddress gateway(172, 20, 10, 1);
IPAddress subnet(255, 255, 255, 240);


// Define a timer variable
unsigned long lastOscDataTime = 0;
unsigned long previousMillis = 0;
void sendOSCData(int d) {
    OscWiFi.send("172.20.10.3", 5556, "/distance", d);
}

void oscConnect() {
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

    OscWiFi.subscribe(port, "/light2/servo1/angle", [&](int i) {
        oscServo1Pos = i;
        Serial.println(i);
        oscDataReceived = true;
        delay(10);
    });
}

void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(115200);

    WiFi.begin(ssid, pwd);
    oscConnect();


    pinMode(DC_MOTOR_PIN, OUTPUT);
    pinMode(DIRECTION_PIN, OUTPUT);
}

void DCmotor(int speed) {
    analogWrite(DC_MOTOR_PIN, speed);
}

void nonBlockingDelay(unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
}

void loop() {
    OscWiFi.update();

    if (oscDataReceived) {
        DCmotor(oscServo1Pos);
        delay(5000);
        lastOscDataTime = millis();
        oscDataReceived = false;
    } else {
        if (millis() - lastOscDataTime > 30000) {
            DCmotor(60);
        }
    }
    
}

