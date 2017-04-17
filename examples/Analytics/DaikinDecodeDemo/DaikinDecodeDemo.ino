

#include <DYIRDaikin.h>
#define SSERIAL Serial

DYIRDaikin irdaikin;
void setup()
{
	SSERIAL.begin(115200);
	delay(500);
	SSERIAL.println("DaikinDecodeDemo Start!");
	irdaikin.begin();
	irdaikin.decodePin(2);
}

void loop() {
	if (irdaikin.decode() == 1) {
		irdaikin.description();
	}
}
