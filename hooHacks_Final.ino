#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseFS.h>
#include <FirebaseJson.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Button2.h"; 
#include "ESPRotary.h";
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#define FIREBASE_HOST "DATABASE URL"
#define FIREBASE_AUTH "DATABASE SECRET"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"
#define API_KEY "API KEY"

const int FLEX_PIN = A0; // Pin connected to voltage divider output
#define ROTARY_PIN1  D7
#define ROTARY_PIN2 D6
#define BUTTON_PIN  D4
#define CLICKS_PER_STEP   4

const float VCC =4.98; // Measured voltage of Ardunio 5V line
const float R_DIV =47500.0; // Measured resistance of 3.3k resistor
const float STRAIGHT_RESISTANCE =37300.0; // resistance when straight
const float BEND_RESISTANCE =90000.0; // resistance at 90 deg

ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
Button2 b = Button2(BUTTON_PIN);

FirebaseData fbdo;
Adafruit_MPU6050 mpu;
int count =0;
float high=0.0f;
float low=0.0f;

void setup()
{
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  Serial.println("Init");
  pinMode(A0, INPUT);
  pinMode(D3, INPUT_PULLUP);
  rotarySetup();
  WifiSetup();
  getAccelValues();
//  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), IntCallback, RISING);
//  attachInterrupt(digitalPinToInterrupt(A0), flexCallback, RISING);
  dataCallibrate();

}

//void flexCallback() {
//  float sentData= readFlex();
////  Firebase.setFloat(fbdo, "/Devices/DeviceId1/Flex", sentData)
//}
//
//void IntCallback() {
//  
//}

void loop()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  String statusC;
  Firebase.getString(fbdo, "DeviceId1/Calibration/status", statusC);
  if(statusC=="3"){
    dataCallibrate();
  }
  float dataFlex = readFlex();
  printAccelValues();
  rotaryUpdates();
//  count++;
}

float readFlex() {
// Read the ADC, and calculate voltage and resistance from it
int flexADC=analogRead(FLEX_PIN);
float flexV=flexADC* VCC / 1023.0;
float flexR= R_DIV * (VCC / flexV-1.0);
//Serial.println("Resistance: "+String(flexR) +" ohms");

// Use the calculated resistance to estimate the sensor's
// bend angle:
float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE, 0, 90.0);
//Serial.println("Bend: "+String(angle) +" degrees");
//Serial.println();
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.setFloat(fbdo, "/Devices/DeviceId1/Flex", angle);
return angle;
//  delay(500);
}

void getAccelValues() {
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  mpu.begin();
  }

  
void printAccelValues() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  mpu.begin();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.setFloat(fbdo, "/Devices/DeviceId1/Accel/X", a.acceleration.x);
  Firebase.setFloat(fbdo, "/Devices/DeviceId1/Accel/Y", a.acceleration.y);
  Firebase.setFloat(fbdo, "/Devices/DeviceId1/Accel/Z", a.acceleration.z);

  delay(500);
}

void rotarySetup() {
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);

  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);
}

void rotaryUpdates() {
  r.directionToString(r.getDirection());
  float hey = r.getPosition();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setFloat(fbdo, "/Devices/DeviceId1/RE/", hey);
}


void WifiSetup() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void rotate(ESPRotary& r) {
   Serial.println(r.getPosition());
   float h = r.getPosition();
   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   Firebase.setFloat(fbdo, "/Devices/DeviceId1/RE/", h);
   
}

// on left or right rotation
void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}
 
// single click
void click(Button2& btn) {
  Serial.println("Click!");
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}

void dataCallibrate() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  String statusC="2";
  int i=0;
  float highx=0.0f;
  float highy=0.0f;
  float highz=0.0f;
  float lowx=a.acceleration.x;
  float lowy=a.acceleration.y;
  float lowz=a.acceleration.z;
  float highFlex=readFlex();
  float lowFlex=readFlex();
  float tempr=0.0f;
  for(i=0;i<50;i++) {
    if(a.acceleration.x>highx) highx=a.acceleration.x;
    if(a.acceleration.x<lowx) lowx=a.acceleration.x;
    if(a.acceleration.y>highy) highx=a.acceleration.y;
    if(a.acceleration.y<lowy) lowy=a.acceleration.y;
    if(a.acceleration.z>highz) highx=a.acceleration.z; 
    if(a.acceleration.z<lowz) lowz=a.acceleration.z;
    tempr=readFlex();
    if(tempr>highFlex) highFlex=tempr;
    if(tempr<lowFlex) lowFlex=tempr;
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Accel/X/high", highx);
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Accel/X/low", lowx);
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Accel/Y/high", highy);
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Accel/Y/low", lowy);
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Accel/Z/high", highz);
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Accel/Z/low", lowz);
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Flex/high", highFlex);
  Firebase.setFloat(fbdo, "/Calibration/DeviceId1/Flex/low", lowFlex);
  Firebase.setString(fbdo, "/Calibration/DeviceId1/status", statusC);
}
  


  
