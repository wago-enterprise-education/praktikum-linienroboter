//Einbinden der Biblotheken
#include <WiFi.h>
//#include <HSCR04.h>
//Definieren der Ausgänge
#define BlinkerL 13
#define BlinkerR 14

#define FotowiderstandL 33
#define FotowiderstandR 32

#define LiniensensorL 18
#define LiniensensorR 19

#define AbstandsmesserTrigger 27
#define AbstandsmesserSensor 26

#define MotorL 22
#define MotorR 23

#define Beeper 4

TaskHandle_t Task1;
TaskHandle_t Task2;

//Variablen Deklarieren
String header;
String Linienfolgen = "Aus";
String Lichtfolgen = "Aus";
String ManuellBetrieb = "Aus";
String MotorSteuerungL = "Aus";
String MotorSteuerungR = "Aus";

int fall = 0;
int abstand;

byte WertSpeed = 180;
byte WertGerade = 90;
byte WertKurve = 13;

bool LiniensensorWertL;
bool LiniensensorWertR;

int SchwellenwertLicht = 3400;
int LichtwiderstandWertL;
int LichtwiderstandWertR;

String boost = "Aus";

//Abstandssensor erstellen
HCSR04 hcsr04(AbstandsmesserTrigger, AbstandsmesserSensor, 20, 4000);

//Netzwerk Daten
const char* ssid = "Roboter Konrad";
const char* passwort = "1235";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  //Pin Modes Setzen
  pinMode(BlinkerL, OUTPUT);
  pinMode(BlinkerR, OUTPUT);

  pinMode(FotowiderstandL, INPUT);
  pinMode(FotowiderstandR, INPUT);

  pinMode(LiniensensorL, INPUT);
  pinMode(LiniensensorR, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  //Motoren konfigurieren
  ledcAttachPin(MotorL, 1);
  ledcSetup(1, 4000, 8);

  ledcAttachPin(MotorR, 2);
  ledcSetup(2, 4000, 8);

  ledcAttachPin(Beeper, 3);
  ledcSetup(3, 400, 8);

  //Access Point starten
  WiFi.softAP(ssid, passwort);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);
  server.begin();

  xTaskCreatePinnedToCore(
    webserver,   /* Task function. */
    "Server",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  xTaskCreatePinnedToCore(
    switchTask,   /* Task function. */
    "Switch",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */
  delay(500);
}
void loop() {

}

void switchTask(void * pvParameters) {
  Serial.print("SwitchTask: ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    switch (fall) {
      case 0:
        digitalWrite(LED_BUILTIN, HIGH);
        ledcWrite(1, 0);
        ledcWrite(2, 0);
        digitalWrite(BlinkerL, LOW);
        digitalWrite(BlinkerR, LOW);
        break;

      case 1:
        digitalWrite(LED_BUILTIN, LOW);
        Linie();
        break;

      case 2:
        digitalWrite(LED_BUILTIN, LOW);
        Licht();
        break;

      case 3:
        digitalWrite(LED_BUILTIN, LOW);
        Manuell();
        break;
    }
    delay(1);
  }
}

void Linie() {
  LiniensensorWertL = digitalRead(LiniensensorL);
  LiniensensorWertR = digitalRead(LiniensensorR);
  if (LiniensensorWertL && LiniensensorWertR) {
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    digitalWrite(BlinkerL, HIGH);
    digitalWrite(BlinkerR, HIGH);
    ledcWrite(3, 255);
  }
  else if (LiniensensorWertL && !LiniensensorWertR) {
    ledcWrite(1, WertKurve);
    ledcWrite(2, WertGerade);
    digitalWrite(BlinkerL, HIGH);
    digitalWrite(BlinkerR, LOW);
    ledcWrite(3, 0);
  }
  else if (!LiniensensorWertL && LiniensensorWertR) {
    ledcWrite(1, WertGerade);
    ledcWrite(2, WertKurve);
    digitalWrite(BlinkerL, LOW);
    digitalWrite(BlinkerR, HIGH);
    ledcWrite(3, 0);
  }
  else {
    if (boost == "An") {
      ledcWrite(1, WertSpeed + 10);
      ledcWrite(2, WertSpeed);
    } else {
      ledcWrite(1, WertGerade + 10);
      ledcWrite(2, WertGerade);
    }
    digitalWrite(BlinkerL, LOW);
    digitalWrite(BlinkerR, LOW);
    ledcWrite(3, 0);
  }
}

void Licht() {
  LichtwiderstandWertL = analogRead(FotowiderstandL);
  LichtwiderstandWertR = analogRead(FotowiderstandR);
  if (LichtwiderstandWertL < SchwellenwertLicht && LichtwiderstandWertR < SchwellenwertLicht) {
    ledcWrite(1, WertGerade + 10);
    ledcWrite(2, WertGerade);
  }
  else if (LichtwiderstandWertL < SchwellenwertLicht && LichtwiderstandWertR > SchwellenwertLicht) {
    ledcWrite(1, WertKurve);
    ledcWrite(2, WertGerade);
  }
  else if (LichtwiderstandWertL > SchwellenwertLicht && LichtwiderstandWertR < SchwellenwertLicht) {
    ledcWrite(1, WertGerade);
    ledcWrite(2, WertKurve);
  }
  else {
    ledcWrite(1, 0);
    ledcWrite(2, 0);
  }
}

void Manuell() {
  if ( MotorSteuerungL == "An" && MotorSteuerungR == "An") {
    ledcWrite(1, WertGerade + 10);
    ledcWrite(2, WertGerade);
  } else if ( MotorSteuerungL == "An" && MotorSteuerungR == "Aus") {
    ledcWrite(1, WertGerade);
    ledcWrite(2, WertKurve);
  } else if ( MotorSteuerungL == "Aus" && MotorSteuerungR == "An") {
    ledcWrite(1, WertKurve);
    ledcWrite(2, WertGerade);
  } else {
    ledcWrite(1, 0);
    ledcWrite(2, 0);
  }
}

void webserver(void * pvParameters) {
  Serial.print("WebServer: ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    WiFiClient client = server.available();
    if (client) {
      String daten = "";
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          header += c;
          if (c == '\n') {
            if (daten.length() == 0) {

              //HTML Befehl beginn?
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if (header.indexOf("GET /1/on") >= 0) {
                Linienfolgen = "An";
                Lichtfolgen = "Aus";
                ManuellBetrieb = "Aus";
                fall = 1;
              } else if (header.indexOf("GET /1/off") >= 0) {
                Linienfolgen = "Aus";
                Lichtfolgen = "Aus";
                ManuellBetrieb = "Aus";
                fall = 0;
              } else if (header.indexOf("GET /2/on") >= 0) {
                Linienfolgen = "Aus";
                Lichtfolgen = "An";
                ManuellBetrieb = "Aus";
                fall = 2;
              } else if (header.indexOf("GET /2/off") >= 0) {
                Linienfolgen = "Aus";
                Lichtfolgen = "Aus";
                ManuellBetrieb = "Aus";
                fall = 0;
              } else if (header.indexOf("GET /3/on") >= 0) {
                Linienfolgen = "Aus";
                Lichtfolgen = "Aus";
                ManuellBetrieb = "An";
                fall = 3;
              } else if (header.indexOf("GET /3/off") >= 0) {
                Linienfolgen = "Aus";
                Lichtfolgen = "Aus";
                ManuellBetrieb = "Aus";
                fall = 0;
              } else if (header.indexOf("GET /ML/on") >= 0) {
                MotorSteuerungL = "An";
              } else if (header.indexOf("GET /ML/off") >= 0) {
                MotorSteuerungL = "Aus";
              } else if (header.indexOf("GET /MR/on") >= 0) {
                MotorSteuerungR = "An";
              } else if (header.indexOf("GET /MR/off") >= 0) {
                MotorSteuerungR = "Aus";
              } else if (header.indexOf("GET /B/on") >= 0) {
                boost = "An";
              } else if (header.indexOf("GET /B/off") >= 0) {
                boost = "Aus";
              }

              //Anzeige der Website
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");

              //CSS
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #555555;}</style></head>");

              //Titelzeile
              client.println("<body><h1>Robotersteuerung</h1>");

              //Anzeige Status Linienfolge
              client.println("<p>Linienfolgen Status:  " + Linienfolgen + "</p>");
              //Anzeige Knopf Liniefolgen
              if (Linienfolgen == "Aus") {
                client.println("<p><a href=\"/1/on\"><button class=\"button\">An</button></a></p>");
              } else {
                client.println("<p><a href=\"/1/off\"><button class=\"button button2\">Aus</button></a></p>");
              }

              //Anzeige Status Linienfolge
              client.println("<p>Lichtfolgen Status:  " + Lichtfolgen + "</p>");
              //Anzeige Knopf Liniefolgen
              if (Lichtfolgen == "Aus") {
                client.println("<p><a href=\"/2/on\"><button class=\"button\">An</button></a></p>");
              } else {
                client.println("<p><a href=\"/2/off\"><button class=\"button button2\">Aus</button></a></p>");
              }

              //Anzeige Status Manueller Betrieb
              client.println("<p>Manuellbetrieb Status:  " + ManuellBetrieb + "</p>");
              //Anzeige Knopf ManuellerBetrieb
              if (ManuellBetrieb == "Aus") {
                client.println("<p><a href=\"/3/on\"><button class=\"button\">An</button></a></p>");
              } else {
                client.println("<p><a href=\"/3/off\"><button class=\"button button2\">Aus</button></a></p>");
              }

              //Anzeige Status Motor Links
              client.println("<p>Links Status:  " + MotorSteuerungL + " Rechts Status: " + MotorSteuerungR + "</p>");
              //Anzeige Knopf ManuellerBetrieb
              if (MotorSteuerungL == "Aus") {
                client.println("<p><a href=\"/ML/on\"><button class=\"button\">An</button></a>");
              } else {
                client.println("<p><a href=\"/ML/off\"><button class=\"button button2\">Aus</button></a>");
              }

              //Anzeige Knopf ManuellerBetrieb
              if (MotorSteuerungR == "Aus") {
                client.println("<a href=\"/MR/on\"><button class=\"button\">An</button></a></p>");
              } else {
                client.println("<a href=\"/MR/off\"><button class=\"button button2\">Aus</button></a></p>");
              }

              //Anzeige Status Manueller Betrieb
              client.println("<p>Boost Status:  " + boost + "</p>");
              //Anzeige Knopf ManuellerBetrieb
              if (boost == "Aus") {
                client.println("<p><a href=\"/B/on\"><button class=\"button\">An</button></a></p>");
              } else {
                client.println("<p><a href=\"/B/off\"><button class=\"button button2\">Aus</button></a></p>");
              }

              client.println("</body></html>");

              //Ende HTTP Antwort
              client.println();
              break;
            }
            else {
              daten = "";
            }
          }
          else if (c != '\r') {
            daten += c;
          }
        }
      }
      header = "";
      // Schließen Verbindung
      client.stop();
    }
    delay(1);
  }
}
