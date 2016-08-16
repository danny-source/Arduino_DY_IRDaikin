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
#define VIRTUAL

class IRDaikinSend
{
public:
  IRDaikinSend() {}
#ifndef SOFT_IR
 void begin();
#else
  void begin(int IRsendPin);
#endif
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendDaikin(unsigned char buf[], int len,int start);
  void sendDaikinWake();
  // private:
  void enableIROut(int khz);
private:
  VIRTUAL void mark(int usec);
  VIRTUAL void space(int usec);
  int halfPeriodicTime;
  int IRpin;  
};

#endif
