
/*
 *
 * Copyright 2014 danny
 * https://github.com/danny-source/Arduino_IRremote_Daikin
 */

#include <IRremoteDaikinSend.h>


IRDaikinSend daikinSend;
int isOn;

void setup()
{
	Serial.begin(115200);
	daikinSend.begin(2);
	delay(1000);
	Serial.println();
	Serial.println("Start");
}

void loop() {
	daikinSend.mark(1000);
	delay(1000);
}
