
#include <DYIRDaikinBRC.h>


DYIRDaikinBRC irdaikin;
int isOn;

void setup()
{
  Serial.begin(115200);
 irdaikin.begin(4);
 irdaikin.on();
 irdaikin.setFan(0);
 irdaikin.setTemp(25);
 //----everything is ok and to execute send command-----
 irdaikin.sendCommand();
 isOn = 0;
}

void loop() {
  while (Serial.available() > 0) {

    if (Serial.read() == '\n') {
        if (isOn == 0){
          isOn = 1;
          irdaikin.off();
          Serial.println("Turn Off");
        }else{
          isOn = 0;
          irdaikin.on();
          irdaikin.setFan(1);
          irdaikin.setTemp(22);
          Serial.println("Turn On");
        }
        irdaikin.sendCommand();
        Serial.println("Execute Command!");
    }
  }
}
