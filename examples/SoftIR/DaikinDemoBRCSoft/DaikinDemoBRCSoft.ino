
/*
 *
 *
 * Version 0.0.1 Aug, 2014
 * Copyright 2014 danny
 * https://github.com/danny-source/Arduino_IRremote_Daikin
 */

#include <DYIRDaikinBRC.h>


DYIRDaikinBRC irdaikin;
int isOn;

void setup()
{
  Serial.begin(115200);
 irdaikin.begin(4);
 irdaikin.daikin_on();
 irdaikin.daikin_setFan(0);
 irdaikin.daikin_setTemp(25);
 //----everything is ok and to execute send command-----
 irdaikin.daikin_sendCommand();
 isOn = 0;
}

void loop() {
  while (Serial.available() > 0) {

    if (Serial.read() == '\n') {
        if (isOn == 0){
          isOn = 1;
          irdaikin.daikin_off();
          Serial.println("Turn Off");
        }else{
          isOn = 0;
          irdaikin.daikin_on();
          irdaikin.daikin_setFan(1);
          irdaikin.daikin_setTemp(22);
          Serial.println("Turn On");
        }
        irdaikin.daikin_sendCommand();
        Serial.println("Execute Command!");
    }
  }
}
