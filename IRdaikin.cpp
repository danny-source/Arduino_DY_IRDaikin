#include <IRdaikin.h>


// # of bytes per command
const int COMMAND_LENGTH = 27;    

unsigned char daikin[COMMAND_LENGTH]     = { 
0x11,0xDA,0x27,0xF0,0x00,0x00,0x00,0x20,
//0    1    2   3    4    5     6   7
0x11,0xDA,0x27,0x00,0x00,0x41,0x1E,0x00,
//8    9   10   11   12    13   14   15
0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0xE3 };
//16  17    18  19   20    21   22  23   24   25   26

static byte vFanTable[] = { 0x30,0x40,0x50,0x60,0x70,0xa0,0xb0};
//0 FAN 1 COOL 2 DRY
static byte vModeTable[] = { 6,3,2};
//
// void IRdaikin()
// {
// 	airController_off();
// 	airController_setMode(0);
// 	airController_setTemp(26);
// 	airController_setFan(5);
// 	daikin_setSwing_off();
// }
IRsend irsend;

void IRdaikin::daikin_on()
{
	airController_on();
}  

void IRdaikin::daikin_off()
{
	airController_off();
}

void IRdaikin::daikin_setSwing_on()
{
	daikin[16]=0xbf;
}

void IRdaikin::daikin_setSwing_off()
{
	daikin[16]=0xb0;
}

void IRdaikin::daikin_setMode(int mode)
{
	if (mode>=0 && mode <=2)
	{
		airController_setMode(vModeTable[mode]);
	}
}

// 0~4 speed,5 auto,6 moon
void IRdaikin::daikin_setFan(int speed)
{
	if (speed>=0 && speed <=6)
	{
		airController_setMode(vFanTable[speed]);
	}
}

void IRdaikin::daikin_setTemp(int temp)
{
	if (temp >= 23 && temp>=33)
	{
		daikin[14] = (temp)*2;
		airController_checksum();
	}
}

void IRdaikin::daikin_sendCommand()
{
		sendDaikinCommand();
}
//
uint8_t IRdaikin::airController_checksum()
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


//private function

void IRdaikin::airController_on(){
	//state = ON;
	daikin[13] |= 0x01;
	airController_checksum();
}

void IRdaikin::airController_off(){
	//state = OFF;
	daikin[13] &= 0xFE;
	airController_checksum();
}

void IRdaikin::airController_setAux(uint8_t aux){
	daikin[21] = aux;
	airController_checksum();
}

void IRdaikin::airController_setTemp(uint8_t temp)
{
	daikin[14] = (temp)*2;
	airController_checksum();
}


void IRdaikin::airController_setFan(uint8_t fan)
{
	daikin[16] = fan;
	airController_checksum();
}

uint8_t IRdaikin::airConroller_getState()
{
	return (daikin[13])&0x01;
}

void IRdaikin::airController_setMode(uint8_t mode)
{
	daikin[13]=mode<<4 | airConroller_getState();
	airController_checksum();
}

void IRdaikin::sendDaikinCommand()
{
      airController_checksum();  
      irsend.sendDaikin(daikin, 8,0); 
      delay(29);
      irsend.sendDaikin(daikin, 19,8); 
}