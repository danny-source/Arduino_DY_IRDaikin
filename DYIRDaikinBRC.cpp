/*
 * Arduino IRremote Daikin 2015
 * Copyright 2015 danny
 *
 *
 * Arduino PWM declare base on  Ken Shirriff's IRremote library.
 * http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 *
 */

#include <DYIRDaikinBRC.h>

void DYIRDaikinBRC::begin()
{
    irsend1.begin();
}
void DYIRDaikinBRC::begin(int IRsendPin)
{
    irsend1.begin(IRsendPin);
}

void DYIRDaikinBRC::daikin_on()
{
    daikinController_on();
}

void DYIRDaikinBRC::daikin_off()
{
    daikinController_off();
}

void DYIRDaikinBRC::daikin_setSwing_on()
{
    daikinBRC[18] &=0xfc;
    daikinBRC[18] |=0x01;
    daikinController_checksum();
}

void DYIRDaikinBRC::daikin_setSwing_off()
{
    daikinBRC[18] &=0xfc;
    daikinBRC[18] |=0x02;
    daikinController_checksum();
}

void DYIRDaikinBRC::daikin_setMode(int mode)
{
    if (mode>=0 && mode <=2)
    {
        daikinController_setMode(mode);
    }
}

// 0~4 speed,5 auto,6 moon
void DYIRDaikinBRC::daikin_setFan(int speed)
{
    if (speed>=0 && speed <=1)
    {
        daikinController_setFan(vFanTableBRC[speed]);
    }
}

void DYIRDaikinBRC::daikin_setTemp(uint8_t temp)
{
    if (temp >= 18 && temp<=36)
    {
        //temp = temp-22;
        //daikinBRC[17] = vTempTableBRC[temp];
        temp = temp - 9;
        temp = temp << 1;
        daikinBRC[17] = temp;
        daikinController_checksum();
    }
}

void DYIRDaikinBRC::daikin_sendCommand()
{
    sendDaikinCommand();
}
//
uint8_t DYIRDaikinBRC::daikinController_checksum()
{
    uint8_t sum = 0;
    uint8_t i;


    for (i = 0; i <= 5; i++) {
        sum += daikinBRC[i];
    }

    daikinBRC[6] = sum &0xFF;

    sum=0;
    for (i = 7; i <= 20; i++) {
        sum += daikinBRC[i];
    }

    daikinBRC[21] = sum &0xFF;


}

void DYIRDaikinBRC::dump()
{
    uint8_t i;
    for (i=0; i < DYIRDAIKINBRC_COMMAND_LENGTH; i++) {
        Serial.print(daikinBRC[i],HEX);
        Serial.print("-");
    }
}


//private function

void DYIRDaikinBRC::daikinController_on()
{
    daikinBRC[14] |= 0x01;
    daikinController_checksum();
}

void DYIRDaikinBRC::daikinController_off()
{
    daikinBRC[14] &= 0xFE;
    daikinController_checksum();
}

void DYIRDaikinBRC::daikinController_setTemp(uint8_t temp)
{
    uint8_t t;
    uint8_t compare1;
    uint8_t compare2;
    t=0;
    compare1 = B10000000;

    if (temp >= 22 && temp<=36)
    {
        temp = temp -22;
        daikinBRC[17] = vTempTableBRC[temp];
        daikinController_checksum();
    }
}


void DYIRDaikinBRC::daikinController_setFan(uint8_t fan)
{
    daikinBRC[18] &= 0xdf;
    daikinBRC[18] |= fan;
    //Serial.println(daikinBRC[18],HEX);
    daikinController_checksum();
}

uint8_t DYIRDaikinBRC::daikinController_getState()
{
    return (daikinBRC[13])&0x01;
}

void DYIRDaikinBRC::daikinController_setMode(uint8_t mode)
{
    daikinBRC[12] &=0x8f;
    daikinBRC[12] |=vModeTableBRC12[mode];
    daikinBRC[14] &=0x8f;
    daikinBRC[14] |=vModeTableBRC14[mode];
    daikinController_checksum();
}

void DYIRDaikinBRC::sendDaikinCommand()
{
    daikinController_checksum();
    irsend1.sendDaikin(daikinBRC, 7,0);
    delay(29);
    irsend1.sendDaikin(daikinBRC, 15,7);
}

