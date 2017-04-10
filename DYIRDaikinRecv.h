#ifndef DYIRDaikinRecv_h
#define DYIRDaikinRecv_h
#include "DYIRDaikinDef.h"

// Only used for testing; can remove virtual for shorter code
#define VIRTUAL

class DYIRDaikinRecv
{
public:
	uint8_t begin(uint8_t pin,uint8_t *buffer,uint8_t buffer_size);
	uint8_t decode();
	uint8_t dumpPackets();
private:
	uint8_t 	irPin = 7;
//
	uint8_t 	irState = 1;
	uint8_t 	irLastState = 1;
	unsigned long	packetTimeoutCounter = 0;
	unsigned long 	signalTimeoutCounter = 0; //about 120ms

	unsigned long 	duationCounter =0;
	unsigned long 	bitCounter = 0;
	unsigned long	signalCounter = 0;

	uint16_t 	packetCounter =0;
	uint8_t		packetLength;
	uint8_t 	packetNumber = 0;

	uint8_t		hasPacket = 0;
	uint8_t		hasWakupPattern = 0;

	uint8_t		irPatternStateMachine = 0;

	uint8_t		wakePatternCounter = 0;
	uint8_t		*irReceiveDataP0;
	uint8_t		irReceiveDataLen = 0;
	uint8_t 	irSignalState = 0;
	unsigned long irSignalDuation = 0;
	uint8_t		irStateBufIdx = 0;
	uint8_t 	irStateBuf[2]= {0};
	unsigned long irStateDurationBuf[2] = {0};
	//
	uint8_t		receiveBuffer[26] = {0};
	uint8_t		receiveBufferBitPtr;
	uint8_t		receiveBufferIndex;

//
uint8_t isSignalLowHighPaired();
//uint8_t decodePackets();
uint8_t decodePerPacket();
uint8_t checkSum(uint8_t *buffer,uint8_t len);
void fillBitToByte(uint8_t *buffer, uint8_t value, int bitPtr, int bufferIndex);
void printARCState(uint8_t *recvData);
uint8_t isOneMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isZeroMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isStartMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isStopMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
};

#endif
