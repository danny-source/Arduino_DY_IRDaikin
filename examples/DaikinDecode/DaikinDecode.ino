
#define ONE_MARK_H 2000
#define ONE_MARK_L 1000
#define ONE_SPACE_H 900
#define ONE_SPACE_L 300
#define ZERO_MARK_H 900
#define ZERO_MARK_L 300
#define ZERO_SPACE_H 900
#define ZERO_SPACE_L 300
#define START_MARK_H 2000
#define START_MARK_L 1000
#define START_SPACE_H 3800
#define START_SPACE_L 3000

bool isStart;
int nowSignal;
int lastSignal;
unsigned long signalHighStartTime;
unsigned long signalLowStartTime;
unsigned long nowTimer;
unsigned long tempTimer;
unsigned long lastSignalDuration;
int lastSignalState;
unsigned long nowSignalDuration;
int nowSignalState;
//
extern volatile unsigned long timer0_millis;
unsigned long new_value = 0;
String pattern;
//

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    lastSignal = digitalRead(3);
    Serial.println("Daikin IR decode");
    //initial
    lastSignalDuration = 0;
    lastSignalState = 0;
    isStart = false;
    //
    nowTimer = micros();
    nowSignal = digitalRead(3);
    lastSignal = nowSignal;
    if (nowSignal == 1) {
      signalHighStartTime = nowSignal;
    }else {
      signalLowStartTime = nowSignal;
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  
   pinMode(3,INPUT);
   nowSignal = digitalRead(3);
   nowTimer = micros();
   if (!(nowSignal == lastSignal)) {
     if (lastSignal == 1) {
       nowSignalDuration = nowTimer - signalHighStartTime;
       nowSignalState = 1;
       signalLowStartTime = nowTimer;
     }else {
       nowSignalDuration = nowTimer - signalLowStartTime;
       nowSignalState = 0;
       signalHighStartTime = nowTimer;       
     }
     //
     if ((nowSignalState ==1) && ( lastSignalState == 0) && (lastSignalDuration > START_SPACE_L) && (lastSignalDuration < START_SPACE_H) && 
         (nowSignalDuration >START_MARK_L) && (nowSignalDuration < START_MARK_H)) {
           isStart = true;
           pattern = "";           
     }     
     checkPattern();
     //
     lastSignalDuration = nowSignalDuration;
     lastSignalState = nowSignalState;
    
     //
//     Serial.print("[");
//     Serial.print(lastSignalState);       
//     Serial.print(":");
//     Serial.println(lastSignalDuration);
//      Serial.print("]");     
    }
    
   lastSignal = nowSignal;
   
    if ((lastSignal == 1) && (isStart == true) && ((nowTimer - signalHighStartTime) > 6000)) {
        Serial.println();
        isStart = false;
        Serial.print(pattern);

    }
}

void checkPattern() {
     if ((lastSignalDuration >0) && (nowSignalDuration>0) && (isStart == true)) {
       if ((lastSignalState == 0) && (nowSignalState == 1)) {
         if ((lastSignalDuration > ONE_SPACE_L) && (lastSignalDuration < ONE_SPACE_H) && 
         (nowSignalDuration >ONE_MARK_L) && (nowSignalDuration <ONE_MARK_H)) {
           //Serial.print("1");
           pattern = pattern +"1";
           //nowSignalDuration = 0;
         }else if ((lastSignalDuration > ZERO_SPACE_L) && (lastSignalDuration < ZERO_SPACE_H) && 
         (nowSignalDuration >ZERO_MARK_L) && (nowSignalDuration <ZERO_MARK_H)) {
           //Serial.print("0");
           //nowSignalDuration = 0;
            pattern = pattern +"0";           
         }        
      }
         
     }  
}

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
