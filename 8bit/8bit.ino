// #include <WiFiClient.h>
// #include <ESP8266WiFi.h>
#include <WiFi.h>
// #include <ESP8266WebServer.h>
// #include <WebServer.h>
#include <FS.h>
#include "private.h"
#define LED 13                    //LED点灯に使用するピン
// ESP8266WebServer server(80);
WiFiServer server(80);
uint8_t buf[BUFFER_SIZE];
#if 0
void onroot() {
    String msg ;
    msg += "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"><title>LED Button</title></head><body>";
    msg += "<script>";
    msg += "function sendOn(){" ;
    msg += "send(\"/on/\");";
    msg += "document.getElementById(\"LEDstatus\").innerHTML=\"ON!\";";
    msg += "}";
    msg += "function sendOff(){";
    msg += "send(\"/off/\");";
    msg += "document.getElementById(\"LEDstatus\").innerHTML=\"OFF!\";";
    msg += "}";
    msg += "function send(url){";
    msg += "var xhr = new XMLHttpRequest();";
    msg += "xhr.open(\"GET\", url, true);";
    msg += "xhr.send();";
    msg += "}";
    msg += "</script>";
    msg += "<button id=\"on\" onClick=sendOn()>LED ON</button>";
    msg += "<button id=\"off\" onClick=sendOff()>LED OFF</button>";
    msg += "<p id=\"LEDstatus\"></p>";
    msg += "</body></html>";

    server.send(200, "text/html", msg);
}
#endif
void onroot(){
  Serial.println("Access");
  server.send(200, "text/html", (char *)buf);
  return;
}
bool readHTML(){
  SPIFFS.begin();
  File htmlFile = SPIFFS.open(HTML_PATH, "r");
  if (!htmlFile) {
    Serial.println("Failed to open index.html");
    return false;
  }
  size_t size = htmlFile.size();
  if(size >= BUFFER_SIZE){
    Serial.print("File Size Error:");
    Serial.println((int)size);
  }else{
    Serial.print("File Size OK:");
    Serial.println((int)size);
  }
  htmlFile.read(buf,size);
  htmlFile.close();
  return true;
}

void ReceiveForm(){
  Serial.println("Receive POST!");
  if(server.method() == HTTP_POST){
    String s = server.arg("mytext");
    Serial.println(s);
    if(s == "1"){
      Serial.println("rx 1");
    }
    else{
      Serial.println("none 1");
    }
  }
  server.send(200, "text/html","OK");
}

void LedOn(){
  Serial.println("ON");
  digitalWrite(LED,HIGH);
  server.send(200, "text/html","OK");
}

void LedOff(){
  Serial.println("OFF");
  digitalWrite(LED,LOW);
  server.send(200, "text/html","OK");
}



void setup() {

    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    if(!readHTML()){
      Serial.println("Read HTML error!!");
    }
    Serial.println("");
    Serial.println("ESP8266 Wifi test");

    WiFi.config(IPAddress(192, 168, 137, 131), WiFi.gatewayIP(), WiFi.subnetMask());
    WiFi.begin(SSID, PASSWORD);
    WiFi.mode(WIFI_STA);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected as ");
    Serial.println(WiFi.localIP());

    // Web server setting
    server.on("/", onroot);
    server.on("/text/", ReceiveForm);
    server.on("/on/", LedOn);
    server.on("/off/", LedOff);
    server.begin();
}

void loop() {
    server.handleClient();
}