
#ifndef DYIRDaikinBRC_h
#define DYIRDaikinBRC_h
#include <DYIRDaikinSend.h>
#define DYIRDAIKINBRC_COMMAND_LENGTH 22
class DYIRDaikinBRC
{
public:
    //void IRdaikin();
    void begin();
    void begin(int IRsendPin);
    void on();
    void off();
    void setSwing_on();
    void setSwing_off();
    void setMode(int mode);//
    void setFan(int speed);// 0~1 Hi,Low
    void setTemp(uint8_t temp);//22 ~ 33
    void sendCommand();
    void dump();
    uint8_t getPower();
private:

    DYIRDaikinSend _irsend;
// # of bytes per command

    unsigned char daikinBRC[DYIRDAIKINBRC_COMMAND_LENGTH]     = {
        0x11,0xDA,0x17,0x18,0x04,0x00,0x1E,
//0    1    2   3    4    5     6
        0x11,0xDA,0x17,0x18,0x00,0x73,0x00,0x21,0x00,
//7, 8    9   10   11   12    13   14   15
        0x00,0x20,0x35,0x00,0x20,0x23    };
//16  17    18  19   20    21

    byte vFanTableBRC[3] = { 0x00,0x20};
//0 FAN 1 COOL 2 DRY
    byte vModeTableBRC12[4] = { 0x60,0x20,0x70};
    byte vModeTableBRC14[4] = { 0x00,0x70,0x20};
//22,23,24,25,26,27,28,29,30,31,32,33,34,35,36
    byte vTempTableBRC[16] = {26,28,30,32,34,36,38,40,
                              42,44,46,48,50,52,54                             };
    uint8_t checksum();
;
};

#endif

