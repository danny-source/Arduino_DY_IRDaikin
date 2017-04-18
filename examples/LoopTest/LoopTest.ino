
#include <DYIRDaikin.h>

#define DYIRDAIKIN_SOFT_IR_1

DYIRDaikin irdaikin;
int isOn;

void setup()
{
	Serial.begin(115200);
	//#ifdef DYIRDAIKIN_SOFT_IR_1
	irdaikin.begin(3);
	//#else
	//irdaikin.begin();
	//#endif
	irdaikin.on();
	irdaikin.setSwing_off();
	irdaikin.setMode(1);
	irdaikin.setFan(4);//FAN speed to MAX
	irdaikin.setTemp(25);
	//----everything is ok and to execute send command-----
	irdaikin.sendCommand();
	isOn = 0;
}

void loop() {
	irdaikin.sendCommand();
	Serial.println("Execute Command!");
	delay(5000);
}
