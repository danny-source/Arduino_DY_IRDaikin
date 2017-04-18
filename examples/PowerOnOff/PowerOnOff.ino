
#include <DYIRDaikin.h>

//#define DYIRDAIKIN_SOFT_IR

DYIRDaikin irdaikin;
int isOn;

void setup()
{
	Serial.begin(115200);
	#ifdef DYIRDAIKIN_SOFT_IR
	irdaikin.begin(3);
	#else
	irdaikin.begin();
	#endif
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
	while (Serial.available() > 0) {

		if (Serial.read() == '\n') {
			if (isOn == 0){
				isOn = 1;
				irdaikin.off();
				Serial.println("Turn Off");
			}else{
				isOn = 0;
				irdaikin.on();
				Serial.println("Turn On");
			}
			irdaikin.sendCommand();
			Serial.println("Execute Command!");
		}
	}
}
