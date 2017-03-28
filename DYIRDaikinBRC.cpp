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
    _irsend.begin();
}
void DYIRDaikinBRC::begin(int IRsendPin)
{
    _irsend.begin(IRsendPin);
}

void DYIRDaikinBRC::on()
{
    daikinBRC[14] |= 0x01;
    checksum();
}

void DYIRDaikinBRC::off()
{
    daikinBRC[14] &= 0xFE;
    checksum();
}

void DYIRDaikinBRC::setSwing_on()
{
    daikinBRC[18] &=0xfc;
    daikinBRC[18] |=0x01;
    checksum();
}

void DYIRDaikinBRC::setSwing_off()
{
    daikinBRC[18] &=0xfc;
    daikinBRC[18] |=0x02;
    checksum();
}

void DYIRDaikinBRC::setMode(int mode)
{
    if (mode>=0 && mode <=2)
    {
		daikinBRC[12] &=0x8f;
		daikinBRC[12] |=vModeTableBRC12[mode];
		daikinBRC[14] &=0x8f;
		daikinBRC[14] |=vModeTableBRC14[mode];
		checksum();
    }
}

// 0~4 speed,5 auto,6 moon
void DYIRDaikinBRC::setFan(int speed)
{
	uint8_t fan = vFanTableBRC[speed];
    if (speed>=0 && speed <=1)
    {
		daikinBRC[18] &= 0xdf;
		daikinBRC[18] |= fan;
		checksum();
    }
}

void DYIRDaikinBRC::setTemp(uint8_t temp)
{
    if (temp >= 18 && temp<=36)
    {
        temp = temp - 9;
        temp = temp << 1;
        daikinBRC[17] = temp;
        checksum();
    }
}

void DYIRDaikinBRC::sendCommand()
{
    checksum();
    _irsend.sendDaikin(daikinBRC, 7,0);
    delay(29);
    _irsend.sendDaikin(daikinBRC, 15,7);
}
//
uint8_t DYIRDaikinBRC::checksum()
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

uint8_t DYIRDaikinBRC::getPower()
{
    return (daikinBRC[14])&0x01;
}


