#include <DYIRDaikinRecv.h>

DYIRDaikinRecv _irrecv;
uint8_t		irReceiveData[30] = {0};
#define SSERIAL Serial

void setup() {
	SSERIAL.begin(115200);
	delay(2000);
	SSERIAL.println();
	SSERIAL.println("Daikin Recv Packet Start");
	_irrecv.begin(2,irReceiveData,30);

}

void loop() {
	_irrecv.dumpPackets();
}
