

#include <DYIRDaikin.h>


DYIRDaikin irdaikin;
void setup()
{
	Serial.begin(115200);
	delay(500);
	Serial.println("DaikinDecodeDemo Start!");
	irdaikin.begin();
	irdaikin.decodePin(4);
}

void loop() {
	if (irdaikin.decode() == 1) {
		irdaikin.description();
	}
}
