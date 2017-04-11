
#include <DYIRDaikin.h>

//
void DYIRDaikin::begin()
{
	_irsend.begin();
}

void DYIRDaikin::begin(int irSendPin, uint8_t irRecvPin)
{
	_irsend.begin(irSendPin);
 	_irrecv.begin(irRecvPin,irReceiveData,25);
}
void DYIRDaikin::begin(int irSendPin)
{
	_irsend.begin(irSendPin);
}

void DYIRDaikin::decodePin(uint8_t irRecvPin)
{
 	_irrecv.begin(irRecvPin,irReceiveData,25);
}


void DYIRDaikin::on()
{
	daikin[13] |= 0x01;
	checksum();
}

void DYIRDaikin::off()
{
	daikin[13] &= 0xFE;
	checksum();
}

void DYIRDaikin::setPower(uint8_t state)
{
	if (state == 0) {
		off();
	}else {
		on();
	}
}

uint8_t DYIRDaikin::getPower()
{
	return (daikin[13] & 0x01);
}

void DYIRDaikin::setSwing_on()
{
	daikin[16] |=0x0f;
	checksum();
}

void DYIRDaikin::setSwing_off()
{
	daikin[16] &=0xf0;
	checksum();
}

void DYIRDaikin::setSwing(uint8_t state)
{
	if (state == 0) {
		setSwing_off();
	}else {
		setSwing_on();
	}
}

uint8_t DYIRDaikin::getSwing()
{
	uint8_t state = daikin[16] & 0x0f;
	if (state == 0x0f) {
		return 1;
	}
	return 0;
}

void DYIRDaikin::setSwingLR_on()
{
	daikin[17] = daikin[17] | 0x0F;
	checksum();
}

void DYIRDaikin::setSwingLR_off()
{
	daikin[17] = daikin[17] & 0xF0;
	checksum();
}

void DYIRDaikin::setSwingLR(uint8_t state)
{
    if (state == 0) {
        setSwingLR_off();
	}else {
        setSwingLR_on();
	}
}

uint8_t DYIRDaikin::getSwingLR()
{
    return (daikin[17]) & 0x01;
}

void DYIRDaikin::setMode(uint8_t mode)
{
	uint8_t trmode = vModeTable[mode];
	if (mode>=0 && mode <=4)
	{
		daikin[13] = (trmode << 4) | getPower();
		checksum();
	}
}

uint8_t DYIRDaikin::getMode()
{
	uint8_t mode = (daikin[13] & B01110000) >> 4;
	if (mode == 0x6) {
		mode = 0;
	}else if (mode == 0x3) {
		mode = 1;
	}else if (mode == 0x2) {
		mode = 2;
	}else if (mode == 0x4) {
		mode = 3;
	}else if (mode == 0x0) {
		mode = 4;
	}
	return mode;
}

// 0~4 speed,5 auto,6 moon
void DYIRDaikin::setFan(uint8_t speed)
{
	uint8_t fan = vFanTable[speed];
	if (speed>=0 && speed <=6)
	{
		daikin[16] &= 0x0f;
		daikin[16] |= fan;
		checksum();
	}
}

uint8_t DYIRDaikin::getFan()
{
	uint8_t fan = (daikin[16] & 0xf0);
	if (fan == 0x30) {
		fan = 0;
	}else if (fan == 0x40) {
		fan = 1;
	}else if (fan == 0x50) {
		fan = 2;
	}else if (fan == 0x60) {
		fan = 3;
	}else if (fan == 0x70) {
		fan = 4;
	}else if (fan == 0xa0) {
		fan = 5;
	}else if (fan == 0xb0) {
		fan = 6;
	}
	return fan;
}

void DYIRDaikin::setTemp(uint8_t temp)
{
	if (temp >= 18 && temp<=32)
	{
		daikin[14] = (temp)*2;
		checksum();
	}
}

uint8_t DYIRDaikin::getTemp()
{
	uint8_t temperature= (daikin[14] & B01111110) >> 1;
	return temperature;
}

void DYIRDaikin::sendCommand()
{
      checksum();
      _irsend.sendDaikin(daikin, 8,0);
      delay(29);
      _irsend.sendDaikin(daikin, 19,8);
}
//
void DYIRDaikin::dump()
{
	uint8_t i;
	for (i=0; i < DYIRDAIKIN_COMMAND_LENGTH; i++) {
		Serial.print(daikin[i],HEX);
		Serial.print("-");
	}
}

void DYIRDaikin::description()
{
	//Serial.print(F("\r\n==send buffer==\r\n"));
	//Serial.print(F("Power:"));
	//Serial.print(getPower(),DEC);
	//Serial.println();
	//Serial.print(F("Mode:"));
	//Serial.print(getMode(),DEC);
	//Serial.println();
	//Serial.print(F("Fan:"));
	//Serial.print(getFan(),DEC);
	//Serial.println();
	//Serial.print(F("Temperature:"));
	//Serial.print(getTemp(),DEC);
	//Serial.println();
	//Serial.print(F("Swing:"));
	//Serial.print(getSwing(),DEC);
	//Serial.println();
	//Serial.print(F("SwingLR:"));
	//Serial.print(getSwingLR(),DEC);
	//Serial.println();
	_irrecv.descriptionARC(daikin + (sizeof(unsigned char)*8));
}

//private function
uint8_t DYIRDaikin::checksum()
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

uint8_t DYIRDaikin::decode() {
	if (_irrecv.decode() > 10) {
		receivedIRUpdateToSendBuffer(irReceiveData);
		return 1;
	}
	return 0;
}
//
void DYIRDaikin::receivedIRUpdateToSendBuffer(uint8_t *recvData) {
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
	uint8_t swingLR = (recvData[9] & 0x01);
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

	//{ 0x6,0x3,0x2,0x4,0x00};
	if (mode == 0x6) {
		mode = 0;
	}else if (mode == 0x3) {
		mode = 1;
	}else if (mode == 0x2) {
		mode = 2;
	}else if (mode == 0x4) {
		mode = 3;
	}else if (mode == 0x0) {
		mode = 4;
	}
	//
	uint8_t econo = (recvData[16] & B00000100) >> 2;
	//set all state
	setPower(powerState);
	setMode(mode);
	setFan(fan);
	setTemp(temperature);
	setSwing(swing);
	setSwingLR(swingLR);


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
