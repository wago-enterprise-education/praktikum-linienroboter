#include "Arduino.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "AsyncJson.h"
#include "Webseite.h"

// drive modes
byte betriebsmodus = 0b00000100; //nulltes Bit: follow light ; erstes Bit: follow line  ;  zweites Bit: manual --> default 
byte manueller_modus_bit = 2;
byte linien_modus_bit = 1;
byte licht_modus_bit = 0;


const char* ap_ssid = "LinienRoboter";
const char* ap_password = "wagowago"; // a valid password must have more than 7 characters

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Die gesuchte Seite kann nicht gefunden werden");
}

byte ws_erhalte_betriebsmodus() {
  return betriebsmodus;
}

void modus_verarbeitung(AsyncWebServerRequest *request, byte mode_bit)
{
  bool state = bitRead(betriebsmodus, mode_bit);
  betriebsmodus = 0b0000;
  bitWrite(betriebsmodus, mode_bit, !state);
  fahrtrichtung = stopp;

  String str_value = String(state);
  String JSONMessageString = "{\"manual\": " + str_value + "}"; //TODO: es wird immer auf manual geschrieben
  char JSONMessage[JSONMessageString.length() + 1];
  JSONMessageString.toCharArray(JSONMessage, JSONMessageString.length() + 1);
  request->send_P(200, "application/json", JSONMessage);
}

void webserver_setup()
{
  // You can remove the password parameter if you want the AP to be open.
  Serial.println("Konfiguriere Access Point...");  
  if (!WiFi.softAP(ap_ssid)) {//, ap_password)) {
    log_e("Starten des Soft AP fehlgeschlagen.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Adresse: ");
  Serial.println(myIP);



  /***************** REST-API Endpoints******************/

  server.onNotFound(notFound);

  // Startseite 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", index_html);
  });

  // manuelle Steuerung
  server.on("/manuell", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    modus_verarbeitung(request, manueller_modus_bit);
  });

  // Linie folgen
  server.on("/linie", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    modus_verarbeitung(request, linien_modus_bit);
  });

  // Licht folgen
  server.on("/licht", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    modus_verarbeitung(request, licht_modus_bit);
  });

  // Steuerbefehl von App für Vorwärtsrichtung erhalten
  AsyncCallbackJsonWebHandler *handler_vorwaerts = new AsyncCallbackJsonWebHandler("/vorwaerts", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<50> data;

    if (json.is<JsonArray>())
    {
      data = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>())
    {
      data = json.as<JsonObject>();
    }

    if (data["vorwaerts"]) {
      fahrtrichtung = vorwaerts;
    } else {
      fahrtrichtung = stopp;
    }

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });
  server.addHandler(handler_vorwaerts);

  // Steuerbefehl von App für Rückwärtsrichtung erhalten ---
  // AsyncCallbackJsonWebHandler *handler_rueckwaerts = new AsyncCallbackJsonWebHandler("/rueckwaerts", [](AsyncWebServerRequest *request, JsonVariant &json) {
  //   StaticJsonDocument<50> data;

  //   if (json.is<JsonArray>())
  //   {
  //     data = json.as<JsonArray>();
  //   }
  //   else if (json.is<JsonObject>())
  //   {
  //     data = json.as<JsonObject>();
  //   }

  //   String response;
  //   serializeJson(data, response);
  //   request->send(200, "application/json", response);
  // });
  // server.addHandler(handler_rueckwaerts);

  // Steuerbefehl von App für Linksrichtung erhalten
  AsyncCallbackJsonWebHandler *handler_links = new AsyncCallbackJsonWebHandler("/links", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<50> data;

    if (json.is<JsonArray>()) {
      data = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>()) {
      data = json.as<JsonObject>();
    }

    if (data["links"]) {
      fahrtrichtung = links;
    } else {
      fahrtrichtung = stopp;
    }

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });
  server.addHandler(handler_links);

  // Steuerbefehl von App für Rechtsrichtung erhalten
  AsyncCallbackJsonWebHandler *handler_rechts = new AsyncCallbackJsonWebHandler("/rechts", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<50> data;

    if (json.is<JsonArray>()) {
      data = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>()) {
      data = json.as<JsonObject>();
    }

    if (data["rechts"]) {
      fahrtrichtung = rechts;
    } else {
      fahrtrichtung = stopp;
    }

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });
  server.addHandler(handler_rechts);

  // Steuerbefehl von App für Hupe erhalten
  AsyncCallbackJsonWebHandler *handler_hupe = new AsyncCallbackJsonWebHandler("/hupe", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<50> data;

    if (json.is<JsonArray>()) {
      data = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>()) {
      data = json.as<JsonObject>();
    }
    if (data["hupe"]) {
      hupe(BEEPER);
    }

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });
  server.addHandler(handler_hupe);

  server.begin();
}