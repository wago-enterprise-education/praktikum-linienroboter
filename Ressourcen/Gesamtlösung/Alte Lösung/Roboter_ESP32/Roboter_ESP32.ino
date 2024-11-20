#include "Robotersteuerung_Fahrtrichtung.h"
#include "Robotersteuerung_Funktionen.h"
#include "Webserver.h"

#define LOGGING (true)


TaskHandle_t webserverTask;

//---------------------------------------------------------------------------------
void setup()
{
  // SETUP SERIELLE SCHNITTSTELLE                                                   // fuer Ausgaben auf seriellen Monitor zum Debuggen
  Serial.begin(9600);
  delay(1000);
#if(LOGGING)
  Serial.println("SETUP");
#endif

  roboter_setup();
  webserver_setup();


#if(LOGGING)
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
#endif

  xTaskCreatePinnedToCore(
    webserverloop, /* Function to implement the task */
    "Webserver Task", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    5,  /* Priority of the task */
    &webserverTask,  /* Task handle. */
    0); /* Core where the task should run */
}


void loop() {
  roboter_loop();
}

void webserverloop(void * parameter){
#if(LOGGING)
  Serial.print("webserverTask running on core ");
  Serial.println(xPortGetCoreID());
#endif
  for(;;) {
    webserver_loop();
    vTaskDelay(2);
  }
}
