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
#include <IRremoteIntDaikin.h>

// Only used for testing; can remove virtual for shorter code
#define VIRTUAL

class IRDaikinSend
{
public:
  IRDaikinSend() {}
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendDaikin(unsigned char buf[], int len,int start);
  void sendDaikinWake();
  // private:
  void enableIROut(int khz);
  void setPin(int pin);
private:
  VIRTUAL void mark(int usec);
  VIRTUAL void space(int usec);
};

#endif
