//#define LOGGING (false)
//---------------------------------------------------------------------------------
////--------------------------------- PIN BELEGUNG --------------------------------
//---------------------------------------------------------------------------------
#define ABSTAND_TRIGGER 27
#define ABSTAND_ECHO 26

#define BEEPER 4

#define LED_LINKS 13
#define LED_RECHTS 14

#define LICHT_LINKS 33
#define LICHT_RECHTS 32

#define LINIE_LINKS 18
#define LINIE_RECHTS 19

#define MOTOR_LINKS 22
#define MOTOR_RECHTS 23

// SETZEN

bool LINIE_FOLGEN  = false;
bool LICHT_FOLGEN  = false;
bool ABSTAND_MESSEN = false;

byte linienfarbe;                                                             // High fuer Schwarz-auf-weiss / Low fuer weiss-auf-schwarz
// bool start = false;

//---------------------------------------------------------------------------------
////---------------------------------- VARIABLEN ----------------------------------
//---------------------------------------------------------------------------------

// Konstanten fuer Abstandssensor
const int maxRange = 300;                                                           // maximal messbarer Abstand
const int minRange = 2;                                                             // minimal messbarer Abstand
const int abstand_grenze = 15;                                                      // Abstand bei dem gestoppt werden soll

// Variablen fuer Abstandssensor
long abstand;                                                                       // Variable fuer gemessenen Abstands
long dauer;                                                                         // Variable fuer Zeitdauer der Messung
int abstand_loop = 0;                                                               // Counter fuer Schleifendurchlaeufe, Mittelwert aus 10 Messungen wird verwendet
int abstand_summe = 0;                                                              // Variable fuer insgesamt in 10 Messungen ermittelten Wert


// Konstanten fuer Lichtsensoren
const int messtoleranz = 50;                                                       // Messwert-Abweichung zw. Sensoren ca. 150-250
const int prozent_einstrahlung = 110;                                               // bei zusaetzlicher Einstrahlung (z.B. Taschenlampe) gemessener Wert ca. 25% hoeher als normal
//const int prozent_einstrahlung = 125;                                             // bei zusaetzlicher Einstrahlung gemessener Wert ca. 25% hoeher als normal
//const int prozent_einstrahlung = 125;                                             // bei zusaetzlicher Einstrahlung gemessener Wert ca. 25% hoeher als normal

// Variablen fuer Lichtsensoren
int lichtWert_durchschnitt;                                                         // durchschnittlich gemessener Helligkeitswert fuer Vergleich mit Umgebungshelligkeit
int lichtWert_links_summe = 0;                                                      // insgesamt bei bisherigen Schleifendurchläufen gemessener Wert, links
int lichtWert_rechts_summe = 0;                                                     // insgesamt bei bisherigen Schleifendurchläufen gemessener Wert, rechts
int loop_count = 0;                                                                 // Counter fuer Schleifendurchlaeufe, Mittelwert aus 10 Messungen wird verwendet
int lichtWert_links = 1;                                                            // aktuell gemessener Helligkeits-Wert rechts, auf 1 gesetzt um durch 0 teilen zu vermeiden
int lichtWert_rechts = 1;                                                           // aktuell gemessener Helligkeits-Wert links, auf 1 gesetzt um durch 0 teilen zu vermeiden
int lichtWert_umgebung = 1;                                                         // Umgebungshelligkeit, auf 1 gesetzt um durch 0 teilen zu vermeiden


// Konstanten fuer Motor-PWM
const int freq = 15000;                                                             // PWM Frequenz
const int motorChannel_links = 0;                                                   // PWM Kanal fuer linken Motor
const int motorChannel_rechts = 2;                                                  // PWM Kanal fuer rechten Motor
const int resolution = 9;                                                           // PWM-Aufloesung
const int maxGeschwindigkeit = (int) (pow(2, resolution) - 1);                      // max. Wert fuer PWM ist 2^(aufloesung) -1

// todo j: zurückändern:
//int geschwindigkeit_anfahren = 0.5 * maxGeschwindigkeit;
//int geschwindigkeit_fahren = 0.4 * maxGeschwindigkeit;

//int geschwindigkeit_anfahren = 0.7 * maxGeschwindigkeit;
//int geschwindigkeit_fahren = 0.6 * maxGeschwindigkeit;
//int geschwindigkeit_drehen_innen = 0.45 * maxGeschwindigkeit;
//int geschwindigkeit_drehen_aussen = 0.85 * maxGeschwindigkeit;    //0,65

int geschwindigkeit_anfahren = 0.6 * maxGeschwindigkeit;
int geschwindigkeit_fahren = 0.45 * maxGeschwindigkeit;
int geschwindigkeit_drehen_innen = 0.34 * maxGeschwindigkeit;
int geschwindigkeit_drehen_aussen = 0.75 * maxGeschwindigkeit;    //0,65
int geschwindigkeit_stoppen = 0;


// Konstanten zum Blinken der LEDs
const long BLINK_INTERVAL = 500;                                                    // Zeitinterval (Millisekunden) in dem LEDs blinken sollen

// Variablen zum Blinken der LEDs
int ledState = LOW;                                                                 // Status der LED zum Blinken (wechselt zw. high und low)
unsigned long previousMillis_led = 0;                                               // letzter Zeitpunkt zu dem LED gesetzt wurde


// Konstanten fuer Beeper-PWM
const int beeperFreq = 1000;                                                        // Ton Frequenz für Ton C
const int beeperChannel = 4;                                                        // PWM Kanal fuer Beeper
const int beeperResolution = 4;                                                     // PWM-Aufloesung
const int beeper_off = 0;
const int beeper_on = 0.5 * (pow(2, beeperResolution) - 1);

// Konstanten zum Hupen
const long BEEPER_INTERVAL = 1000;                                                  // Zeitinterval (Millisekunden) in dem Beeper ein- und ausgeschaltet werden soll

// Variablen zum Hupen
int pinState = LOW;                                                                 // Status des Pins zum Hupen (wechselt zw. high und low)
unsigned long previousMillis_beeper = 0;                                            // letzter Zeitpunkt zu dem Beeper gesetzt wurde


// Variablen fuer Programmablauf
fahrtrichtung_t fahrtrichtung = halt;


//---------------------------------------------------------------------------------
////------------------------------------ SETUP ------------------------------------
//---------------------------------------------------------------------------------
void roboter_setup()
{
  Serial.println("ROBOTER SETUP");

  pinMode(LED_LINKS, OUTPUT);
  pinMode(LED_RECHTS, OUTPUT);

  //#if(ABSTAND_MESSEN)
  // SETUP ABSTANDSSENSOR
  pinMode(ABSTAND_TRIGGER, OUTPUT);
  pinMode(ABSTAND_ECHO, INPUT);

  // pinMode(BEEPER, OUTPUT);
  ledcSetup(beeperChannel, beeperFreq, beeperResolution);
  ledcAttachPin(BEEPER, beeperChannel);

  //#endif

  //#if(LICHT_FOLGEN)
  // SETUP LICHTSENSOREN                                                             // Bestimmen Umgebungshelligkeit als Mittelwert der gemessenen Helligkeit
  lichtWert_links = analogRead(LICHT_LINKS);
  lichtWert_rechts = analogRead(LICHT_RECHTS);
  lichtWert_umgebung = (lichtWert_links + lichtWert_rechts) / 2;
  //#endif

  //#if(LINIE_FOLGEN)
  // SETUP LINIENSENSOREN
  pinMode(LINIE_LINKS, INPUT);
  pinMode(LINIE_RECHTS, INPUT);
  //#endif

  // SETUP MOTOREN (PWM)
  ledcSetup(motorChannel_links, freq, resolution);
  ledcAttachPin(MOTOR_LINKS, motorChannel_links);

  ledcSetup(motorChannel_rechts, freq, resolution);
  ledcAttachPin(MOTOR_RECHTS, motorChannel_rechts);
}


//---------------------------------------------------------------------------------
////------------------------------ BASIS-FUNKTIONEN -------------------------------
//---------------------------------------------------------------------------------

void blinken(int led_pin)
{
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis_led = millis();

  if (currentMillis_led - previousMillis_led >= BLINK_INTERVAL) {
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(led_pin, ledState);

    // save the last time you blinked the LED
    previousMillis_led = currentMillis_led;
  }
}

void hupen(int pin)
{
  // check to see if it's time to toggle the pin; that is, if the difference
  // between the current time and last time you toggled is bigger than
  // the interval at which you want to change the pin's state.
  unsigned long currentMillis_beeper = millis();

  if (currentMillis_beeper - previousMillis_beeper >= BEEPER_INTERVAL) {
    // if the LED is off turn it on and vice-versa:
    if (pinState == LOW) {
      pinState = HIGH;
      ledcWrite(beeperChannel, beeper_on);
    } else {
      pinState = LOW;
      ledcWrite(beeperChannel, beeper_off);
    }

    // save the last time you blinked the LED
    previousMillis_beeper = currentMillis_beeper;
  }
}

//---------------------------------------------------------------------------------
////----------------------------- FUNKTIONEN: FAHREN ------------------------------
//---------------------------------------------------------------------------------
void geradeaus(int geschwindigkeit)
{
  ledcWrite(motorChannel_links, geschwindigkeit);
  ledcWrite(motorChannel_rechts, geschwindigkeit);
#if(LOGGING)
  Serial.println("Fahren: Geradeaus");
#endif
}

void nachLinks()
{
  ledcWrite(motorChannel_links, geschwindigkeit_drehen_innen);
  ledcWrite(motorChannel_rechts, geschwindigkeit_drehen_aussen);

#if(LINIE_FOLGEN)
  blinken(LED_LINKS);
#endif

#if(LOGGING)
  Serial.println("Fahren: Nach Links");
#endif
}

void nachRechts()
{
  ledcWrite(motorChannel_links, geschwindigkeit_drehen_aussen);
  ledcWrite(motorChannel_rechts, geschwindigkeit_drehen_innen);

#if(LINIE_FOLGEN)
  blinken(LED_RECHTS);
#endif

#if(LOGGING)
  Serial.println("Fahren: Nach Rechts");
#endif
}

void stoppen()
{
  ledcWrite(motorChannel_links, geschwindigkeit_stoppen);
  ledcWrite(motorChannel_rechts, geschwindigkeit_stoppen);

 // hupen(BEEPER);

#if(LOGGING)
  Serial.println("Fahren: Stoppen");
#endif
}

//---------------------------------------------------------------------------------
////---------------------------- FUNKTIONEN: SENSOREN -----------------------------
//---------------------------------------------------------------------------------

////---------------------------------- ABSTAND ------------------------------------
bool abstandMessen_anhalten()
{

  int loop_anzahl = 1;
  abstand_summe = 0;
  abstand_loop = 0;

  while (abstand_loop < loop_anzahl)
  {

#if(LOGGING)
    Serial.println("Sensor: Abstand");
#endif

    //Abstandsmessung durch Trigger (10µs) gestartet
    digitalWrite(ABSTAND_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(ABSTAND_TRIGGER, LOW);

    //Messung am Input-Pin: wie lange bleibt Signal aktiviert
    dauer = pulseIn(ABSTAND_ECHO, HIGH);

    //Abstand aus aufgenommener Zeit berechnen
    abstand_summe += (dauer / 58.2);
    abstand_loop ++;
  }

  if (abstand_loop == loop_anzahl)
  {
    abstand = abstand_summe / loop_anzahl;

#if(LOGGING)
    //Ueberpruefung ob Abstand innerhalb zulässiger Entfernung
    if (abstand >= maxRange || abstand <= minRange) {
      Serial.println("Abstand ausserhalb des Messbereichs");
      Serial.println("-----------------------------------");
      Serial.print("Der Abstand betraegt: ");
      Serial.print(abstand);
      Serial.println("cm");
    }
    else {
      Serial.print("Der Abstand betraegt: ");
      Serial.print(abstand);
      Serial.println("cm");
      Serial.println("-----------------------------------");
    }
#endif

    if (abstand < abstand_grenze)
    {
#if(LOGGING)
      Serial.println("Abstand zu gering: Stoppen");
#endif
      //fahrtrichtung = halt;

      return true;
    }
  }

  // return fahrtrichtung;
  return false;
}

////----------------------------------- LICHT -------------------------------------
fahrtrichtung_t lichtFolgen()
{
  int licht_loop = 5;
  loop_count = 0;
  lichtWert_links_summe = 0;
  lichtWert_rechts_summe = 0;
#if(LOGGING)
  Serial.println("Sensor: Licht");
#endif

  // Helligkeit fuer 10 Zyklen messen
  while (loop_count < licht_loop)
  {
    lichtWert_links_summe = lichtWert_links_summe + analogRead(LICHT_LINKS);
    lichtWert_rechts_summe = lichtWert_rechts_summe + analogRead(LICHT_RECHTS);

    loop_count++;
  }

  // nach 10tem Zyklus Mittelwert der Messwerte bilden und Werte vergleichen
  if (loop_count == licht_loop)
  {
    lichtWert_links = lichtWert_links_summe / licht_loop;
    lichtWert_rechts = lichtWert_rechts_summe / licht_loop;

#if(LOGGING)
    Serial.print("Links: ");
    Serial.println(lichtWert_links, DEC);
    Serial.print("Rechts: ");
    Serial.println(lichtWert_rechts, DEC);
#endif

    lichtWert_durchschnitt = (lichtWert_links + lichtWert_rechts) / 2;

#if(LOGGING)
    Serial.print("Durchschnitt: ");
    Serial.println(lichtWert_durchschnitt, DEC);
    Serial.print("Umgebung: ");
    Serial.println(lichtWert_umgebung, DEC);
#endif

    // falls Setup fehlgeschlagen - erster Messwert ist Umgebungshelligkeit
    if (lichtWert_umgebung == 1)
    {
      lichtWert_umgebung = lichtWert_durchschnitt;
#if(LOGGING)
      Serial.print("Umgebung: ");
      Serial.println(lichtWert_umgebung, DEC);
      Serial.println("-----------------------------------");
#endif
    }

    // wenn Helligkeit groesser, steigt Wert des Analog-Inputs
    // beide Sensoren messen leicht voneinander abweichende Werte in gleicher Helligkeit - Abfangen mit Messtoleranz
    if (abs(lichtWert_links - lichtWert_rechts) < messtoleranz)
    {
      // beide Sensoren messen aehnliche Werte - keine Einstrahlung oder Taschenlampe von vorne

      if ((lichtWert_durchschnitt / lichtWert_umgebung) * 100 >= prozent_einstrahlung)
      {
        // normale Umgebungshelligkeit: Schalten wenn gemessene Werte mehr als x% groesser als Umgebungshelligkeit
#if(LOGGING)
        Serial.print("Geradeaus: ");
        Serial.println((lichtWert_durchschnitt / lichtWert_umgebung) * 100 );
#endif
        fahrtrichtung = vorwaerts;
      }
      else
      {
#if(LOGGING)
        Serial.println("Halt");
#endif
        fahrtrichtung = halt;
      }
    }
    // bei zusaetzlicher Einstrahlung (durch Taschenlampe) ist Messwert 25% hoeher als normal
    else if ((lichtWert_rechts / lichtWert_links) * 100 >= prozent_einstrahlung)
    {
#if(LOGGING)
      Serial.println("Rechts > Links -> nach Rechts fahren");
#endif
      // fahrtrichtung = rechts;
      fahrtrichtung = links;
    }
    else if ((lichtWert_links / lichtWert_rechts) * 100  >= prozent_einstrahlung)
    {
#if(LOGGING)
      Serial.println("Links > Rechts -> nach Links fahren");
#endif
      // fahrtrichtung = links;
      fahrtrichtung = rechts;
    }

    //    loop_count = 0;
    //    lichtWert_links_summe = 0;
    //    lichtWert_rechts_summe = 0;

  }
  return fahrtrichtung;
}


////----------------------------------- LINIE -------------------------------------
fahrtrichtung_t linieFolgen()
{
  // Variablen fuer Liniensensoren
  byte val_links;
  byte val_rechts;

  val_links = digitalRead(LINIE_LINKS);
  val_rechts = digitalRead(LINIE_RECHTS);

#if(LOGGING)
  Serial.println("Sensor: Linie");
  Serial.print("Links: ");
  Serial.print(val_links);
  Serial.print(" Rechts: ");
  Serial.println(val_rechts);
#endif

  // Schwarz erkannt = High, Weiß erkannt = Low
  // Fahrzeug ist auf Linie, wenn beide Sensoren keine Linie erkennen
  if ((val_links != linienfarbe) && (val_rechts != linienfarbe))
  {
#if(LOGGING)
    Serial.println("geradeaus");
#endif
    fahrtrichtung = vorwaerts;
  }
  // wenn Fahrzeug zu weit rechts faehrt, wird Links Linie erkannt -> mit rechtem Motor nachsteuern
  else if ((val_links == linienfarbe) && (val_rechts != linienfarbe))
  {
#if(LOGGING)
    Serial.println("nach links");
#endif
    fahrtrichtung = links;
  }
  // analog nach links
  else if ((val_links != linienfarbe) && (val_rechts == linienfarbe))
  {
#if(LOGGING)
    Serial.println("nach rechts");
#endif
    fahrtrichtung = rechts;
  }
  else
  {
#if(LOGGING)
    Serial.println("stoppen");
#endif
    fahrtrichtung = halt;
  }
  return fahrtrichtung;
}


//---------------------------------------------------------------------------------
////------------------------------------ LOOP -------------------------------------
//---------------------------------------------------------------------------------

void roboter_loop()
{
  //if(!betriebsmodus != 2){
  //  fahrtrichtung = halt;
  //}
  /*
    //--------------------- LINIE ERKENNEN ---------------------
    if(LINIE_FOLGEN){
    fahrtrichtung = linieFolgen();
    }

    //--------------------- LICHT ERKENNEN ---------------------
    if(LICHT_FOLGEN){
    fahrtrichtung = lichtFolgen();
    }

    //--------------------- ABSTAND MESSEN ---------------------
    if(ABSTAND_MESSEN){
    if( abstandMessen_anhalten()){
      fahrtrichtung = halt;
    }
    }
  */
  /*
    // Testpfad:
    geradeaus(geschwindigkeit_anfahren);
    delay(200);
    geradeaus(geschwindigkeit_fahren);
    delay(2000);
    nachLinks(geschwindigkeit_fahren);
    delay(3000);
    nachRechts(geschwindigkeit_fahren);
    delay(3000);
    geradeaus(geschwindigkeit_fahren);
    delay(2000);
    stoppen();
    delay(10000);
  */

  switch (fahrtrichtung)
  {
    case vorwaerts:
      //      if (start == true) {
      //
      //        geradeaus(geschwindigkeit_anfahren);
      //        start = false;
      //      }
      //      else
      //      {
#if(LOGGING)
      Serial.println("Fahrtrichtung: Geradeaus");
#endif
      geradeaus(geschwindigkeit_fahren);
      //      }
      break;

    case links:
#if(LOGGING)
      Serial.println("Fahrtrichtung: nach Links");
#endif
      nachLinks();
      break;

    case rechts:
#if(LOGGING)
      Serial.println("Fahrtrichtung: nach Rechts");
#endif
      nachRechts();
      break;

    case halt:
#if(LOGGING)
      Serial.println("Fahrtrichtung: Stoppen");
#endif
      // hupen(BEEPER);
      stoppen();
      break;
  }
}
