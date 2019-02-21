#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  Serial.println("start bluetooth");
  SerialBT.begin("ESP32");
}

void loop() {

  SerialBT.println("Hello World");
  delay(1000);
}