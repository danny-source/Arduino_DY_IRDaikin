
#include "DYIRDaikinSend.h"

void DYIRDaikinSend::begin()
{
    IRpin = -1;
    enableIROut(DYIRDAIKIN_FREQUENCY);
}

void DYIRDaikinSend::begin(int IRsendPin)
{
    IRpin = IRsendPin;
    enableIROut(DYIRDAIKIN_FREQUENCY);
    pinMode(IRpin, OUTPUT);
    digitalWrite(IRpin, LOW); // When not sending PWM, we want it low
}


void DYIRDaikinSend::sendDaikin(unsigned char buf[], int len, int start) {
    int data2;
	sendIRWarm();
    enableIROut(DYIRDAIKIN_FREQUENCY);
    mark(DYIRDAIKIN_HDR_MARK);
    space(DYIRDAIKIN_HDR_SPACE);

    for (int i = start; i < start+len; i++) {
        data2=buf[i];

        for (int j = 0; j < 8; j++) {
            if ((1 << j & data2)) {
                mark(DYIRDAIKIN_ONE_MARK);
                space(DYIRDAIKIN_ONE_SPACE);
            }
            else {
                mark(DYIRDAIKIN_ZERO_MARK);
                space(DYIRDAIKIN_ZERO_SPACE);

            }
        }

    }
    mark(DYIRDAIKIN_ONE_MARK);
    space(DYIRDAIKIN_ZERO_SPACE);
}

void DYIRDaikinSend::sendDaikinWake() {
    enableIROut(38);
    space(DYIRDAIKIN_ZERO_MARK);
    //
    mark(DYIRDAIKIN_ZERO_MARK);
    space(DYIRDAIKIN_ZERO_MARK);
    //
    mark(DYIRDAIKIN_ZERO_MARK);
    space(DYIRDAIKIN_ZERO_MARK);
    //
    mark(DYIRDAIKIN_ZERO_MARK);
    space(DYIRDAIKIN_ZERO_MARK);
    //
    mark(DYIRDAIKIN_ZERO_MARK);
    space(DYIRDAIKIN_ZERO_MARK);
    //
    mark(DYIRDAIKIN_ZERO_MARK);
    space(DYIRDAIKIN_ZERO_MARK);
}

void DYIRDaikinSend::sendIRWarm() {
    digitalWrite(IRpin, HIGH);
    delayMicrosecondsEnhance(100);
    digitalWrite(IRpin, LOW);
    delayMicrosecondsEnhance(5);
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
        unsigned long beginTime = micros();
        unsigned long endTime = (unsigned long)time;
        unsigned long nowTime = micros();
        while (nowTime - beginTime < endTime) {
            digitalWrite(IRpin, HIGH);
            //PORTD |= B00000100;
            delayMicrosecondsEnhance(halfPeriodicTimeHigh);
            digitalWrite(IRpin, LOW);
            //PORTD &= B11111011;
            delayMicrosecondsEnhance(halfPeriodicTimeLow);
            nowTime = micros();
        }
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
        int halfPeriodicTime = (500/khz); // T = 1/f but we need T/2 in microsecond and f is in kHz
        int periodicTime = (1000/khz);
#if (AVR_HARDWARE_PWM || defined(DY_IRDAIKIN_SOFTIR))
#if (defined(__AVR_ATmega2560__))
        halfPeriodicTimeHigh = 4;
#elif (defined(__AVR_ATmega32U4__))
		halfPeriodicTimeHigh = 7;
#else
        halfPeriodicTimeHigh = 8;
#endif
#else
        halfPeriodicTimeHigh = (periodicTime / 4) * 3;
#endif
        digitalWrite(IRpin, LOW);
        // 38 kHz -> T = 26.31 microsec (periodic time), half of it is 13
#if (AVR_HARDWARE_PWM || defined(DY_IRDAIKIN_SOFTIR))
#if (defined(__AVR_ATmega2560__))
        halfPeriodicTimeLow = 0;
#elif (defined(__AVR_ATmega32U4__))
		halfPeriodicTimeLow = 0;
#else
        halfPeriodicTimeLow = 0;
#endif
#else
        halfPeriodicTimeLow = (periodicTime / 4) + (periodicTime % 4);
#endif
    }
}

void DYIRDaikinSend::delayMicrosecondsEnhance(uint32_t usec)
{
#if AVR_HARDWARE_PWM
	if (usec > 4) {
		unsigned long tStart = micros();
		unsigned long tEnd = tStart + usec - 4;
#else
	if (usec > 2) {
		unsigned long tStart = micros();
		unsigned long tEnd = tStart + usec - 2;
#endif
		if (tEnd < tStart) { // Check if overflow
		while ( micros() > tStart ) {} // wait until overflow
		}
		while ( micros() < tEnd ) {} // normal wait
	}
}

