
#include "DYIRDaikinSend.h"

void DYIRDaikinSend::begin()
{
    IRpin = -1;
#if ((AVR_HARDWARE_PWM) || defined(DY_IRDAIKIN_SOFTIR))
	IRpin = SOFTIR_PIN;
#endif
	enableIROut(DY_IRDAIKIN_FREQUENCY);
}

void DYIRDaikinSend::begin(int IRsendPin)
{
    IRpin = IRsendPin;
	enableIROut(DY_IRDAIKIN_FREQUENCY);
    pinMode(IRpin, OUTPUT);
    digitalWrite(IRpin, LOW); // When not sending PWM, we want it low
}


void DYIRDaikinSend::sendDaikin(unsigned char buf[], int len, int start) {
    int data2;
    enableIROut(DY_IRDAIKIN_FREQUENCY);
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

void DYIRDaikinSend::sendDaikinWake() {
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

void DYIRDaikinSend::sendRaw(unsigned int buf[], int len, int hz)
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


void DYIRDaikinSend::mark(int time) {
    if (IRpin == -1) {
		//Serial.println();
		//Serial.println("PWM");
        TIMER_ENABLE_PWM; // Enable pin 3 PWM output
        delayMicrosecondsEnhance(time);
    } else {
		//Serial.println();
		//Serial.print("SOFT:");
		//Serial.println(halfPeriodicTime);
		noInterrupts();
        unsigned long beginTime = micros();
        unsigned long endTime = (unsigned long)time;
        while (micros() - beginTime < endTime) {
            digitalWrite(IRpin, HIGH);
            #if (AVR_HARDWARE_PWM & defined(DY_IRDAIKIN_SOFTIR))
				delayMicrosecondsEnhance(halfPeriodicTime - 6);
			#else
				delayMicrosecondsEnhance(halfPeriodicTime);
				#warning "Use soft IR to Simulate!1"
            #endif
				digitalWrite(IRpin, LOW);
            // 38 kHz -> T = 26.31 microsec (periodic time), half of it is 13
            #if (AVR_HARDWARE_PWM & defined(DY_IRDAIKIN_SOFTIR))
				delayMicrosecondsEnhance(halfPeriodicTime - 10);
            #else
				delayMicrosecondsEnhance(halfPeriodicTime);
				#warning "Use soft IR to Simulate!0"
            #endif
        }
        interrupts();
    }
}

/* Leave pin off for time (given in microseconds) */
void DYIRDaikinSend::space(int time) {
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

void DYIRDaikinSend::enableIROut(int khz) {
    if (IRpin == -1) {
        pinMode(TIMER_PWM_PIN, OUTPUT);
        TIMER_CONFIG_KHZ(khz);
    } else {
        halfPeriodicTime = (500/khz); // T = 1/f but we need T/2 in microsecond and f is in kHz
    }
}

void DYIRDaikinSend::delayMicrosecondsEnhance(uint32_t usec)
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

void DYIRDaikinSend::delayMicrosecondsSys(uint32_t usec) {
  while(usec--) {
    _delay_us(1);

  }
}
