/*
 * Arduino IRremote Daikin 2015
 * Copyright 2015 danny
 *
 *
 * Arduino PWM declare base on  Ken Shirriff's IRremote library.
 * http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 *
 */

#ifndef IRdaikin_h
#define IRdaikin_h
#include <IRremoteDaikinSend.h>
#include <IRremoteDaikinRecv.h>

class IRdaikin
{
public:
	void begin();
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
};

#endif
