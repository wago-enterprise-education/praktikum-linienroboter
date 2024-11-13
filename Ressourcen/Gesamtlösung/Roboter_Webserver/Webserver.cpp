#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPUI.h>

class Webserver{
  private: 
    IPAddress IP;
    int var;
  public: 
  
    Webserver(char* ssid = "Webserver", char* password = "") {
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
    IP = WiFi.softAPIP();
  // Start ESPUI
  ESPUI.begin("ESP32 Control");

  // Add a button with color and default label
  ESPUI.button("Button1", [] (Control *sender, int type) {
    Serial.println("Button1 pressed");
  }, ControlColor::Turquoise, "Press");

   // Add a button with color and default label
  ESPUI.button("Button2", [] (Control *sender, int type) {
    Serial.println("Button2 pressed");
  }, ControlColor::Turquoise, "Press");

    ESPUI.button("Button3", [] (Control *sender, int type) {
    Serial.println("Button3 pressed");
  }, ControlColor::Turquoise, "Press");
  
    }
};


