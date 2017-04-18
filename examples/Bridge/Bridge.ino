
#include <DYIRDaikin.h>

#define DYIRDAIKIN_SOFT_IR
DYIRDaikin irdaikin;
int isOn;

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("Daikin Bridge Start!");
	#ifdef DYIRDAIKIN_SOFT_IR
	irdaikin.begin(3);
	#else
	irdaikin.begin();
	#endif
	irdaikin.decodePin(4);
	irdaikin.on();
	irdaikin.setSwing_off();
	irdaikin.setMode(1);
	irdaikin.setFan(4);//FAN speed to MAX
	irdaikin.setTemp(25);
}

void loop() {
	if (irdaikin.decode() == 1) {
		irdaikin.description();
		delay(1000);
		irdaikin.sendCommand();
		Serial.println("send command");
	}
}
