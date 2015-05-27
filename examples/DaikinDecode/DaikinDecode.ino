
#define ONE_MARK 1300
#define ONE_SPACE 340

bool isStart;
int nowSignal;
int lastSignal;
unsigned long nowTimer;
unsigned long lastTimer;
unsigned long clearTimer;
unsigned long tempTimer;
//
extern volatile unsigned long timer0_millis;
unsigned long new_value = 0;
//
unsigned long lastBufferSignalLength;
int lastBufferSignal;
int counterLength;

void setup() {
  // put your setup code here, to run once:
     isStart = false;
    Serial.begin(115200);
    lastSignal = digitalRead(3);
    clearTimer = 0;
    Serial.println("Daikin IR decode");
}

void loop() {
  // put your main code here, to run repeatedly:
   pinMode(3,INPUT);
   nowSignal = digitalRead(3);
   nowTimer = micros();   
   //
   if (! (nowSignal ==lastSignal)) {     
       clearTimer = 0;
       if (isStart) {
       //Serial.print(lastSignal);
       //Serial.print("-");
       //Serial.print(nowTimer-lastTimer);
       //Serial.print(" ");
      if (counterLength>1) {
        if ((lastBufferSignal == 1) && (lastSignal == 0)) {
          if ((lastBufferSignalLength > ONE_MARK) && (nowTimer-lastTimer) >ONE_SPACE) {
            Serial.print("1");
          }else {
            Serial.print("0");
          }
        }
      }
      lastBufferSignalLength =  nowTimer-lastTimer;
      lastBufferSignal = lastSignal;
      counterLength ++;
       }
       if (lastSignal == 0 && (nowTimer - lastTimer)>3000) {
         isStart = true;
         counterLength = 0;
         lastBufferSignalLength = 0;
         lastBufferSignal = 0;
         
       }      
       lastTimer = nowTimer;
   }
       if ((lastSignal == nowSignal) && ( nowSignal == 1) ) {
         if (clearTimer == 0) {
           clearTimer = millis();
         } else {
                tempTimer = millis();     
                if (((millis()-clearTimer)>20) && (isStart == true)) {
                     isStart = false;
                     Serial.println();
                     clearTimer = 0;
                }
       } 
   }
   lastSignal = nowSignal;
}

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
