#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"
#include <SoftwareSerial.h>
SoftwareSerial s(0,1);
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
  s.begin(115200); 
}

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& table = jsonBuffer.createObject();

void loop() {
  table["temp1"] = bme.readTemperature();
  table["pres1"] = bme.readPressure();
  table["temp2"] = bme2.readTemperature();
  table["pres2"] = bme2.readPressure();
  table["gas"]= analogRead(A0);
  table["smok"] = analogRead(A1);
  table["hum1"] = dht.readHumidity();
  table["hum2"]= dht2.readHumidity();
  if(s.available()>0) {
  root.printTo(s);
  }
  delay(3000);
}
