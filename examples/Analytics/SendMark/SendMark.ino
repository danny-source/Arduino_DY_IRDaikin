
#include <DYIRDaikinSend.h>

//#define DYIRDAIKIN_SOFT_IR

DYIRDaikinSend daikinSend;
int isOn;

void setup()
{
	Serial.begin(115200);
	#ifdef DYIRDAIKIN_SOFT_IR
	daikinSend.begin(3);
	#else
	daikinSend.begin();
	#endif
	delay(1000);
	Serial.println();
	Serial.println("Start");
}

void loop() {
	daikinSend.mark(1000);
	delay(1000);
}
