
/*
 *
 *
 * Version 0.0.1 Aug, 2014
 * Copyright 2014 danny
 * https://github.com/danny-source/Arduino_IRremote_Daikin
 * Desc:
 * decode ir
 */

#include <IRdaikin.h>


IRdaikin irdaikin;
void setup()
{
	Serial.begin(115200);
	delay(500);
	Serial.println("DaikinDecodeDemo Start!");
	irdaikin.begin(4);
}

void loop() {
	if (irdaikin.decode() == 1) {
		irdaikin.description();		
	}
}
