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

#include "IRremoteDaikinSend.h"
#ifndef ESP8266
#include <util/delay.h>
#else
#define _delay_us delayMicroseconds
#define TIMER_ENABLE_PWM
#define TIMER_DISABLE_PWM
#define TIMER_CONFIG_KHZ
#define TIMER_PWM_PIN 3
#endif

void IRDaikinSend::begin()
{
    IRpin = -1;
	enableIROut(38);
}

void IRDaikinSend::begin(int IRsendPin)
{
    IRpin = IRsendPin;
	enableIROut(38);
    pinMode(IRpin, OUTPUT);
    digitalWrite(IRpin, LOW); // When not sending PWM, we want it low
}


void IRDaikinSend::sendDaikin(unsigned char buf[], int len, int start) {
    int data2;
    enableIROut(38);
    mark(DAIKIN_HDR_MARK);
    space(DAIKIN_HDR_SPACE);

    for (int i = start; i < start+len; i++) {
        data2=buf[i];

        for (int j = 0; j < 8; j++) {
            if ((1 << j & data2)) {
                mark(DAIKIN_ONE_MARK);
                space(DAIKIN_ONE_SPACE);
            }
            else {
                mark(DAIKIN_ZERO_MARK);
                space(DAIKIN_ZERO_SPACE);

            }
        }

    }
    mark(DAIKIN_ONE_MARK);
    space(DAIKIN_ZERO_SPACE);
}

void IRDaikinSend::sendDaikinWake() {
    enableIROut(38);
    space(DAIKIN_ZERO_MARK);
    //
    mark(DAIKIN_ZERO_MARK);
    space(DAIKIN_ZERO_MARK);
    //
    mark(DAIKIN_ZERO_MARK);
    space(DAIKIN_ZERO_MARK);
    //
    mark(DAIKIN_ZERO_MARK);
    space(DAIKIN_ZERO_MARK);
    //
    mark(DAIKIN_ZERO_MARK);
    space(DAIKIN_ZERO_MARK);
    //
    mark(DAIKIN_ZERO_MARK);
    space(DAIKIN_ZERO_MARK);
}

void IRDaikinSend::sendRaw(unsigned int buf[], int len, int hz)
{
    enableIROut(hz);
    for (int i = 0; i < len; i++) {
        if (i & 1) {
            space(buf[i]);
        }
        else {
            mark(buf[i]);
        }
    }
    space(0); // Just to be sure
}


void IRDaikinSend::mark(int time) {
    if (IRpin == -1) {
		Serial.println();
		Serial.println("PWM");
        TIMER_ENABLE_PWM; // Enable pin 3 PWM output
        delayMicrosecondsEnhance(time);
    } else {
		Serial.println();
		Serial.print("SOFT:");
		Serial.println(halfPeriodicTime);
        unsigned long beginTime = micros();
        unsigned long endTime = (unsigned long)time;
        while (micros() - beginTime < endTime) {
            digitalWrite(IRpin, HIGH);
            delayMicrosecondsEnhance(halfPeriodicTime);
            digitalWrite(IRpin, LOW);
            // 38 kHz -> T = 26.31 microsec (periodic time), half of it is 13
            delayMicrosecondsEnhance(halfPeriodicTime);
        }
    }
}

/* Leave pin off for time (given in microseconds) */
void IRDaikinSend::space(int time) {
    if (IRpin == -1) {
        TIMER_DISABLE_PWM; // Disable pin 3 PWM output
        delayMicrosecondsEnhance(time);
    } else {
        digitalWrite(IRpin, LOW);
        if (time > 0) {
            delayMicrosecondsEnhance(time);
        }
    }
}

//~ #endif

void IRDaikinSend::enableIROut(int khz) {
    if (IRpin == -1) {
        pinMode(TIMER_PWM_PIN, OUTPUT);
        TIMER_CONFIG_KHZ(khz);
    } else {
        halfPeriodicTime = (500/khz); // T = 1/f but we need T/2 in microsecond and f is in kHz
    }
}

void IRDaikinSend::delayMicrosecondsEnhance(uint32_t usec)
{
    if (usec > 100)
    {
        uint32_t tStart,tNow;

        tStart = micros();
        do
        {
            tNow=micros();
        } while (tNow >= tStart && tNow < (tStart + usec - 1));
    }
    else
    {
        if (usec > 1)
            delayMicroseconds(usec-1);
    }
}

void IRDaikinSend::delayMicrosecondsSys(uint32_t usec) {
  while(usec--) {
    _delay_us(1);

  }
}
