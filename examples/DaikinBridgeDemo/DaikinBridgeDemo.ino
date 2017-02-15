
/*
 *
 *
 * Version 0.0.1 Aug, 2014
 * Copyright 2014 danny
 * https://github.com/danny-source/Arduino_IRremote_Daikin
 * Desc:
 * bridge receive and send ir
 */

#include <IRdaikin.h>


IRdaikin irdaikin;
int isOn;

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("DaikinBridgeDemo Start!");
	irdaikin.begin();
	irdaikin.decodePin(4);
	irdaikin.daikin_on();
	irdaikin.daikin_setSwing_off();
	irdaikin.daikin_setMode(1);
	irdaikin.daikin_setFan(4);//FAN speed to MAX
	irdaikin.daikin_setTemp(25);
}

void loop() {
	if (irdaikin.decode() == 1) {
		irdaikin.description();
		delay(1000);
		irdaikin.daikin_sendCommand();
		Serial.println("send command");
	}
}
