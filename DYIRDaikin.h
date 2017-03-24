
#ifndef DYIRDaikin_h
#define DYIRDaikin_h
#include <DYIRDaikinSend.h>
#include <DYIRDaikinRecv.h>
#define DYIRDAIKIN_COMMAND_LENGTH 27
class DYIRDaikin
{
public:
    void begin();
    //soft IR
    void begin(int IRsendPin, uint8_t irRecvPin);
    void begin(int IRsendPin);
    void daikin_on();
    void daikin_off();
    void daikin_setPower(uint8_t state);
    void daikin_setSwing_on();
    void daikin_setSwing_off();
    void daikin_setSwing(uint8_t state);
    void daikin_setMode(uint8_t mode);//0 FAN, 1 COOL, 2 DRY
    void daikin_setFan(uint8_t speed);// 0~4 speed,5 auto,6 moon
    void daikin_setTemp(uint8_t temp);//23 ~ 33
    void daikin_sendCommand();
    void dump();
    void description();
    //
    uint8_t daikin_getPower();
    uint8_t daikin_getSwingState();
    uint8_t daikin_getMode();
    uint8_t daikin_getFan();
    uint8_t daikin_getTemp();
    uint8_t decode();
    void decodePin(uint8_t irRecvPin);

private:
    DYIRDaikinSend irsend;
    DYIRDaikinRecv irrecv;
    unsigned char daikinHeader[8] = {
        0x11,0xDA,0x27,0x00,0xC5,0x00,0x00,0xD7
    };

    unsigned char daikin[DYIRDAIKIN_COMMAND_LENGTH]     = {
        0x11,0xDA,0x27,0xF0,0x00,0x00,0x00,0x20,
//0    1    2   3    4    5     6   7
        0x11,0xDA,0x27,0x00,0x00,0x41,0x1E,0x00,
//8    9   10   11   12    13   14   15
        0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0xC1,0x80,0x00,0xE3    };
//16  17    18  19   20    21   22  23   24   25   26
//0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0xE3 };
    byte vFanTable[8] = { 0x30,0x40,0x50,0x60,0x70,0xa0,0xb0};
//0 FAN 1 COOL 2 DRY 3 HEAT
    byte vModeTable[5] = { 0x6,0x3,0x2,0x4};
//
    uint8_t	irReceiveData[25] = {0};

    void daikinController_on();
    void daikinController_off();
    void daikinController_setTemp(uint8_t temp);
    void daikinController_setFan(uint8_t fan);
    void daikinController_setMode(uint8_t mode);
    void sendDaikinCommand();
    uint8_t daikinController_checksum();
    uint8_t daikinController_getState();
//
    void receivedIRUpdateToSendBuffer(uint8_t *recvData);
};

#endif

