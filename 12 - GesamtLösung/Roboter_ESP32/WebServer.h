#define LOGGING (true)
#include <DNSServer.h>

#include <DNSServer.h>
#include <ESPUI.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

const char *ssid = "LinienRoboter";
const char *password = "wago";

const char *hostname = "wago";

uint16_t homeTab, manuellTab, lichtSwitch, linieSwitch, manuellSwitch, hindernisSwitch, linieSelect, linie0, linie1, manuellPad;

int betriebsmodus = -1, hindernis = 0;

void lichtSwitchAction(Control *sender, int value) {
  switch (value) {
    case S_ACTIVE:
      betriebsmodus = 0;
      ESPUI.getControl(lichtSwitch)->color = ControlColor::Emerald;
      ESPUI.updateControl(lichtSwitch);

      ESPUI.getControl(linieSwitch)->value = "0";
      ESPUI.getControl(linieSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(linieSwitch);
      ESPUI.getControl(manuellSwitch)->value = "0";
      ESPUI.getControl(manuellSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(manuellSwitch);
      break;
    case S_INACTIVE:
      betriebsmodus = -1;
      ESPUI.getControl(lichtSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(lichtSwitch);
      break;
  }
#if(LOGGING)
  Serial.print(" ");
  Serial.println(sender->id);
#endif
}

void linieSwitchAction(Control *sender, int value) {
  switch (value) {
    case S_ACTIVE:
      betriebsmodus = 1;
      ESPUI.getControl(linieSwitch)->color = ControlColor::Emerald;
      ESPUI.updateControl(linieSwitch);

      ESPUI.getControl(lichtSwitch)->value = "0";
      ESPUI.getControl(lichtSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(lichtSwitch);
      ESPUI.getControl(manuellSwitch)->value = "0";
      ESPUI.getControl(manuellSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(manuellSwitch);
      break;
    case S_INACTIVE:
      betriebsmodus = -1;
      ESPUI.getControl(linieSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(linieSwitch);
      break;
  }
#if(LOGGING)
  Serial.print(" ");
  Serial.println(sender->id);
#endif
}

void manuellSwitchAction(Control *sender, int value) {
  switch (value) {
    case S_ACTIVE:
      betriebsmodus = 2;
      ESPUI.getControl(manuellSwitch)->color = ControlColor::Emerald;
      ESPUI.updateControl(manuellSwitch);

      ESPUI.getControl(lichtSwitch)->value = "0";
      ESPUI.getControl(lichtSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(lichtSwitch);
      ESPUI.getControl(linieSwitch)->value = "0";
      ESPUI.getControl(linieSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(linieSwitch);
      break;
    case S_INACTIVE:
      betriebsmodus = -1;
      ESPUI.getControl(manuellSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(manuellSwitch);
      break;
  }
#if(LOGGING)
  Serial.print(" ");
  Serial.println(sender->id);
#endif
}

void hindernisSwitchAction(Control *sender, int value) {
  switch (value) {
    case S_ACTIVE:
      hindernis = 1;
      ESPUI.getControl(hindernisSwitch)->color = ControlColor::Emerald;
      ESPUI.updateControl(hindernisSwitch);
      break;
    case S_INACTIVE:
      hindernis = 0;
      ESPUI.getControl(hindernisSwitch)->color = ControlColor::Alizarin;
      ESPUI.updateControl(hindernisSwitch);
      break;
  }
#if(LOGGING)
  Serial.print(" ");
  Serial.println(sender->id);
#endif
}

void padExample(Control *sender, int value) {
  if (betriebsmodus == 2)
    switch (value) {
      case P_LEFT_DOWN:
        fahrtrichtung = links;
#if(LOGGING)
        Serial.print("left down");
#endif
        break;

      case P_LEFT_UP:
        fahrtrichtung = halt;
#if(LOGGING)
        Serial.print("left up");
#endif
        break;

      case P_RIGHT_DOWN:
        fahrtrichtung = rechts;
#if(LOGGING)
        Serial.print("right down");
#endif
        break;

      case P_RIGHT_UP:
        fahrtrichtung = halt;
#if(LOGGING)
        Serial.print("right up");
#endif
        break;

      case P_FOR_DOWN:
        fahrtrichtung = vorwaerts;
#if(LOGGING)
        Serial.print("for down");
#endif
        break;

      case P_FOR_UP:
        fahrtrichtung = halt;
#if(LOGGING)
        Serial.print("for up");
#endif
        break;

      case P_BACK_DOWN:
        fahrtrichtung = halt;
#if(LOGGING)
        Serial.print("back down");
#endif
        break;

      case P_BACK_UP:
        fahrtrichtung = halt;
#if(LOGGING)
        Serial.print("back up");
#endif
        break;
    }

#if(LOGGING)
  Serial.print(" ");
  Serial.println(sender->id);
#endif
}

void linieSelectAction(Control *sender, int value) {
#if(LOGGING)
  Serial.print("Select: ID: ");
  Serial.print(sender->id);
  Serial.print(", Value: ");
  Serial.println(sender->value);
#endif

  if (sender->value == "linieHIGH") linienfarbe = HIGH;
  else if (sender->value == "linieLOW") linienfarbe = LOW;
}

void homepage() {
  homeTab = ESPUI.addControl(ControlType::Tab, "betrieb", "Betriebsmodus");
  lichtSwitch = ESPUI.addControl(ControlType::Switcher, "Licht folgen", "", ControlColor::Alizarin, homeTab, &lichtSwitchAction);
  linieSwitch = ESPUI.addControl(ControlType::Switcher, "Linie folgen", "", ControlColor::Alizarin, homeTab, &linieSwitchAction);
  manuellSwitch = ESPUI.addControl(ControlType::Switcher, "Manuelle Steuerung", "", ControlColor::Alizarin, homeTab, &manuellSwitchAction);
  hindernisSwitch = ESPUI.addControl(ControlType::Switcher, "Hindernissensor", "", ControlColor::Alizarin, homeTab, &hindernisSwitchAction);

  linieSelect = ESPUI.addControl(ControlType::Select, "Linie folgen:", "", ControlColor::Turquoise, homeTab, &linieSelectAction);
  ESPUI.addControl(ControlType::Option, "Schwarze Linie auf weiß", "linieHIGH", ControlColor::Turquoise, linieSelect); // linie = HIGH
  ESPUI.addControl(ControlType::Option, "Weiße Linie auf schwarz", "linieLOW", ControlColor::Turquoise, linieSelect); // linie = LOW
}

void liniePage() {

}

void lichtPage() {

}

void manuellPage() {
  manuellTab = ESPUI.addControl(ControlType::Tab, "manuell", "Manuelle Steuerung");
  manuellPad = ESPUI.addControl(ControlType::Pad, "Steuerung", "", ControlColor::Turquoise, manuellTab, &padExample);
}

void webserver_setup()  {
  //ESPUI SETUP
  ESPUI.setVerbosity(Verbosity::VerboseJSON);

#if defined(ESP32)
  WiFi.setHostname(hostname);
#else
  WiFi.hostname(hostname);
#endif

  // try to connect to existing network
  WiFi.begin(ssid, password);
#if(LOGGING)
  Serial.print("\n\nTry to connect to existing network");
#endif


  uint8_t timeout = 10;

  // Wait for connection, 5s timeout
  do {
    delay(500);
#if(LOGGING)
    Serial.print(".");
#endif
    timeout--;
  } while (timeout && WiFi.status() != WL_CONNECTED);

  // not connected -> create hotspot
  if (WiFi.status() != WL_CONNECTED) {
#if(LOGGING)
    Serial.print("\n\nCreating hotspot");
#endif
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid);

    timeout = 5;

    do {
      delay(500);
#if(LOGGING)
      Serial.print(".");
#endif
      timeout--;
    } while (timeout);
  }


  dnsServer.start(DNS_PORT, "*", apIP);

#if(LOGGING)
  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
#endif

  homepage();
  liniePage();
  lichtPage();
  manuellPage();

  ESPUI.begin("ESPUI Control");
}

/*
void webserver_loop() {
  dnsServer.processNextRequest();

  if (hindernis == 0) {
    ABSTAND_MESSEN = false;
    switch (betriebsmodus) {
//      case -1:
//        fahrtrichtung = halt;
//        break;
      case 0:
        LICHT_FOLGEN = true;
        LINIE_FOLGEN = false;
        break;
      case 1:
        LICHT_FOLGEN = false;
        LINIE_FOLGEN = true;
        break;
      case 2:
        //TODO manuelle Steuerung
        LICHT_FOLGEN = false;
        LINIE_FOLGEN = false;
        break;
      default:
        break;
    }
  } else {
    ABSTAND_MESSEN = true;
    switch (betriebsmodus) {
//      case -1:
//        fahrtrichtung = halt;
//        break;
      case 0:
        LICHT_FOLGEN = true;
        LINIE_FOLGEN = false;
        break;
      case 1:
        LICHT_FOLGEN = false;
        LINIE_FOLGEN = true;
        break;
      case 2:
        //TODO manuelle Steuerung
        LICHT_FOLGEN = false;
        LINIE_FOLGEN = false;
        break;
      default:
        break;
    }
  }
}
*/

void webserver_loop() {
  dnsServer.processNextRequest();

  if (hindernis == 0) {
    switch (betriebsmodus) {
      case -1:
        fahrtrichtung = halt;
        break;
      case 0:
        lichtFolgen();
        break;
      case 1:
        linieFolgen();
        break;
      case 2:
        //TODO manuelle Steuerung
        break;
      default:
        break;
    }
  } else {
   // abstandMessen();
   if( abstandMessen_anhalten()){
    hupen(BEEPER);
    fahrtrichtung = halt;
  }
  else{
    switch (betriebsmodus) {
      case -1:
        fahrtrichtung = halt;
        break;
      case 0:
        lichtFolgen();
        break;
      case 1:
        linieFolgen();
        break;
      case 2:
        //TODO manuelle Steuerung
        break;
      default:
        break;
    }
  }
  }
}
