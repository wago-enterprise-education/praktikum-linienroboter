#include <stdint.h>
#include "HardwareSerial.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPUI.h>

class Webserver {
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
  uint16_t hornTapper, controlPad;


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
  void start(char *ssid = "Webserver", char *password = "") {
    // Setting up Network
    WiFi.softAP(ssid, password);
    IP = WiFi.softAPIP();
    Serial.print(IP);

    // Start ESPUI
    ESPUI.begin("WAGO Linienroboter");

    // IO Switch Component
    ioSwitcher = ESPUI.switcher(
      "Aus/An", [this](Control *sender, int type) mutable {
        if (type == S_ACTIVE) {
          ioSwitch = true;
          ESPUI.setElementStyle(ioSwitcher, "background: #6EC800; border-bottom: #555");
        } else {
          ioSwitch = false;
          ESPUI.setElementStyle(ioSwitcher, "background: #1F2837; border-bottom: #555");
        }
      },
      ControlColor::None, false);
    ESPUI.setPanelStyle(ioSwitcher, "background: #1F2837; border-bottom: #555");
    // Tabs
    uint16_t tab1 = ESPUI.addControl(ControlType::Tab, "", "Funktionssteuerung");
    uint16_t tab2 = ESPUI.addControl(ControlType::Tab, "", "Manuelle Steuerung");

    // Line Switch Component
    lineSwitch = ESPUI.addControl(ControlType::Switcher, "Linie folgen", "", ControlColor::None, tab1, [this](Control *sender, int type) mutable {
      if (type == S_ACTIVE) {
        followLine = true;
        followLight = false;
        ESPUI.getControl(lightSwitch)->value = "0";
        ESPUI.setElementStyle(lineSwitch, "background: #6EC800; border-bottom: #555");
      } else {
        followLine = false;
        ESPUI.setElementStyle(lineSwitch, "background: #1F2837; border-bottom: #555");
      }
      ESPUI.updateControl(lightSwitch);
      ESPUI.updateControl(lineSwitch);
    });
    ESPUI.setPanelStyle(lineSwitch, "background: #6EC800; border-bottom: #555");
    

    // Light Switch Component
    lightSwitch = ESPUI.addControl(ControlType::Switcher, "Licht folgen", "", ControlColor::None, tab1, [this](Control *sender, int type) mutable {
      if (type == S_ACTIVE) {
        followLight = true;
        followLine = false;
        ESPUI.getControl(lineSwitch)->value = "0";
        ESPUI.setElementStyle(lightSwitch, "background: #6EC800; border-bottom: #555");
      } else {
        followLight = false;
        ESPUI.setElementStyle(lightSwitch, "background: #1F2837; border-bottom: #555");
      }
      ESPUI.updateControl(lightSwitch);
      ESPUI.updateControl(lineSwitch);
    });
    ESPUI.setPanelStyle(lightSwitch, "background: #6EC800; border-bottom: #555");
    // Ultrasonic Component
    ultrasonic = ESPUI.addControl(ControlType::Switcher, "Ueberholmodus", "", ControlColor::None, tab1, [this](Control *sender, int type) mutable {
      if (type == S_ACTIVE) {
        obstacleSensor = true;
        ESPUI.setElementStyle(ultrasonic, "background: #6EC800; border-bottom: #555");
      } else {
        obstacleSensor = false;
        ESPUI.setElementStyle(ultrasonic, "background: #1F2837; border-bottom: #555");
      }
      ESPUI.updateControl(ultrasonic);
    });
    ESPUI.setPanelStyle(ultrasonic, "background: #6EC800; border-bottom: #555");
    // Line Color Component
    uint16_t select1 = ESPUI.addControl(ControlType::Select, "Linie folgen:", "", ControlColor::None, tab1, [this](Control *sender, int type) mutable {
      if (sender->value == "lineHIGH") {
        blackLine = true;
      } else {
        blackLine = false;
      }
    });
    ESPUI.setPanelStyle(select1, "background: #000000; border-bottom: #555");
    ESPUI.addControl(ControlType::Option, "Schwarze Linie auf Weiß", "lineHIGH", ControlColor::None, select1);
    ESPUI.addControl(ControlType::Option, "Weiße Linie auf Schwarz", "lineLOW", ControlColor::None, select1);

    // Pad Component
    controlPad = ESPUI.addControl(ControlType::Pad, "Bewegung", "", ControlColor::None, tab2, [this](Control *sender, int type) mutable {
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
      ESPUI.setPanelStyle(controlPad, "background: #6EC800; border-bottom: #555");

      if (padLeft || padRight || padFor || padBack) {
        if (lineSwitch == true) {
          lineSwitch = false;
          ESPUI.getControl(lineSwitch)->value = "0";
          ESPUI.updateControl(lineSwitch);
        }

        if (lightSwitch == true) {
          lightSwitch = false;
          ESPUI.getControl(lightSwitch)->value = "0";
          ESPUI.updateControl(lightSwitch);
        }
      }
    });

    // Horn Component
    hornTapper = ESPUI.addControl(ControlType::Button, "Hupe", "Hup! Hup!", ControlColor::None, tab2, [this](Control *sender, int type) mutable {
      if (type == B_DOWN) {
        hornButton = true;
      } else if (type == B_UP) {
        hornButton = false;
      }
    });
    ESPUI.setPanelStyle(hornTapper, "background: #6EC800; border-bottom: #555");
    ESPUI.setElementStyle(hornTapper, "background: #1F2837; border-bottom: #555");
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
