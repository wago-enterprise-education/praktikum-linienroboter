#define LOGGING (false)
//---------------------------------------------------------------------------------
////--------------------------------- PIN BELEGUNG --------------------------------
//---------------------------------------------------------------------------------
#define ABSTAND_TRIGGER 27
#define ABSTAND_ECHO 26

#define BEEPER 4

#define LED_links 13
#define LED_rechts 14

#define LICHT_links 33
#define LICHT_rechts 32

#define LINIE_links 18
#define LINIE_rechts 19

#define MOTOR_links 22
#define MOTOR_rechts 23


typedef enum fahrtrichtung {
  stopp,
  vorwaerts, 
  links,
  rechts,
}fahrtrichtung_t;


bool FOLGE_LINIE  = false;
bool FOLGE_LICHT  = false;
bool ABSTAND_MESSEN = false;

byte linienfarbe;                                                             // High fuer Schwarz-auf-weiss / Low fuer weiss-auf-schwarz
// bool start = false;

//---------------------------------------------------------------------------------
////---------------------------------- VARIABLEN ----------------------------------
//---------------------------------------------------------------------------------

// Konstanten fuer Abstandssensor
const int maxRange = 300;                                                           // maximal messbarer Abstand
const int minRange = 2;                                                             // minimal messbarer Abstand
const int abstand_grenze = 15;                                                      // Abstand bei dem gestopppt werden soll

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
const int max_velocity = (int) (pow(2, resolution) - 1);                      // max. Wert fuer PWM ist 2^(aufloesung) -1

int velocity_driveoff = 0.6 * max_velocity;
int velocity_drive = 0.45 * max_velocity;
int velocity_turn_inside = 0.34 * max_velocity;
int velocity_turn_outside = 0.75 * max_velocity;    //0,65
int velocity_stopp = 0;


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
const long BEEPER_INTERVAL = 10;                                                  // Zeitinterval (Millisekunden) in dem Beeper ein- und ausgeschaltet werden soll

// Variablen zum Hupen
int pinState = LOW;                                                                 // Status des Pins zum Hupen (wechselt zw. high und low)
unsigned long previousMillis_beeper = 0;                                            // letzter Zeitpunkt zu dem Beeper gesetzt wurde


// Variablen fuer Programmablauf
fahrtrichtung_t fahrtrichtung = stopp;


//---------------------------------------------------------------------------------
////------------------------------------ SETUP ------------------------------------
//---------------------------------------------------------------------------------
void robot_setup()
{
  Serial.println("ROBOTER SETUP");

  pinMode(LED_links, OUTPUT);
  pinMode(LED_rechts, OUTPUT);

  // SETUP ABSTANDSSENSOR
  pinMode(ABSTAND_TRIGGER, OUTPUT);
  pinMode(ABSTAND_ECHO, INPUT);

  // pinMode(BEEPER, OUTPUT);
  ledcSetup(beeperChannel, beeperFreq, beeperResolution);
  ledcAttachPin(BEEPER, beeperChannel);


  // SETUP LIGHTSENSOREN                                                             // Bestimmen Umgebungshelligkeit als Mittelwert der gemessenen Helligkeit
  lichtWert_links = analogRead(LICHT_links);
  lichtWert_rechts = analogRead(LICHT_rechts);
  lichtWert_umgebung = (lichtWert_links + lichtWert_rechts) / 2;

  // SETUP LINENSENSOREN
  pinMode(LINIE_links, INPUT);
  pinMode(LINIE_rechts, INPUT);

  // SETUP MOTOREN (PWM)
  ledcSetup(motorChannel_links, freq, resolution);
  ledcAttachPin(MOTOR_links, motorChannel_links);

  ledcSetup(motorChannel_rechts, freq, resolution);
  ledcAttachPin(MOTOR_rechts, motorChannel_rechts);
}


//---------------------------------------------------------------------------------
////------------------------------ BASIS-FUNKTIONEN -------------------------------
//---------------------------------------------------------------------------------

void blinken(int led_pin)
{
  // check to see if it's time to blinken the LED; that is, if the difference
  // between the current time and last time you blinkened the LED is bigger than
  // the interval at which you want to blinken the LED.
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

    // save the last time you blinkened the LED
    previousMillis_led = currentMillis_led;
  }
}

void hupe(int pin)
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

    // save the last time you blinkened the LED
    previousMillis_beeper = currentMillis_beeper;
  }
}

//---------------------------------------------------------------------------------
////----------------------------- FUNKTIONEN: FAHREN ------------------------------
//---------------------------------------------------------------------------------
void fahre_geradeaus(int geschwindigkeit)
{
  ledcWrite(motorChannel_links, geschwindigkeit);
  ledcWrite(motorChannel_rechts, geschwindigkeit);

}

void fahre_links()
{
  ledcWrite(motorChannel_links, velocity_turn_inside);
  ledcWrite(motorChannel_rechts, velocity_turn_outside);

#if(FOLGE_LINIE)
  blinken(LED_links);
#endif

}

void fahre_rechts()
{
  ledcWrite(motorChannel_links, velocity_turn_outside);
  ledcWrite(motorChannel_rechts, velocity_turn_inside);

#if(FOLGE_LINIE)
  blinken(LED_rechts);
#endif

}

void anhalten()
{
  ledcWrite(motorChannel_links, velocity_stopp);
  ledcWrite(motorChannel_rechts, velocity_stopp);
}

//---------------------------------------------------------------------------------
////---------------------------- FUNKTIONEN: SENSOREN -----------------------------
//---------------------------------------------------------------------------------

////---------------------------------- ABSTAND ------------------------------------
// TODO integrieren
bool abstandMessen_anhalten()
{

  int loop_anzahl = 1;
  abstand_summe = 0;
  abstand_loop = 0;

  while (abstand_loop < loop_anzahl)
  {
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

    if (abstand < abstand_grenze)
    {
      //fahrtrichtung = stopp;

      return true;
    }
  }

  // return fahrtrichtung;
  return false;
}

////----------------------------------- LIGHT -------------------------------------
fahrtrichtung_t licht_folgen()
{
  int licht_loop = 5;
  loop_count = 0;
  lichtWert_links_summe = 0;
  lichtWert_rechts_summe = 0;

  // Helligkeit fuer 10 Zyklen messen
  while (loop_count < licht_loop)
  {
    lichtWert_links_summe = lichtWert_links_summe + analogRead(LICHT_links);
    lichtWert_rechts_summe = lichtWert_rechts_summe + analogRead(LICHT_rechts);

    loop_count++;
  }

  // nach 10tem Zyklus Mittelwert der Messwerte bilden und Werte vergleichen
  if (loop_count == licht_loop)
  {
    lichtWert_links = lichtWert_links_summe / licht_loop;
    lichtWert_rechts = lichtWert_rechts_summe / licht_loop;

    lichtWert_durchschnitt = (lichtWert_links + lichtWert_rechts) / 2;

    // falls Setup fehlgeschlagen - erster Messwert ist Umgebungshelligkeit
    if (lichtWert_umgebung == 1)
    {
      lichtWert_umgebung = lichtWert_durchschnitt;
    }

    // wenn Helligkeit groesser, steigt Wert des Analog-Inputs
    // beide Sensoren messen leicht voneinander abweichende Werte in gleicher Helligkeit - Abfangen mit Messtoleranz
    if (abs(lichtWert_links - lichtWert_rechts) < messtoleranz)
    {
      // beide Sensoren messen aehnliche Werte - keine Einstrahlung oder Taschenlampe von vorne

      if ((lichtWert_durchschnitt / lichtWert_umgebung) * 100 >= prozent_einstrahlung)
      {
        // normale Umgebungshelligkeit: Schalten wenn gemessene Werte mehr als x% groesser als Umgebungshelligkeit
        fahrtrichtung = vorwaerts;
      }
      else
      {
        fahrtrichtung = stopp;
      }
    }
    // bei zusaetzlicher Einstrahlung (durch Taschenlampe) ist Messwert 25% hoeher als normal
    else if ((lichtWert_rechts / lichtWert_links) * 100 >= prozent_einstrahlung)
    {
      // fahrtrichtung = rechts;
      fahrtrichtung = links;
    }
    else if ((lichtWert_links / lichtWert_rechts) * 100  >= prozent_einstrahlung)
    {
      // fahrtrichtung = links;
      fahrtrichtung = rechts;
    }

    //    loop_count = 0;
    //    lichtWert_links_summe = 0;
    //    lichtWert_rechts_summe = 0;

  }
  return fahrtrichtung;
}


////----------------------------------- LINE -------------------------------------
fahrtrichtung_t linie_folgen()
{
  // Variablen fuer Liniensensoren
  byte val_links;
  byte val_rechts;

  val_links = digitalRead(LINIE_links);
  val_rechts = digitalRead(LINIE_rechts);

  // Schwarz erkannt = High, Weiß erkannt = Low
  // Fahrzeug ist auf Linie, wenn beide Sensoren keine Linie erkennen
  if ((val_links != linienfarbe) && (val_rechts != linienfarbe))
  {
    fahrtrichtung = vorwaerts;
  }
  // wenn Fahrzeug zu weit rechts faehrt, wird Links Linie erkannt -> mit rechtem Motor nachsteuern
  else if ((val_links == linienfarbe) && (val_rechts != linienfarbe))
  {
    fahrtrichtung = links;
  }
  // analog nach links
  else if ((val_links != linienfarbe) && (val_rechts == linienfarbe))
  {
    fahrtrichtung = rechts;
  }
  else
  {
    fahrtrichtung = stopp;
  }
  return fahrtrichtung;
}


//---------------------------------------------------------------------------------
////------------------------------------ LOOP -------------------------------------
//---------------------------------------------------------------------------------

void robot_loop()
{
  switch (fahrtrichtung)
  {
    case vorwaerts:
      fahre_geradeaus(velocity_drive);
      break;

    case links:
      fahre_links();
      break;

    case rechts:
      fahre_rechts();
      break;

    case stopp:
      anhalten();
      break;
  }
}
