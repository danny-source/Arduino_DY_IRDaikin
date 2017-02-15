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

#ifndef IRdaikinBRC_h
#define IRdaikinBRC_h
#include <IRremoteDaikinSend.h>

class IRdaikinBRC
{
public:
  //void IRdaikin();
	void begin();
	void begin(int IRsendPin);
  void daikin_on();
  void daikin_off();
  void daikin_setSwing_on();
  void daikin_setSwing_off();
  void daikin_setMode(int mode);//
  void daikin_setFan(int speed);// 0~1 Hi,Low
  void daikin_setTemp(uint8_t temp);//22 ~ 33
  void daikin_sendCommand();
  void dump();

private:
void daikinController_on();
void daikinController_off();
void daikinController_setTemp(uint8_t temp);
void daikinController_setFan(uint8_t fan);
void daikinController_setMode(uint8_t mode);
void sendDaikinCommand();
uint8_t daikinController_checksum();
uint8_t daikinController_getState();
};

#endif
