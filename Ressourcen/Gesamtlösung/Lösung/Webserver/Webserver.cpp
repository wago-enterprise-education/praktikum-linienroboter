#include "HardwareSerial.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPUI.h>

class Webserver{
  private: 
    IPAddress IP;

    bool ioSwitch;

    // Modes
    bool followLine;
    bool followLight;
    bool obstacleSensor;
    bool blackLine = true;

    // PAD
    bool padLeft;
    bool padRight;
    bool padFor;
    bool padBack;

    bool hornButton;

    // ESP Components
    uint16_t lineSwitch, lightSwitch, ultrasonic, ioSwitcher;
  public: 
    // Constructor
    Webserver() {
    }

    /**
     * Start ESP Webserver
     * 
     * @param ssid: Network Name
     * @param password: Network Password
     */
    void start(char* ssid = "Webserver", char* password = "") {
      // Setting up Network
      WiFi.softAP(ssid, password);
      IP = WiFi.softAPIP();
      Serial.print(IP);

      // Start ESPUI
      ESPUI.begin("ESP32 Control");

      // IO Switch Component
      ioSwitcher = ESPUI.switcher("I/0", [this] (Control *sender, int type) mutable {
        if(type == S_ACTIVE) {
          ioSwitch = true;
        } else {
          ioSwitch = false;
        }
      }, ControlColor::Dark, false);

      // Tabs
      uint16_t tab1 = ESPUI.addControl(ControlType::Tab, "", "Funktionssteuerung");
      uint16_t tab2 = ESPUI.addControl(ControlType::Tab, "", "Manuelle Steuerung");

      // Line Switch Component
      lineSwitch = ESPUI.addControl(ControlType::Switcher, "Linie folgen", "", ControlColor::Alizarin, tab1, [this] (Control *sender, int type) mutable {
        if(type == S_ACTIVE) {
          followLine = true;
          followLight = false;
          ESPUI.getControl(lightSwitch) -> value = "0";
          ESPUI.getControl(lightSwitch) -> color = ControlColor::Alizarin;
          ESPUI.getControl(lineSwitch) -> color = ControlColor::Emerald;
        } else {
          followLine = false;
          ESPUI.getControl(lineSwitch) -> color = ControlColor::Alizarin;
        }
        ESPUI.updateControl(lightSwitch);
        ESPUI.updateControl(lineSwitch);
      });

      // Light Switch Component
      lightSwitch = ESPUI.addControl(ControlType::Switcher, "Licht folgen", "", ControlColor::Alizarin, tab1, [this] (Control *sender, int type) mutable {
        if(type == S_ACTIVE) {
          followLight = true;
          followLine = false;
          ESPUI.getControl(lineSwitch) -> value = "0";
          ESPUI.getControl(lineSwitch) -> color = ControlColor::Alizarin;
          ESPUI.getControl(lightSwitch) -> color = ControlColor::Emerald;
        } else {
          followLight = false;
          ESPUI.getControl(lightSwitch) -> color = ControlColor::Alizarin;
        }
        ESPUI.updateControl(lightSwitch);
        ESPUI.updateControl(lineSwitch);
      });

      // Ultrasonic Component
      ultrasonic = ESPUI.addControl(ControlType::Switcher, "Hindernissensor", "", ControlColor::Alizarin, tab1, [this] (Control *sender, int type) mutable {
        if(type == S_ACTIVE) {
          obstacleSensor = true;
          ESPUI.getControl(ultrasonic) -> color = ControlColor::Emerald;
        } else {
          obstacleSensor = false;
          ESPUI.getControl(ultrasonic) -> color = ControlColor::Alizarin;
        }
        ESPUI.updateControl(ultrasonic);
      });

      // Line Color Component
      uint16_t select1 = ESPUI.addControl(ControlType::Select, "Linie folgen:", "", ControlColor::Turquoise, tab1, [this] (Control *sender, int type) mutable {
        if(sender -> value == "lineHIGH") {
          blackLine = true;
        } else {
          blackLine = false;
        }
      });

      ESPUI.addControl(ControlType::Option, "Schwarze Linie auf Weiß", "lineHIGH", ControlColor::Turquoise, select1);
      ESPUI.addControl(ControlType::Option, "Weiße Linie auf Schwarz", "lineLOW", ControlColor::Turquoise, select1);

      // Pad Component
      ESPUI.addControl(ControlType::Pad, "Bewegung", "", ControlColor::Dark, tab2, [this] (Control *sender, int type) mutable {
        switch (type) {
          case P_LEFT_DOWN:
            padLeft = true;
            break;
          case P_LEFT_UP:
            padLeft = false;
            break;
          case P_RIGHT_DOWN:
            padRight = true;
            break;
          case P_RIGHT_UP:
            padRight = false;
            break;
          case P_FOR_DOWN:
            padFor = true;
            break;
          case P_FOR_UP:
            padFor = false;
            break;
          case P_BACK_DOWN:
            padBack = true;
            break;
          case P_BACK_UP:
            padBack = false;
            break;
        }

        if(padLeft || padRight || padFor || padBack){
          if(lineSwitch == true) {
            lineSwitch = false;
            ESPUI.getControl(lineSwitch) -> value = "0";
            ESPUI.getControl(lineSwitch) -> color = ControlColor::Alizarin;
            ESPUI.updateControl(lineSwitch);
          }
          
          if(lightSwitch == true){
            lightSwitch = false;
            ESPUI.getControl(lightSwitch) -> value = "0";
            ESPUI.getControl(lightSwitch) -> color = ControlColor::Alizarin;
            ESPUI.updateControl(lightSwitch);
          }
        }
      });

      // Horn Component
      ESPUI.addControl(ControlType::Button, "Hupe", "Hup! Hup!", ControlColor::Alizarin, tab2, [this] (Control *sender, int type) mutable {
        if(type == B_DOWN) {
          hornButton = true;
        } else if(type == B_UP) {
          hornButton = false;
        }
      });
    }

    // Getter Methodes
    bool getPadLeft() {
      return padLeft;
    }

    bool getPadRight() {
      return padRight;
    }

    bool getPadFor() {
      return padFor;
    }

    bool getPadBack() {
      return padBack;
    }

    bool getIOSwitch() {
      return ioSwitch;
    }

    bool getHornButton() {
      return hornButton;
    }

    bool getLineButton() {
      return followLine;
    }

    bool getLightButton() {
      return followLight;
    }

    bool getObstacleButton() {
      return obstacleSensor;
    }

    bool isBlackLine() {
      return blackLine;
    }

    bool isWhiteLine() {
      return !blackLine;
    }
};

