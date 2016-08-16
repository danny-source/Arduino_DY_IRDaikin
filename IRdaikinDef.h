
#ifndef IRdaikinDef_h
#define IRdaikinDef_h

//#define SOFT_IR

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef SOFT_IR
#include <IRremoteIntDaikin.h>
#endif


//DAIKIN
#define DAIKIN_HDR_MARK	    3600 //DAIKIN_ZERO_MARK*8
#define DAIKIN_HDR_SPACE	1600 //DAIKIN_ZERO_MARK*4
#define DAIKIN_ONE_SPACE	1300
#define DAIKIN_ONE_MARK	    380
#define DAIKIN_ZERO_MARK	380
#define DAIKIN_ZERO_SPACE   380


// IR detector output is active low
#define MARK  0
#define SPACE 1


#endif
