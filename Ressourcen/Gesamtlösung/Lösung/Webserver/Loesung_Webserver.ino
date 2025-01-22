#include "Webserver.cpp"

// Pins
#define TURN_SIGNAL_LEFT 13
#define TURN_SIGNAL_RIGHT 14
#define DISTANCE_TRIGGER 27
#define DISTANCE_ECHO 26
#define LIGHT_LEFT 33
#define LIGHT_RIGHT 32
#define BEEPER 4
#define LINE_LEFT 18
#define LINE_RIGHT 19
#define MOTOR_LEFT 22
#define MOTOR_RIGHT 23

// Constants
int SPEED = 75; // Min: 0; Max: 255;
int MIN_DISTANCE = 15;

Webserver server;

bool blackLine = true;
bool leftSensor = true;
int condition = 0;
unsigned long lastTime;


void setup() {
  Serial.begin(9600);
  // Setup Pins
  pinMode(TURN_SIGNAL_LEFT, OUTPUT);
  pinMode(TURN_SIGNAL_RIGHT, OUTPUT);
  pinMode(DISTANCE_TRIGGER, OUTPUT);
  pinMode(DISTANCE_ECHO, INPUT);
  pinMode(LIGHT_LEFT, INPUT);
  pinMode(LIGHT_RIGHT, INPUT);
  pinMode(BEEPER, OUTPUT);
  pinMode(LINE_LEFT, INPUT);
  pinMode(LINE_RIGHT, INPUT);
  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);

  // Config Beeper
  ledcAttach(BEEPER, 450, 8);

  // Start Webserver
  server.start("ESP32", "");
}


void loop() {
  // IO switch
  if(!server.getIOSwitch()){
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, 0);
    digitalWrite(TURN_SIGNAL_LEFT, 0);
    digitalWrite(TURN_SIGNAL_RIGHT, 0);
    ledcWrite(BEEPER, 0);
    return;
  };

  // Settings
  blackLine = server.isBlackLine();

  // Function calls
  if(server.getObstacleButton()){
    if(obstacleScan()) return;
  }

  if(server.getLineButton()){
    followLine();
    return;
  }
  if(server.getLightButton()){
    followLight();
    return;
  }

  if(server.getPadFor()){
    analogWrite(MOTOR_LEFT, SPEED);
    analogWrite(MOTOR_RIGHT, SPEED);
  }
  if(server.getPadRight()){
    analogWrite(MOTOR_LEFT, SPEED);
    analogWrite(MOTOR_RIGHT, 0);
  }
  if(server.getPadBack()){
    // No backward drive compatibility
    // Placeholder for additional functionality
  }
  if(server.getPadLeft()){
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, SPEED);
  }

  if(!server.getPadFor() && !server.getPadRight() && !server.getPadBack() && !server.getPadLeft() && !server.getLineButton() && !server.getLightButton()) {
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, 0);
  }

  if(server.getHornButton()){
    ledcWrite(BEEPER, 8);
  }else {
    ledcWrite(BEEPER, 0);
  }
}

/**
 * Follow the line
 */
void followLine(){
  if(digitalRead(LINE_LEFT) == blackLine && digitalRead(LINE_RIGHT) == blackLine){
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, 0);
    digitalWrite(TURN_SIGNAL_LEFT, HIGH);
    digitalWrite(TURN_SIGNAL_RIGHT, HIGH);
    ledcWrite(BEEPER, 8);
  }else{
    if(digitalRead(LINE_LEFT) == blackLine){
      digitalWrite(TURN_SIGNAL_LEFT, HIGH);
      analogWrite(MOTOR_LEFT, SPEED / 5);
      analogWrite(MOTOR_RIGHT, SPEED);
    }else if(digitalRead(LINE_RIGHT) == blackLine){
      digitalWrite(TURN_SIGNAL_RIGHT, HIGH);
      analogWrite(MOTOR_LEFT, SPEED);
      analogWrite(MOTOR_RIGHT, SPEED / 5);
    }else {
      digitalWrite(TURN_SIGNAL_LEFT, LOW);
      digitalWrite(TURN_SIGNAL_RIGHT, LOW);
      analogWrite(MOTOR_LEFT, SPEED);
      analogWrite(MOTOR_RIGHT, SPEED);
    }
    ledcWrite(BEEPER, 0);
  }
}


/**
 * Follow the line for sharp turns
 */
void followLineSharp(){
  if(digitalRead(LINE_LEFT) == blackLine && digitalRead(LINE_RIGHT) == blackLine){
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, 0);
    digitalWrite(TURN_SIGNAL_LEFT, HIGH);
    digitalWrite(TURN_SIGNAL_RIGHT, HIGH);
    ledcWrite(BEEPER, 8);
  }else{
    if(digitalRead(LINE_LEFT) == blackLine){
      leftSensor = true;
      digitalWrite(TURN_SIGNAL_RIGHT, LOW);
    }
    if(digitalRead(LINE_RIGHT) == blackLine){
      leftSensor = false;
      digitalWrite(TURN_SIGNAL_LEFT, LOW);
    }
    if(leftSensor){
      analogWrite(MOTOR_LEFT, 0);
      analogWrite(MOTOR_RIGHT, SPEED);
      digitalWrite(TURN_SIGNAL_LEFT, HIGH);
    }else{
      analogWrite(MOTOR_LEFT, SPEED);
      analogWrite(MOTOR_RIGHT, 0);
      digitalWrite(TURN_SIGNAL_RIGHT, HIGH);
    }
    ledcWrite(BEEPER, 0);
  }
}


/**
 * Follow the light
 */
void followLight(){
  if(analogRead(LIGHT_LEFT) <= analogRead(LIGHT_RIGHT)){
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, SPEED);
    digitalWrite(TURN_SIGNAL_LEFT, HIGH);
    digitalWrite(TURN_SIGNAL_RIGHT, LOW);
  }else{
    analogWrite(MOTOR_LEFT, SPEED);
    analogWrite(MOTOR_RIGHT, 0);
    digitalWrite(TURN_SIGNAL_RIGHT, HIGH);
    digitalWrite(TURN_SIGNAL_LEFT, LOW);
  }
}

/**
 * Check if a obstacle is in the way and turn of motor if MIN_DISTANCE is reached
 *
 * @return Obstacle in MIN_DISTANCE
 */
bool obstacleScan(){
  digitalWrite(DISTANCE_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_TRIGGER, LOW);
  long time = pulseIn(DISTANCE_ECHO, HIGH);
  long distance = (time / 2) * 0.03432;

  if(distance < MIN_DISTANCE){
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, 0);
    ledcWrite(BEEPER, 8);
    return true;
  }
  ledcWrite(BEEPER, 0);
  return false;
}

/**
 * Implementation of the overtake exercise with time
 * WARNING: Not final. Stuff is breaking
 */
void overtake(){
  digitalWrite(DISTANCE_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_TRIGGER, LOW);
  long time = pulseIn(DISTANCE_ECHO, HIGH);
  long distance = (time / 2) * 0.03432;
  switch(condition){
    case 0:
      if(distance > 15){
        analogWrite(MOTOR_LEFT, SPEED);
        analogWrite(MOTOR_RIGHT, SPEED);
      }else{
        analogWrite(MOTOR_LEFT, 0);
        analogWrite(MOTOR_RIGHT, 0);
        condition = 1;
      }
      break;
    case 1:
      for(int i = 0; i < 4; i++){
        Serial.println(i);
        digitalWrite(TURN_SIGNAL_LEFT, HIGH);
        delay(400);
        digitalWrite(TURN_SIGNAL_LEFT, LOW);
        delay(400);
      }
      condition = 2;
      break;
    case 2:
      analogWrite(MOTOR_RIGHT, SPEED);
      if(digitalRead(LINE_LEFT)){
        analogWrite(MOTOR_LEFT, SPEED);
        analogWrite(MOTOR_RIGHT, SPEED);
        delay(100);
        condition = 3;
      }
      break;
    case 3:
      if(digitalRead(LINE_LEFT)){
        analogWrite(MOTOR_RIGHT, 0);
        delay(550);
        analogWrite(MOTOR_LEFT, 0);
        time = millis();
        condition = 4;
      }
      break;
    case 4:
      if(distance > 15 && millis() - time < 2000){
        analogWrite(MOTOR_LEFT, SPEED);
        analogWrite(MOTOR_RIGHT, SPEED);
      }else if(distance <= 15){
        analogWrite(MOTOR_LEFT, 0);
        analogWrite(MOTOR_RIGHT, 0);
        ledcWrite(BEEPER, 8);
        time = millis();
      }else{
        analogWrite(MOTOR_LEFT, 0);
        analogWrite(MOTOR_RIGHT, 0);
        ledcWrite(BEEPER, 0);
        condition = 5;
      }
      break;
    case 5:
      analogWrite(MOTOR_LEFT, SPEED);
      if(digitalRead(LINE_RIGHT)){
        analogWrite(MOTOR_LEFT, SPEED);
        analogWrite(MOTOR_RIGHT, SPEED);
        delay(100);
        condition = 6;
      }
      break;
    case 6:
      if(digitalRead(LINE_RIGHT)){
        analogWrite(MOTOR_LEFT, 0);
        delay(550);
        analogWrite(MOTOR_RIGHT, 0);
        condition = 0;
      }
      break;
  }
}