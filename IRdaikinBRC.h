/*
 * IRdaikin
 * Version 0.0.1 Apri, 2015
 * Copyright 2015 danny
 *
 *IRremote library base from Ken Shirriff's IRremote library and add daikin IR function.
 *IRdaikinBRC is using custom IRremote library to simply to send daikin air conditioning BRC remote control ir command.
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
 *daikin_setFan(int speed);// 0-Low,1-High
 *daikin_setTemp(int temp);//18 ~ 36 Celsius,if you using Fahrenheit ,maybe to enter Fahrenheit.
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
 *daikin_setFan(0);//FAN speed to Low
 *daikin_setTemp(25);
 *----everything is ok and to execute send command-----
 *daikin_sendCommand();
 */
#ifndef IRdaikinBRC_h
#define IRdaikinBRC_h
#include <IRremoteDaikin.h>
#include <IRremoteIntDaikin.h>

class IRdaikinBRC
{
public:
  //void IRdaikin();		
  void daikin_on();
  void daikin_off();
  void daikin_setSwing_on();
  void daikin_setSwing_off();
  void daikin_setMode(int mode);//
  void daikin_setFan(int speed);// 0~1 Hi,Low
  void daikin_setTemp(uint8_t temp);//22 ~ 33
  void daikin_sendCommand();
  void dump();
  void setPin(int pin);

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