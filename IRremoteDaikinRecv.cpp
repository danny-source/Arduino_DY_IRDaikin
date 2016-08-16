/*
 * Arduino IRremote Daikin 2015
 * Copyright 2015 danny
 *
 *
 * enableIROut declare base on  Ken Shirriff's IRremote library.
 * http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 *
 */
#include <IRremoteDaikinRecv.h>

#ifndef IRremoteintDaikinESP_h
#include <util/delay.h>
#else
#define _delay_us delayMicroseconds
#endif
//decode
#define SAMPLE_DELAY_TIME 10//uS
#define IDLE_TIMER_COUNT ((1000*13)/SAMPLE_DELAY_TIME)//SAMPLE_DELAY_TIME*100*13
#define BUFFER_SIZE 310
//
//~ #define DEBUG_IR_PRINT
//~ #define DEBUG_IR_PRINT_DECODED_DATA

	uint8_t irPin = 7;
//
	uint8_t 	irState = 1;
	uint8_t 	irLastState = 1;
	uint16_t 	stopBitCounter = 0;
	uint16_t 	timeoutCounter = 0; //about 120ms
	uint16_t 	timeCounter =0;
	uint16_t 	packetCounter =0;
	uint8_t	packetLength = 3;
	uint8_t 	packetNumber = 0;
	uint8_t 	irStateBuf[2]= {0};
	uint8_t	hasPacket = 0;
	unsigned long irDurationBuf[2] = {0};
	uint16_t 	idx = 0;
	uint16_t 	bitCounter = 0;
	uint8_t	irPatternStateMachine = 0;
	uint8_t	irRawSateMachine = 0;
	uint8_t	wakePatternCounter = 0;
	uint8_t	*irReceiveDataP0;
	uint8_t	irReceiveDataLen = 0;
//
uint8_t IRDaikinRecv::begin(uint8_t pin,uint8_t *buffer,uint8_t buffer_size)
{
	if (buffer_size <24) {
		return 0;
	}
	irPin = pin;
	pinMode(irPin,INPUT);
	irReceiveDataP0 = buffer;
	memset(irReceiveDataP0,0,buffer_size);
	hasPacket = 0;
	packetCounter =0;
	bitCounter = 0;
	irPatternStateMachine = 0;
	packetCounter = 0;
	wakePatternCounter = 0;
	packetLength = 3;
	irState = irLastState = digitalRead(irPin);
	irReceiveDataLen = 0;
	return 1;
}

uint8_t IRDaikinRecv::decode() {

  // put your main code here, to run repeatedly:
  for (;;) {
    irState = digitalRead(irPin);
    if (irState!=irLastState) {
      break;
    }
    irLastState = irState;
    return 0;//polling
  }
  //start sniffer
  timeCounter = 1;
  stopBitCounter = 0;
  timeoutCounter = 0;
  hasPacket = 0;
  irLastState = irState;
  idx = 0;
  //searching ir data
  //~ cli();
  while (1) {
    _delay_us(SAMPLE_DELAY_TIME);
    irState = digitalRead(irPin);
    //
    if (irState!=irLastState) {
		if (irDataStoreBuffer() == 2) {
	      decodeIR(false);
		}
      timeCounter = 0;
      stopBitCounter = 0;
    }//if (irState!=irLastState) {
    //
    stopBitCounter ++;
    timeCounter++;
    timeoutCounter;
    irLastState = irState;
    //
    if (stopBitCounter > IDLE_TIMER_COUNT) {
		if (irDataStoreBuffer() == 2) {
			if (decodeIR(true) > 0) {
				if (checkSum(irReceiveDataP0,irReceiveDataLen) == 0) {
					#ifdef DEBUG_IR_PRINT
						Serial.println("=CRC Fail=");
						hasPacket = 1;
						break;
					#endif
				}else {
					hasPacket = 1;
					break;
				}
			}else {
				#ifdef DEBUG_IR_PRINT
				Serial.println("=Fail=");
				#endif
			}
		}
		hasPacket = 0;
		packetCounter =0;
		bitCounter = 0;
		irPatternStateMachine = 0;
		packetCounter = 0;
		wakePatternCounter = 0;
		packetLength = 3;
		irState = irLastState = digitalRead(irPin);
		memset(irReceiveDataP0,0,25);
		irReceiveDataLen = 0;
		break;
    }
  //------------------------------------------------------

  }//while
  //~ sei();
	if (hasPacket == 1) {//default 3
		#ifdef DEBUG_IR_PRINT
		Serial.println();
        for (uint16_t i = 0;i < 25; i++) {
			Serial.print(irReceiveDataP0[i],HEX);
			Serial.print(" ");
        }
        Serial.println();
        #endif
        #ifdef DEBUG_IR_PRINT_DECODED_DATA
        if (irReceiveDataLen >10) {
	        printARCState(irReceiveDataP0);
		}
		#endif
		hasPacket = 0;
        bitCounter = 0;
        irPatternStateMachine = 0;
        packetCounter = 0;
        wakePatternCounter = 0;
        packetCounter =0;
        return irReceiveDataLen;
    }
    return 0;
}

/*
 * 0:none 1:store to buffer 2:pair store to buffer
*/
uint8_t IRDaikinRecv::irDataStoreBuffer()
{
	if (irRawSateMachine == 0) {
		if (irLastState == 0) {
			irStateBuf[0] = irLastState;
			irDurationBuf[0] = timeCounter;
			irRawSateMachine = 1;
			return 1;
		}
		return 0;
	}
	if ( irRawSateMachine == 1) {
		if (irLastState == 1) {
			irStateBuf[1] = irLastState;
			irDurationBuf[1] = timeCounter;
			irRawSateMachine = 0;
			return 2;
		}else {
			irStateBuf[0] = irLastState;
			irDurationBuf[0] = timeCounter;
			irRawSateMachine = 1;
			return 1;
		}
		return 0;
	}
}

uint8_t IRDaikinRecv::decodeIR(bool lastBit) {

	if (irPatternStateMachine == 0) {
		//~ skip wake up pattern
		if (isZeroMatched(irDurationBuf[0],irDurationBuf[1]) == 1) {
			wakePatternCounter++;
		}
		if (lastBit == true) {
			if (wakePatternCounter > 4) {
				irPatternStateMachine = 1;
				wakePatternCounter = 0;
				packetLength = 3;
				packetCounter = -1;
				irReceiveDataLen = 0;
				memset(irReceiveDataP0,0,25);
				return 1;
			}
			return 0;
		}
		//start bit
		if (isStartMatched(irDurationBuf[0],irDurationBuf[1]) == 1) {
			//some model no wake up pattern and only 2 packet
			packetLength = 1;
			irPatternStateMachine = 2;
			irReceiveDataLen = 0;
			bitToByteBuffer(irReceiveDataP0,0,1,NULL);
			memset(irReceiveDataP0,0,25);
			return 1;
		}else {
			return 0;
		}
	}
	//
	if (irPatternStateMachine == 1) {//start bit
	  //~ skipe start bit
	  if (isStartMatched(irDurationBuf[0],irDurationBuf[1]) == 1) {
		irPatternStateMachine = 2;
		bitToByteBuffer(irReceiveDataP0,0,1,NULL);
		return 1;
	  }
	  return 0;
	}
	if (irPatternStateMachine == 2) {//packet number 1
		if (lastBit == true){
			if (isStopMatched(irDurationBuf[0],irDurationBuf[1]) == 1) {
				irPatternStateMachine = 0;
				packetLength++;
				return 1;
			}else {
				irPatternStateMachine = 0;
				return 0;
			}
		}
		if (isZeroMatched(irDurationBuf[0],irDurationBuf[1]) == 1) {
			bitToByteBuffer(irReceiveDataP0,0,0,&irReceiveDataLen);
		}else {
			bitToByteBuffer(irReceiveDataP0,1,0,&irReceiveDataLen);
		}
		return 0;
	}
}

uint8_t IRDaikinRecv::checkSum(uint8_t *buffer,uint8_t len)
{
	uint8_t sum = 0;
	for (uint8_t i =0;i< (len - 1);i++) {
		sum = (uint8_t)(sum + buffer[i]);
	}
	//~ Serial.println();
	//~ Serial.print(sum,HEX);
	//~ Serial.print("-");
	//~ Serial.print(buffer[len],HEX);
	//~ Serial.println();
	if (buffer[len] == sum) {
		return 1;
	}
	return 0;
}

void IRDaikinRecv::bitToByteBuffer(uint8_t *buffer, uint8_t value, int restart,uint8_t *bufferPtr) {
	static uint8_t bitIndex = 0;
	static uint8_t bufferIndex = 0;
	//~ Serial.print(value,DEC);
	if (restart == 1) {
		bitIndex = 0;
		bufferIndex = 0;
		return;
	}
	if (bitIndex>7) {
		bufferIndex++;
		bitIndex = 0;
	}else {
		buffer[bufferIndex] = buffer[bufferIndex] >> 1;
	}
	if (value == 1) {
		buffer[bufferIndex] = buffer[bufferIndex]|0x80;
	}
	if (value == 0) {

	}
	bitIndex++;
	if (!(bufferPtr == NULL)) {
		*bufferPtr = bufferIndex;
	}
}
//
void IRDaikinRecv::printARCState(uint8_t *recvData) {
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

	uint8_t swing = (recvData[8] & 0x0f) >> 4;
	uint8_t powerState =  (recvData[5] & 0x01);
	uint8_t timerOn =  (recvData[5] & 0x02) >> 1;
	uint16_t timerOnValue = (uint16_t)recvData[10]|(uint16_t)(recvData[11] & B00000111)<<8;
	//~ AAAAAAAA AAAXBBBB BBBBBBBX
	uint8_t timerOff =  (recvData[5] & 0x04) >> 2;
	uint16_t timerOffValue = (uint16_t)((recvData[11] & B11110000) >> 4)|(uint16_t)(recvData[12] & B01111111)<<4;
	uint8_t mode = (recvData[5] & B01110000) >> 4;
	//
	uint16_t timeNow = 0;
	if (packetLength == 3) {
		timeNow = (uint16_t)recvData[5]|(uint16_t)(recvData[6] & B00000111)<<8;
	}

	//~ static byte vModeTable[] = { 0x6,0x3,0x2};
	if (mode == 0x6) mode = 0;
	if (mode == 0x3) mode = 1;
	if (mode == 0x2) mode = 2;

	uint8_t econo = (recvData[16] & B00000100) >> 2;

	Serial.print("\r\n===\r\n");
	Serial.print("Power:");
	Serial.print(powerState,DEC);
	Serial.println();
	Serial.print("Mode:");
	Serial.print(mode,DEC);
	Serial.println();
	Serial.print("Fan:");
	Serial.print(fan,DEC);
	Serial.println();
	Serial.print("Temperature:");
	Serial.print(temperature,DEC);
	Serial.println();
	Serial.print("Swing:");
	Serial.print(swing,DEC);
	Serial.println();
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

uint8_t IRDaikinRecv::isOneMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{
	if ((lowTimeCounter > 15 && lowTimeCounter < 50) && (highTimecounter > (lowTimeCounter + lowTimeCounter)  && highTimecounter < 200)) {
		return 1;
	}
	return 0;
}

uint8_t IRDaikinRecv::isZeroMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{

	if ((lowTimeCounter > 15 && lowTimeCounter < 50) && (highTimecounter > 15 && highTimecounter < 50)) {
		return 1;
	}
	return 0;
}

uint8_t IRDaikinRecv::isStartMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{
	if ((lowTimeCounter > 100 && lowTimeCounter < 400) && (highTimecounter > 70  && highTimecounter < 250)) {
		return 1;
	}
	return 0;
}

uint8_t IRDaikinRecv::isStopMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{
	if ((lowTimeCounter > 15 && lowTimeCounter < 50) && (highTimecounter > 200)) {
		return 1;
	}
	return 0;
}
