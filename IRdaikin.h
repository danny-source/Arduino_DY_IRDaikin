//
#ifndef IRdaikin_h
#define IRdaikin_h
#include <IRremote.h>
#include <IRremoteInt.h>

class IRdaikin
{
public:
  //void IRdaikin();		
  void daikin_on();
  void daikin_off();
  void daikin_setSwing_on();
  void daikin_setSwing_off();
  void daikin_setMode(int mode);//0 FAN, 1 COOL, 2 DRY
  void daikin_setFan(int speed);// 0~4 speed,5 auto,6 moon
  void daikin_setTemp(int temp);//23 ~ 33
  void daikin_sendCommand();

private:
void airController_on();
void airController_off();
void airController_setAux(uint8_t aux);
void airController_setTemp(uint8_t temp);
void airController_setFan(uint8_t fan);
void airController_setMode(uint8_t mode);
void sendDaikinCommand();
uint8_t airController_checksum();
uint8_t airConroller_getState();
};

#endif
