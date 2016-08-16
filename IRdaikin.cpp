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


#include <IRdaikin.h>


//IR send
// # of bytes per command
const int COMMAND_LENGTH = 27;

unsigned char daikinHeader[8] = {
	0x11,0xDA,0x27,0x00,0xC5,0x00,0x00,0xD7
};

unsigned char daikin[COMMAND_LENGTH]     = {
0x11,0xDA,0x27,0xF0,0x00,0x00,0x00,0x20,
//0    1    2   3    4    5     6   7
0x11,0xDA,0x27,0x00,0x00,0x41,0x1E,0x00,
//8    9   10   11   12    13   14   15
0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0xC1,0x80,0x00,0xE3 };
//16  17    18  19   20    21   22  23   24   25   26
//0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0xE3 };
static byte vFanTable[] = { 0x30,0x40,0x50,0x60,0x70,0xa0,0xb0};
//0 FAN 1 COOL 2 DRY
static byte vModeTable[] = { 0x6,0x3,0x2};
//
uint8_t	irReceiveData[25] = {0};

IRDaikinSend irsend;
IRDaikinRecv irrecv;

//

#ifndef SOFT_IR
void IRdaikin::begin(uint8_t irRecvPin)
{
 	irrecv.begin(irRecvPin,irReceiveData,25);
}
void IRdaikin::begin()
{
}
#else
void IRdaikin::begin(int IRsendPin, uint8_t irRecvPin)
{
	irsend.begin(IRsendPin);
 	irrecv.begin(irRecvPin,irReceiveData,25);
}
void IRdaikin::begin(int IRsendPin)
{
	irsend.begin(IRsendPin);
}
#endif

void IRdaikin::daikin_on()
{
	daikinController_on();
}

void IRdaikin::daikin_off()
{
	daikinController_off();
}

void IRdaikin::daikin_setPower(uint8_t state)
{
	if (state == 1) {
		daikinController_on();
	}else {
		daikinController_off();
	}
}

uint8_t IRdaikin::daikin_getPower()
{
	return (daikin[13] & 0x01);
}

void IRdaikin::daikin_setSwing_on()
{
	daikin[16] |=0x0f;
	daikinController_checksum();
}

void IRdaikin::daikin_setSwing_off()
{
	daikin[16] &=0xf0;
	daikinController_checksum();
}

void IRdaikin::daikin_setSwing(uint8_t state)
{
	if (state == 1) {
		daikin_setSwing_on();
	}else {
		daikin_setSwing_off();
	}
}

uint8_t IRdaikin::daikin_getSwingState()
{
	uint8_t state = daikin[16] & 0x0f;
	if (state == 0x0f) {
		return 1;
	}
	return 0;
}

void IRdaikin::daikin_setMode(uint8_t mode)
{
	if (mode>=0 && mode <=2)
	{
		daikinController_setMode(vModeTable[mode]);
	}
}

uint8_t IRdaikin::daikin_getMode()
{
	uint8_t mode = (daikin[13] & B01110000) >> 4;
	if (mode == 0x6) mode = 0;
	if (mode == 0x3) mode = 1;
	if (mode == 0x2) mode = 2;
	return mode;
}

// 0~4 speed,5 auto,6 moon
void IRdaikin::daikin_setFan(uint8_t speed)
{
	if (speed>=0 && speed <=6)
	{
		daikinController_setFan(vFanTable[speed]);
	}
}

uint8_t IRdaikin::daikin_getFan()
{
	uint8_t fan = (daikin[16] & 0xf0);
	if (fan == 0x30) fan = 0;
	if (fan == 0x40) fan = 1;
	if (fan == 0x50) fan = 2;
	if (fan == 0x60) fan = 3;
	if (fan == 0x70) fan = 4;
	if (fan == 0xa0) fan = 5;
	if (fan == 0xb0) fan = 6;
	return fan;
}

void IRdaikin::daikin_setTemp(uint8_t temp)
{
	if (temp >= 18 && temp<=32)
	{
		daikin[14] = (temp)*2;
		daikinController_checksum();
	}
}

uint8_t IRdaikin::daikin_getTemp()
{
	uint8_t temperature= (daikin[14] & B01111110) >> 1;
	return temperature;
}

void IRdaikin::daikin_sendCommand()
{
		sendDaikinCommand();
}
//
void IRdaikin::dump()
{
	uint8_t i;
	for (i=0; i <COMMAND_LENGTH; i++) {
		Serial.print(daikin[i],HEX);
		Serial.print("-");
	}
}

void IRdaikin::description()
{
	Serial.print(F("\r\n==send buffer==\r\n"));
	Serial.print(F("Power:"));
	Serial.print(daikin_getPower(),DEC);
	Serial.println();
	Serial.print(F("Mode:"));
	Serial.print(daikin_getMode(),DEC);
	Serial.println();
	Serial.print(F("Fan:"));
	Serial.print(daikin_getFan(),DEC);
	Serial.println();
	Serial.print(F("Temperature:"));
	Serial.print(daikin_getTemp(),DEC);
	Serial.println();
	Serial.print(F("Swing:"));
	Serial.print(daikin_getSwingState(),DEC);
	Serial.println();
}

//private function
uint8_t IRdaikin::daikinController_checksum()
{
	uint8_t sum = 0;
	uint8_t i;


	for(i = 0; i <= 6; i++){
		sum += daikin[i];
	}

        daikin[7] = sum &0xFF;

        sum=0;
	for(i = 8; i <= 25; i++){
		sum += daikin[i];
        }

        daikin[26] = sum &0xFF;


}

void IRdaikin::daikinController_on()
{
	daikin[13] |= 0x01;
	daikinController_checksum();
}

void IRdaikin::daikinController_off()
{
	daikin[13] &= 0xFE;
	daikinController_checksum();
}

void IRdaikin::daikinController_setTemp(uint8_t temp)
{
	daikin[14] = (temp)*2;
	daikinController_checksum();
}


void IRdaikin::daikinController_setFan(uint8_t fan)
{
	daikin[16] &= 0x0f;
	daikin[16] |= fan;
	daikinController_checksum();
}

uint8_t IRdaikin::daikinController_getState()
{
	return (daikin[13])&0x01;
}

void IRdaikin::daikinController_setMode(uint8_t mode)
{
	daikin[13]=mode<<4 | daikinController_getState();
	daikinController_checksum();
}

void IRdaikin::sendDaikinCommand()
{
	  //~ irsend.sendDaikinWake();
      //~ delay(20);
      //~ irsend.sendDaikin(daikinHeader, 9,0);
      //~ delay(29);
      delay(25);
      daikinController_checksum();
      irsend.sendDaikin(daikin, 8,0);
      delay(29);
      irsend.sendDaikin(daikin, 19,8);
}

uint8_t IRdaikin::decode() {
	if (irrecv.decode()>10) {
		receivedIRUpdateToSendBuffer(irReceiveData);
		return 1;
	}
	return 0;
}
//
void IRdaikin::receivedIRUpdateToSendBuffer(uint8_t *recvData) {
	//decode all state
	//~ static byte vFanTable[] = { 0x30,0x40,0x50,0x60,0x70,0xa0,0xb0};
	uint8_t temperature= (recvData[6] & B01111110) >> 1;
	uint8_t fan = (recvData[8] & 0xf0);
	if (fan == 0x30) fan = 0;
	if (fan == 0x40) fan = 1;
	if (fan == 0x50) fan = 2;
	if (fan == 0x60) fan = 3;
	if (fan == 0x70) fan = 4;
	if (fan == 0xa0) fan = 5;
	if (fan == 0xb0) fan = 6;

	uint8_t swing = (recvData[8] & 0x01);
	uint8_t powerState =  (recvData[5] & 0x01);
	uint8_t timerOn =  (recvData[5] & 0x02) >> 1;
	uint16_t timerOnValue = (uint16_t)recvData[10]|(uint16_t)(recvData[11] & B00000111)<<8;
	//~ AAAAAAAA AAAXBBBB BBBBBBBX
	uint8_t timerOff =  (recvData[5] & 0x04) >> 2;
	uint16_t timerOffValue = (uint16_t)((recvData[11] & B11110000) >> 4)|(uint16_t)(recvData[12] & B01111111)<<4;
	uint8_t mode = (recvData[5] & B01110000) >> 4;
	//
	uint16_t timeNow = 0;

	//~ if (packetLength == 3) {
		//~ timeNow = (uint16_t)recvData[5]|(uint16_t)(recvData[6] & B00000111)<<8;
	//~ }

	//~ static byte vModeTable[] = { 0x6,0x3,0x2};
	if (mode == 0x6) mode = 0;
	if (mode == 0x3) mode = 1;
	if (mode == 0x2) mode = 2;
	//
	uint8_t econo = (recvData[16] & B00000100) >> 2;
	//set all state
	daikin_setPower(powerState);
	daikin_setMode(mode);
	daikin_setFan(fan);
	daikin_setTemp(temperature);
	daikin_setSwing(swing);


	//~ Serial.print(F("\r\n==receive buffer==\r\n"));
	//~ Serial.print(F("Power:"));
	//~ Serial.print(powerState,DEC);
	//~ Serial.println();
	//~ Serial.print(F("Mode:"));
	//~ Serial.print(mode,DEC);
	//~ Serial.println();
	//~ Serial.print(F("Fan:"));
	//~ Serial.print(fan,DEC);
	//~ Serial.println();
	//~ Serial.print(F("Temperature:"));
	//~ Serial.print(temperature,DEC);
	//~ Serial.println();
	//~ Serial.print(F("Swing:"));
	//~ Serial.print(swing,DEC);
	//~ Serial.println();
	//~ Serial.print("Econo:");
	//~ Serial.print(econo,DEC);
	//~ Serial.println();
	//~ Serial.print("Timer On:");
	//~ Serial.print(timerOn,DEC);
	//~ Serial.println();
	//~ Serial.print("Timer On Value:");
	//~ Serial.print((timerOnValue / 60),DEC);
	//~ Serial.print(":");
	//~ Serial.print((timerOnValue % 60),DEC);
	//~ Serial.println();
	//~ Serial.print("Timer Off:");
	//~ Serial.print(timerOff,DEC);
	//~ Serial.println();
	//~ Serial.print("Timer Off Value:");
	//~ Serial.print((timerOffValue / 60),DEC);
	//~ Serial.print(":");
	//~ Serial.print((timerOffValue % 60),DEC);
	//~ Serial.println();
	//~ Serial.print("Timer Now:");
	//~ Serial.print((timeNow / 60),DEC);
	//~ Serial.print(":");
	//~ Serial.print((timeNow % 60),DEC);
	//~ Serial.println();
}
