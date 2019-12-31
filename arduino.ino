#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"
#include <SoftwareSerial.h>
SoftwareSerial s(4,5);
#include <ArduinoJson.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bme;
Adafruit_BMP280 bme2(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

DHT dht(6, DHT11);
DHT dht2(7, DHT11);
  
void setup() {
  bme.begin();
  bme2.begin();
  dht.begin();
  dht2.begin();
  s.begin(9600); 
  Serial.begin(9600);
}

void loop() {
  int g = 0;
  int ss = 0;
  float t1 = 0;
  float t2 = 0;
  int h1 = 0;
  int h2 = 0;
  float p1 = 0; 
  float p2 = 0; 
  int a = 4;
  int b = 0;
  
  while(a != b) {
    t1 += bme.readTemperature();
    p1 += bme.readPressure();
    t2 += bme2.readTemperature();
    p2 += bme2.readPressure();
    g += analogRead(A0);
    ss += analogRead(A1);
    h1 += dht.readHumidity();
    h2 += dht2.readHumidity();
    delay(500);
    b++;
  }
  if ( h1 > 5000 || h2 > 5000) {
    h1 = 4;
    h2 = 4;
  }
  
  DynamicJsonDocument table(256);
  table["temp1"] = t1 / a;
  table["pres1"] = p1 / a;
  table["temp2"] = t2 / a;
  table["pres2"] = p2 / a;
  table["gas"] = g / a;
  table["smok"] = ss / a;
  table["hum1"] = h1 / a;
  table["hum2"] = h2 / a;
  serializeJson(table, s);
  serializeJsonPretty(table, Serial);
}
