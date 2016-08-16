/*
 * Arduino IRremote Daikin 2015
 * Copyright 2015 danny
 *
 *
 * enableIROut declare base on  Ken Shirriff's IRremote library.
 * http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 *
 */

#ifndef IRremoteDaikinRecv_h
#define IRremoteDaikinRecv_h
#include <Arduino.h>
#ifndef CORE_ESP8266_FEATURES_H
#include <IRremoteIntDaikin.h>
#else
#include <IRremoteIntDaikinESP.h>
#endif

// Only used for testing; can remove virtual for shorter code
#define VIRTUAL

class IRDaikinRecv
{
public:
	//~ IRDaikinRecv() {}
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
