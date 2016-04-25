/*
 * IRdaikin
 * Version 0.0.2 Sep, 2014
 * Copyright 2014 danny
 *
 *IRremote library base from Ken Shirriff's IRremote library and add daikin IR function.
 *IRdaikin is using custom IRremote library to simply to send daikin air conditioning ARC remote control ir command.
 *
 *0.Pinout:
 * pin 3:UNO
 * pin 2:Leonardo
 * pin 9:Mega
 *
 *1.Setting:
 *
 *daikin_on();
 *daikin_off();
 *daikin_setSwing_on();
 *daikin_setSwing_off();
 *daikin_setMode(int mode);//0=FAN, 1=COOL, 2=DRY
 *daikin_setFan(int speed);// 0~4=speed(1,2,3,4,5),5=auto,6=moon
 *daikin_setTemp(int temp);//18 ~ 32 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
 *daikin_sendCommand();
 *
 *2.Execute:
 *daikin_sendCommand();
 *
 * You must initial setting by your programming and,
 * After Setting execute daikin_sendCommand()
 *
 *Example:
 *
 *daikin_on();
 *daikin_setSwing_off();
 *daikin_setMode(1)
 *daikin_setFan(4);//FAN speed to MAX
 *daikin_setTemp(25);
 *----everything is ok and to execute send command-----
 *daikin_sendCommand();
 */
#ifndef IRdaikin_h
#define IRdaikin_h
#include <IRremoteDaikin.h>
#include <IRremoteIntDaikin.h>

class IRdaikin
{
public:
  //void IRdaikin();
	void daikin_on();
	void daikin_off();
	void daikin_setPower(uint8_t state);
	void daikin_setSwing_on();
	void daikin_setSwing_off();
	void daikin_setSwing(uint8_t state);
	void daikin_setMode(uint8_t mode);//0 FAN, 1 COOL, 2 DRY
	void daikin_setFan(uint8_t speed);// 0~4 speed,5 auto,6 moon
	void daikin_setTemp(uint8_t temp);//23 ~ 33
	void daikin_sendCommand();
	void dump();
	void description();
	//
	uint8_t daikin_getPower();
	uint8_t daikin_getSwingState();
	uint8_t daikin_getMode();
	uint8_t daikin_getFan();
	uint8_t daikin_getTemp();
	void setPin(int pin);
	uint8_t decode();

private:
void daikinController_on();
void daikinController_off();
void daikinController_setTemp(uint8_t temp);
void daikinController_setFan(uint8_t fan);
void daikinController_setMode(uint8_t mode);
void sendDaikinCommand();
uint8_t daikinController_checksum();
uint8_t daikinController_getState();
//
void receivedIRUpdateToSendBuffer(uint8_t *recvData);
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
