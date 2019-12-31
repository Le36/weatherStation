#include <ESP8266WiFi.h>
#include "DNSServer.h"
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
SoftwareSerial s(15,13);
#include <ArduinoJson.h>
#include <FS.h>
#include "index.h"
const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(5, 5, 5, 5);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
ESP8266WebServer  webServer(80);          // HTTP server


float temp1 = 0; float pres1 = 0; float temp2 = 0; float pres2 = 0; int gas = 0; int smoke = 0; int hum1 = 0; int hum2 = 0;

void handleGraph() {
  String graphh = graphPage; //Read HTML contents
  webServer.send(200, "text/html", graphh); //Send web page
}
 
void handleData() {
  webServer.send(200, "text/plane", data(temp1, temp2, hum1, hum2, pres2)); //Send data to index.h graph
}

void handleChartminjs() {
  File file = SPIFFS.open("/Chart.min.js", "r");
  webServer.streamFile(file, "application/javascript");
  file.close();
}

String data(float llampo1, float llampo2, int kostt1, int kostt2,float presssu) {
  String temp = "{\"temp1\":\""+String(llampo1)+"\", \"temp2\":\""+ String(llampo2) +"\", \"hum1\":\""+ String(kostt1) +"\", \"hum2\":\""+ String(kostt2) +"\",\"pres2\":\""+ String(presssu) +"\"}";
  return temp;
}

void httpDefault() {
  webServer.sendHeader("Location", "http://w.s", true);
  webServer.send(302, "text/plain", "");
  webServer.client().stop();
}

void httpHome() {
  if (webServer.hostHeader() != String("w.s")) {
    return httpDefault();
  }
  webServer.send(200, "text/html", sendHTML(temp1,temp2,pres1,pres2,gas,smoke,hum1,hum2));
}

String sendHTML(float tempSensor1,float tempSensor2,float presSensor1,float presSensor2,int kaasu,int savu,int humidity1, int humidity2){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<body style=\"background-color: #1c1c1c;\">\n";
  ptr +="<title>Weather Station</title>\n";
  ptr +="<style>html { font-family: Lucida Console; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #f7f7f7;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #f7f7f7;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,2000);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.body.innerHTML =this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Weather Station</h1>\n";
  ptr +="<p>Temperature1: ";
  ptr +=tempSensor1;
  ptr +=" &deg;C</p>";
  ptr +="<p>Temperature2: ";
  ptr +=tempSensor2;
  ptr +=" &deg;C</p>";
  ptr +="<p>Pressure1: ";
  ptr +=presSensor1;
  ptr +=" Pa</p>";
  ptr +="<p>Pressure2: ";
  ptr +=presSensor2;
  ptr +=" Pa</p>";
  ptr +="<p>Gas: ";
  ptr +=kaasu;
  ptr +=" ppm</p>";
  ptr +="<p>Smoke: ";
  ptr +=savu;
  ptr +=" #</p>";
  ptr +="<p>Humidity1: ";
  ptr +=humidity1;
  ptr +=" %</p>";
  ptr +="<p>Humidity2: ";
  ptr +=humidity2;
  ptr +=" %</p>";
  ptr +="<p><a href=\"/graph\"><font color=\"#00bbff\">Live Graph</font></a></p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
void setup() {
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("\\- Weather Station -/"); // WiFi name

  SPIFFS.begin();
  
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.on("/", httpHome);
  webServer.on("/graph", handleGraph);      
  webServer.on("/readData", handleData);
  webServer.on("/Chart.min.js", handleChartminjs);
  webServer.onNotFound(httpDefault);
  webServer.begin();
  s.begin(9600); 
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  if(s.available()>0) {
    DynamicJsonDocument table(256);
    deserializeJson(table, s);
    if (table["temp1"] == 0 && table["pres2"] == 0) {
      //do nothing
    } else if (table["hum1"] == 0 || table["hum2"] == 0) {
      //do nothing
    } else {
      temp1 = table["temp1"];
      pres1 = table["pres1"];
      temp2 = table["temp2"];
      pres2 = table["pres2"];
      gas = table["gas"];
      smoke = table["smok"];
      hum1 = table["hum1"];
      hum2 = table["hum2"];
    }
  }
}



