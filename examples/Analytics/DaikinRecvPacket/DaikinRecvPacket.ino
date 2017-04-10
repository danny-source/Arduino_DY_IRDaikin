#include <DYIRDaikinRecv.h>

DYIRDaikinRecv _irrecv;
uint8_t		irReceiveData[26] = {0};

void setup() {
	Serial.begin(115200);
	delay(2000);
	Serial.println();
	Serial.println("Daikin Recv Packet Start");
	_irrecv.begin(4,irReceiveData,26);

}

void loop() {
	_irrecv.dumpPackets();
}
