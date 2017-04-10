
#include <DYIRDaikinSend.h>


DYIRDaikinSend daikinSend;
int isOn;

void setup()
{
	Serial.begin(115200);
	daikinSend.begin();
	delay(1000);
	Serial.println();
	Serial.println("Start");
}

void loop() {
	daikinSend.mark(1000);
	delay(1000);
}
