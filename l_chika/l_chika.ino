

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("TEST");
   
  // pinMode(13, OUTPUT);
}
 
void loop() {
//   digitalWrite(13, HIGH);
  Serial.println("hello!");
  delay(500);
  Serial.println("world!");
  // digitalWrite(13, LOW);
  delay(500);
}