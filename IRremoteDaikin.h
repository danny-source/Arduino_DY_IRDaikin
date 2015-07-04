/*
 * IRremote
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
* LG added by Darryl Smith (based on the JVC protocol)
 */

#ifndef IRremoteDaikin_h
#define IRremoteDaikin_h

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

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
