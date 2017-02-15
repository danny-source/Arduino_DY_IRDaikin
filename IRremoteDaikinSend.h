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

#ifndef IRremoteDaikinSend_h
#define IRremoteDaikinSend_h
#include "IRdaikinDef.h"

// Only used for testing; can remove virtual for shorter code
//#define VIRTUAL

class IRDaikinSend
{
public:
  IRDaikinSend() {}
  void begin();
  void begin(int IRsendPin);
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendDaikin(unsigned char buf[], int len,int start);
  void sendDaikinWake();
  // private:
  void enableIROut(int khz);
  void mark(int usec);
  void space(int usec);
  void delayMicrosecondsEnhance(uint32_t usec);
private:
	void delayMicrosecondsSys(uint32_t usec);
  int halfPeriodicTime;
  int IRpin;
};

#endif
