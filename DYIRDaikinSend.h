
#ifndef DYIRDaikinSend_h
#define DYIRDaikinSend_h
#include "DYIRDaikinDef.h"


class DYIRDaikinSend
{
public:
    DYIRDaikinSend() {}
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

