#include "Webserver.cpp"

Webserver server;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  server.start("ESP32", "");
}

void loop() {
  server.getPadBack();
}
