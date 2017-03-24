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
private:
uint8_t irDataStoreBuffer();
uint8_t decodeIR(bool lastBit);
uint8_t checkSum(uint8_t *buffer,uint8_t len);
void bitToByteBuffer(uint8_t *buffer, uint8_t value, int restart,uint8_t *bufferPtr);
void printARCState(uint8_t *recvData);
uint8_t isOneMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isZeroMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isStartMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
uint8_t isStopMatched(uint16_t lowTimeCounter,uint16_t highTimecounter);
};

#endif
