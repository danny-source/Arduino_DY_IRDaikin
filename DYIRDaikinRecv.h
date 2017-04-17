#ifndef DYIRDaikinRecv_h
#define DYIRDaikinRecv_h
#include "DYIRDaikinDef.h"

// Only used for testing; can remove virtual for shorter code
#define VIRTUAL
//#define DYIRDAIKIN_DEBUG_PRINT_SIGNAL_DUATION

//#if (defined(__AVR_ATmega328P__) | defined(__AVR_ATmega8__) | defined(__AVR_ATmega8P__))
//#warning "Use PIND read"
//#define readIR(p)	((PIND & bitMask) ? 1:0);
//#else
#define readIR(p)	digitalRead(p);
//#endif

class DYIRDaikinRecv
{
public:
	uint8_t begin(uint8_t pin,uint8_t *buffer,uint8_t buffer_size);
	uint8_t decode();
	uint8_t dumpPackets();
	void descriptionARC(uint8_t *recvData);
private:
	uint8_t 	irPin = 7;
//
	uint8_t 	irState = 1;
	uint8_t 	irLastState = 1;
	uint16_t	packetTimeoutCounter = 0;
	uint16_t 	signalTimeoutCounter = 0; //about 120ms

	uint16_t 	duationCounter =0;
	uint16_t 	bitCounter = 0;
	uint16_t	signalCounter = 0;

	uint8_t 	packetCounter =0;
	uint8_t		packetLength;
	uint8_t 	packetNumber = 0;

	uint8_t		hasPacket = 0;
	uint8_t		hasWakupPattern = 0;

	uint8_t		irPatternStateMachine = 0;

	uint8_t		wakePatternCounter = 0;
	uint8_t		*irReceiveDataP0;
	uint8_t		irReceiveDataLen = 0;
	uint8_t 	irSignalState = 0;
	uint16_t irSignalDuation = 0;
	uint16_t		irStateBufIdx = 0;
	uint8_t 	irStateBuf[2]= {0};
	uint16_t irStateDurationBuf[2] = {0};
	//
	uint8_t		receiveBuffer[26] = {0};
	uint8_t		receiveBufferBitPtr = 0;
	uint8_t		receiveBufferIndex = 0;
	uint8_t		bitMask = B00000001;
	//
	#ifdef DYIRDAIKIN_DEBUG_PRINT_SIGNAL_DUATION
	uint8_t 	irStateDebugBuf[310]= {0};
	uint16_t irStateDurationDebugBuf[310] = {0};
	uint16_t	irStateDebugIdx = 0;
	#endif
	unsigned long startTime;
	unsigned long endTime;

//
uint8_t isSignalLowHighPaired();
//uint8_t decodePackets();
uint8_t decodePerPacket();
uint8_t checkSum(uint8_t *buffer,uint8_t len);
void fillBitToByte(uint8_t *buffer, uint8_t value, uint8_t *bitPtr, uint8_t *bufferIndex);
uint8_t isOneMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isZeroMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isStartMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isStopMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
};
#endif
