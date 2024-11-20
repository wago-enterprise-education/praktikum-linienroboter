//pin-definitions
#define TURNSIGNALLEFT 13
#define TURNSIGNALRIGHT 14
#define DISTANCETRIGGER 27
#define DISTANCEECHO 26
#define LIGHTLEFT 33
#define LIGHTRIGHT 32
#define BEEPER 4
#define LINELEFT 18
#define LINERIGHT 19
#define MOTORLEFT 22
#define MOTORRIGHT 23

//lineDrive
#define MOTORSPEED 85 //Hoechstgewschwindigkeit
#define MOTORSLOW 50 //Untere Geschwindigkeit
#define TURNCYCLES 15

//followLight
#define LIGHTDIFFERENCE 150

//overtake
#define SWITCHCYCLES 50

//VARIABLES-----------------------------

//lineDrive
int StopCycles = 0;
int StopWarn = 0;
int StopActivate = 0;

int LeftCycles = 0;
bool LeftBrake = false;

int RightCycles = 0;
bool RightBrake = false;

//followLight
int LDRLeftVal = 0;
int LDRRightVal = 0;

//obstacleScan
long Time = 0;
long Distance = 0;
bool Obstacle = false;

//Road
bool IsOvertaking = false;
bool LineLeft = false;
bool LineRight = false;

//Overtake
bool OvertakeMode = false; //If true, car will try to overtake as if on Road. If false, it will just beep if an obstacle is detected
int Case = 0;
int TurnSignalCounter = 0;
int SwitchCounter = 0;

bool LineCount = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TURNSIGNALLEFT, OUTPUT);
  pinMode(TURNSIGNALRIGHT, OUTPUT);
  pinMode(DISTANCETRIGGER, OUTPUT);
  pinMode(DISTANCEECHO, INPUT);
  pinMode(LIGHTLEFT, INPUT); //LIGHTSENSOR
  pinMode(LIGHTRIGHT, INPUT); //LIGHTSENSOR
  pinMode(BEEPER, OUTPUT);
  pinMode(LINELEFT, INPUT);
  pinMode(LINERIGHT, INPUT);
  pinMode(MOTORLEFT, OUTPUT);
  pinMode(MOTORRIGHT, OUTPUT);
  ledcAttach(MOTORLEFT, 15000, 8);
  ledcAttach(MOTORRIGHT, 15000, 8);
  ledcAttach(BEEPER, 450, 8);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  obstacleScan();
  if(Obstacle || IsOvertaking){
    if(OvertakeMode){
      overtake();
    } else {
      ledcWrite(BEEPER, 8);
      analogWrite(TURNSIGNALLEFT, 255);
      analogWrite(TURNSIGNALRIGHT, 255);
      ledcWrite(MOTORRIGHT, 0);
      ledcWrite(MOTORLEFT, 0);
    }

  } else {
    // hier wird entschieden welches Programm aktuell ausgeführt werden soll.
    // alle Programme die nicht laufen sollen werden auskommentiert

    lineDrive();
    //followLight();
    //road();
  }
  Serial.println("---------------");
}

void lineDrive(){ //Follow the Line
  if(!digitalRead(LINELEFT) && !digitalRead(LINERIGHT)){ //Full Forward
    analogWrite(TURNSIGNALLEFT, 0);
    analogWrite(TURNSIGNALRIGHT, 0);
    ledcWrite(MOTORLEFT, MOTORSPEED);
    ledcWrite(MOTORRIGHT, MOTORSPEED);
    ledcWrite(BEEPER, 0);
    
    RightBrake = false;
    LeftBrake = false;
    StopActivate = 0;
  }
  
  else if(digitalRead(LINERIGHT) && !digitalRead(LINELEFT)){ //RIGHT
    analogWrite(TURNSIGNALRIGHT, 255);
    analogWrite(TURNSIGNALLEFT, 0);
    ledcWrite(MOTORRIGHT, MOTORSLOW);
    ledcWrite(MOTORLEFT, MOTORSPEED);
    
    if(RightBrake){
      ledcWrite(MOTORRIGHT, 0);
    }
    RightCycles += 1;
    if(RightCycles == TURNCYCLES){
      RightCycles = 0;
      RightBrake = true;
    }

    LeftBrake = false;
    StopActivate = 0;
  }

  else if(digitalRead(LINELEFT) && !digitalRead(LINERIGHT)){ //LEFT
    analogWrite(TURNSIGNALLEFT, 255);
    analogWrite(TURNSIGNALRIGHT, 0);
    ledcWrite(MOTORLEFT, MOTORSLOW);
    ledcWrite(MOTORRIGHT, MOTORSPEED);
    if(LeftBrake){
      ledcWrite(MOTORLEFT, 0);
    }
    LeftCycles += 1;
    if(LeftCycles == TURNCYCLES){
      LeftCycles = 0;
      LeftBrake = true;
    }

    RightBrake = false;
    StopActivate = 0;
  }
  
  else { //STOP
    if(StopActivate >= 1){ //delayed brake
      ledcWrite(MOTORRIGHT, 0);
      ledcWrite(MOTORLEFT, 0);
      ledcWrite(BEEPER, 8);
    }

    if(StopWarn == 5){ //Brake Activator
      StopWarn = 0;
      StopActivate += 1;
      if(StopActivate > 5){
        StopActivate -= 1;
      }
    }
    else if(StopWarn == 1 || StopWarn == 3){ //LIGHTS SWITCHING
      analogWrite(TURNSIGNALRIGHT, 255); //Right on
      analogWrite(TURNSIGNALLEFT, 0);
    }
    else{
      analogWrite(TURNSIGNALLEFT, 255); //Left on
      analogWrite(TURNSIGNALRIGHT, 0);
    }

    StopCycles += 1; //CycleCounter
    if(StopCycles == 15){
      StopCycles = 0;
      StopWarn += 1;
    }
  }
}

void followLight(){ //THE DARKER THE HIGHER THE VAL -> Low = Light
  LDRLeftVal = analogRead(LIGHTLEFT);
  LDRRightVal = analogRead(LIGHTRIGHT)+200;//Compensation for different Values

  if(LDRLeftVal < LDRRightVal){ //Left
    if(LDRRightVal - LDRLeftVal > LIGHTDIFFERENCE){
      ledcWrite(MOTORLEFT, MOTORSLOW);
      ledcWrite(MOTORRIGHT, MOTORSPEED);
      ledcWrite(BEEPER, 8);
      analogWrite(TURNSIGNALLEFT, 255);
      analogWrite(TURNSIGNALRIGHT, 0);
    }
  }
  else if(LDRRightVal < LDRLeftVal){ //Right
    if(LDRLeftVal - LDRRightVal > LIGHTDIFFERENCE){
      ledcWrite(MOTORRIGHT, MOTORSLOW);
      ledcWrite(MOTORLEFT, MOTORSPEED);
      ledcWrite(BEEPER, 8);
      analogWrite(TURNSIGNALRIGHT, 255);
      analogWrite(TURNSIGNALLEFT, 0);
    }
  }
  else{ //Forward
    ledcWrite(MOTORLEFT, MOTORSPEED);
    ledcWrite(MOTORRIGHT, MOTORSPEED);
    ledcWrite(BEEPER, 0);
    analogWrite(TURNSIGNALLEFT, 0);
    analogWrite(TURNSIGNALRIGHT, 0);
  }
}

void obstacleScan(){
  digitalWrite(DISTANCETRIGGER, HIGH); //Send Signal
  delayMicroseconds(10);
  digitalWrite(DISTANCETRIGGER, LOW);
  Time = pulseIn(DISTANCEECHO, HIGH); //Receive Signal
  Distance = (Time / 2) * 0.03432;

  if(Distance < 15){
    Obstacle = true;
  }
  else {
    Obstacle = false;
  }
}

void road(){
  LineLeft = digitalRead(LINELEFT);
  LineRight = digitalRead(LINERIGHT);

  if(!LineLeft && !LineRight){
    analogWrite(TURNSIGNALLEFT, 0);
    analogWrite(TURNSIGNALRIGHT, 0);
    ledcWrite(MOTORLEFT, MOTORSPEED);
    ledcWrite(MOTORRIGHT, MOTORSPEED);
    ledcWrite(BEEPER, 0);
  }
  else if(LineLeft && !LineRight){ //Go RIGHT
    ledcWrite(MOTORLEFT, MOTORSPEED);
    ledcWrite(MOTORRIGHT, MOTORSLOW);
  }
  else if(!LineLeft && LineRight){ //Go LEFT
    ledcWrite(MOTORLEFT, MOTORSLOW);
    ledcWrite(MOTORRIGHT, MOTORSPEED);
  }
  else{
    ledcWrite(MOTORRIGHT, 0);
    ledcWrite(MOTORLEFT, 0);
  }
}

void overtake(){
  if(Obstacle && Case >= 3){
    ledcWrite(BEEPER, 8);
    analogWrite(TURNSIGNALLEFT, 255);
    analogWrite(TURNSIGNALRIGHT, 255);
    ledcWrite(MOTORRIGHT, 0);
    ledcWrite(MOTORLEFT, 0);
  }
  else{
    ledcWrite(BEEPER, 0);
    Serial.println("Case: " + Case);
    switch(Case){
    case 0: //Stop and TurnSignal Left
      IsOvertaking = true;
      ledcWrite(MOTORRIGHT, 0);
      ledcWrite(MOTORLEFT, 0);
      analogWrite(TURNSIGNALLEFT, 255);
      delay(500);
      analogWrite(TURNSIGNALLEFT, 0);
      delay(500);
      analogWrite(TURNSIGNALLEFT, 255);
      delay(500);
      analogWrite(TURNSIGNALLEFT, 0);
      delay(500);
      analogWrite(TURNSIGNALLEFT, 255);
      delay(500);
      analogWrite(TURNSIGNALLEFT, 0);
      delay(500);
      Case += 1;
      break;
    case 1: //Turn Left
      ledcWrite(MOTORRIGHT, MOTORSPEED);
      ledcWrite(MOTORLEFT, 0);
      SwitchCounter += 1;
      if(SwitchCounter > SWITCHCYCLES){
        ledcWrite(MOTORRIGHT, 0);
        ledcWrite(MOTORLEFT, 0);
        Case += 1;
        SwitchCounter = 0;
      }
      break;
    case 2: //Drive Till second line
      // LineLeft = digitalRead(LINELEFT); Linecounting, did not work hence remodelling to Time-based
      // LineRight = digitalRead(LINERIGHT);

      // switch(LineCount){
      //   case 0:
      //     ledcWrite(MOTORRIGHT, MOTORSPEED);
      //     ledcWrite(MOTORLEFT, MOTORSPEED);
      //     if(LineLeft) {
      //       LineCount += 1;
      //     }
      //     Serial.println("LINELEFT 1");
      //     break;
      //   case 1:
      //     ledcWrite(MOTORRIGHT, MOTORSPEED);
      //     ledcWrite(MOTORLEFT, MOTORSPEED);
      //     if(LineRight) {
      //       LineCount += 1;
      //     }
      //     Serial.println("LINERIGHT");
      //     break;
      //   case 2:
      //     if(LineLeft){
      //       ledcWrite(MOTORRIGHT, 0);
      //       ledcWrite(MOTORLEFT, 0);
      //       Serial.println("LINELEFT STOP");
      //       Case += 1;
      //     }
      //   break;
      //}
      ledcWrite(MOTORRIGHT, MOTORSPEED);
      ledcWrite(MOTORLEFT, MOTORSPEED);
      delay(500);
      pulseIn(LINELEFT, HIGH);
      ledcWrite(MOTORRIGHT, 0);
      ledcWrite(MOTORLEFT, 0);
      Case += 1;
      break;

    case 3: //Turn Right (onto road)
      ledcWrite(MOTORLEFT, MOTORSPEED);
      ledcWrite(MOTORRIGHT, 0);
      SwitchCounter += 1;
      if(SwitchCounter > SWITCHCYCLES){
        ledcWrite(MOTORRIGHT, 0);
        ledcWrite(MOTORLEFT, 0);
        Case += 1;
        SwitchCounter = 0;
      }
      break;
    case 4: //Drive for certain time
      ledcWrite(MOTORRIGHT, MOTORSPEED);
      ledcWrite(MOTORLEFT, MOTORSPEED);
      delay(2000);
      Case += 1;
      break;
    case 5: //Turn Right TurnSignal Right
      ledcWrite(MOTORLEFT, MOTORSPEED);
      ledcWrite(MOTORRIGHT, 0);
      analogWrite(TURNSIGNALRIGHT, 255);
      SwitchCounter += 1;
      if(SwitchCounter > SWITCHCYCLES){
        ledcWrite(MOTORRIGHT, 0);
        ledcWrite(MOTORLEFT, 0);
        Case += 1;
        SwitchCounter = 0;
      }
      break;
    case 6: //Drive Till second line
      analogWrite(TURNSIGNALRIGHT, 0);
      ledcWrite(MOTORRIGHT, MOTORSPEED);
      ledcWrite(MOTORLEFT, MOTORSPEED);
      delay(500);
      pulseIn(LINERIGHT, HIGH);
      ledcWrite(MOTORRIGHT, 0);
      ledcWrite(MOTORLEFT, 0);
      Case += 1;
      break;
    case 7: //Turn Left (onto Road)
      ledcWrite(MOTORRIGHT, MOTORSPEED);
      ledcWrite(MOTORLEFT, 0);
      SwitchCounter += 1;
      if(SwitchCounter > SWITCHCYCLES){
        ledcWrite(MOTORRIGHT, 0);
        ledcWrite(MOTORLEFT, 0);
        Case += 1;
        SwitchCounter = 0;
      }
      break;
    case 8: //EndCase
      IsOvertaking = false;
      ledcWrite(MOTORRIGHT, 0);
      ledcWrite(MOTORLEFT, 0);
      break;
    }
  }
}

