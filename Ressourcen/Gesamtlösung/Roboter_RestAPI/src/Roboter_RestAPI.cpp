#include <Arduino.h>
#include "Roboter_Funktionen.h"
#include "Webserver.h"



void setup() {
  Serial.begin(115200);
  webserver_setup();
  robot_setup();
}

void loop() {

  int mode = ws_erhalte_betriebsmodus();
  if (mode == 4) {
    // manuelle Steuerung
  }
  else if (mode == 2) {
    linie_folgen();
  }
  else if (mode == 1) {
    licht_folgen();
  }
  else {
    anhalten();
  }

  robot_loop();
}