
#include <IRdaikinBRC.h>


// # of bytes per command
const int COMMAND_LENGTH_BRC = 22;    

unsigned char daikinBRC[COMMAND_LENGTH_BRC]     = { 
0x11,0xDA,0x17,0x18,0x04,0x00,0x1E,
//0    1    2   3    4    5     6
0x11,0xDA,0x17,0x18,0x00,0x73,0x00,0x21,0x00,
//7, 8    9   10   11   12    13   14   15
0x00,0x20,0x35,0x00,0x20,0x23};
//16  17    18  19   20    21 

static byte vFanTableBRC[] = { 0x00,0x20};
//0 FAN 1 COOL 2 DRY
static byte vModeTableBRC12[] = { 0x60,0x20,0x70};
static byte vModeTableBRC14[] = { 0x00,0x70,0x20};
//22,23,24,25,26,27,28,29,30,31,32,33,34,35,36
static byte vTempTableBRC[] = {26,28,30,32,34,36,38,40,
	42,44,46,48,50,52,54};	
//
// void IRdaikin()
// {
// 	daikinController_off();
// 	daikinController_setMode(0);
// 	daikinController_setTemp(26);
// 	daikinController_setFan(5);
// 	daikin_setSwing_off();
// }
IRDaikinSend irsend1;

void IRdaikinBRC::daikin_on()
{
	daikinController_on();
}  

void IRdaikinBRC::daikin_off()
{
	daikinController_off();
}

void IRdaikinBRC::daikin_setSwing_on()
{
	daikinBRC[18] &=0xfc;
	daikinBRC[18] |=0x01;
	daikinController_checksum();
}

void IRdaikinBRC::daikin_setSwing_off()
{
	daikinBRC[18] &=0xfc;
	daikinBRC[18] |=0x02;
	daikinController_checksum();
}

void IRdaikinBRC::daikin_setMode(int mode)
{
	if (mode>=0 && mode <=2)
	{
		daikinController_setMode(mode);		
	}
}

// 0~4 speed,5 auto,6 moon
void IRdaikinBRC::daikin_setFan(int speed)
{
	if (speed>=0 && speed <=1)
	{
		daikinController_setFan(vFanTableBRC[speed]);
	}
}

void IRdaikinBRC::daikin_setTemp(uint8_t temp)
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

void IRdaikinBRC::daikin_sendCommand()
{
		sendDaikinCommand();
}
//
uint8_t IRdaikinBRC::daikinController_checksum()
{
	uint8_t sum = 0;
	uint8_t i;


	for(i = 0; i <= 5; i++){
		sum += daikinBRC[i];
	}

        daikinBRC[6] = sum &0xFF;
        
        sum=0;
	for(i = 7; i <= 20; i++){
		sum += daikinBRC[i];
        }

        daikinBRC[21] = sum &0xFF;

        
}

void IRdaikinBRC::dump()
{
	uint8_t i;
	for (i=0; i <COMMAND_LENGTH_BRC; i++) {
		Serial.print(daikinBRC[i],HEX);
		Serial.print("-");
	}
}


//private function

void IRdaikinBRC::daikinController_on()
{
	daikinBRC[14] |= 0x01;
	daikinController_checksum();
}

void IRdaikinBRC::daikinController_off()
{
	daikinBRC[14] &= 0xFE;
	daikinController_checksum();
}

void IRdaikinBRC::daikinController_setTemp(uint8_t temp)
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


void IRdaikinBRC::daikinController_setFan(uint8_t fan)
{
	daikinBRC[18] &= 0xdf;
	daikinBRC[18] |= fan;
	//Serial.println(daikinBRC[18],HEX);
	daikinController_checksum();
}

uint8_t IRdaikinBRC::daikinController_getState()
{
	return (daikinBRC[13])&0x01;
}

void IRdaikinBRC::daikinController_setMode(uint8_t mode)
{
	daikinBRC[12] &=0x8f;
	daikinBRC[12] |=vModeTableBRC12[mode];
	daikinBRC[14] &=0x8f;
	daikinBRC[14] |=vModeTableBRC14[mode];
	daikinController_checksum();
}

void IRdaikinBRC::sendDaikinCommand()
{
      daikinController_checksum();  
      irsend1.sendDaikin(daikinBRC, 7,0); 
      delay(29);
      irsend1.sendDaikin(daikinBRC, 15,7); 
}

void IRdaikinBRC::setPin(int pin)
{
	irsend1.setPin(pin);
}