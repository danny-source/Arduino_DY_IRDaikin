
#include <DYIRDaikinRecv.h>

//decode
#define SAMPLE_DELAY_TIME 10//uS
#define IDLE_TIMER_COUNT ((1000*13)/SAMPLE_DELAY_TIME)//SAMPLE_DELAY_TIME*100*13
#define BUFFER_SIZE 310
#define NORMAL_INPUT_STATE 1
//
#define SIGNAL_TIMEOUT__COUNT (6000/SAMPLE_DELAY_TIME)
#define PACKET_TIMEOUT__COUNT (40000/SAMPLE_DELAY_TIME)

// 0:none,1:start,2:packet,3:stop,4 packet error,5:wake
#define SIGNAL_PATTERN_START 1
#define SIGNAL_PATTERN_PACKET 2
#define SIGNAL_PATTERN_STOP 3
#define SIGNAL_PATTERN_PACKET_ERROR 4
#define SIGNAL_PATTERN_WAKUP 5
//
#define SIGNAL_PAIRED	2

//
#define DEBUG_IR_PRINT
//~ #define DEBUG_IR_PRINT_DECODED_DATA

uint8_t DYIRDaikinRecv::begin(uint8_t pin,uint8_t *buffer,uint8_t buffer_size)
{
	if (buffer_size <24) {
		return 0;
	}
	irPin = pin;
	pinMode(irPin,INPUT);
	//pinMode(3,OUTPUT);
	//PORTD |= B00001000;
	irReceiveDataP0 = buffer;
	memset(irReceiveDataP0,0,buffer_size);
	hasPacket = 0;
	packetCounter = 0;
	signalCounter = 0;
	bitCounter = 0;
	irPatternStateMachine = 0;
	wakePatternCounter = 0;
	packetLength = 3;
	irState = irLastState = digitalRead(irPin);
	irReceiveDataLen = 0;
	receiveBufferBitPtr = 0;
	receiveBufferIndex = 0;
	return 1;
}


uint8_t DYIRDaikinRecv::decode() {
	return dumpPackets();
}
/*
 * 0:none 2:paired
*/
uint8_t DYIRDaikinRecv::isSignalLowHighPaired()
{
	irStateBufIdx = irSignalState;
	irStateBuf[irStateBufIdx] = irSignalState;
	irStateDurationBuf[irStateBufIdx] = irSignalDuation;
	if (irSignalState == 1) {
		return 2;
	}
	return 0;
}

// 0:none,1:start,2:packet,3:stop,4 packet error,5:wake
uint8_t DYIRDaikinRecv::decodePerPacket() {

	if (irPatternStateMachine == 0) {
		//detect 3 packets pattern that first have 4 zero pattern,it is 3 packets protocol
		if (isZeroMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
			wakePatternCounter++;
			if ((wakePatternCounter > 3)) {
				if (hasWakupPattern == 0) {
					DYIRDAIKIN_DEBUG_PRINTLN("wake");
					hasWakupPattern = 1;
					//PORTD &= B11110111;
				}else {
					//PORTD |= B00001000;
				}
				return SIGNAL_PATTERN_WAKUP;
			}
		}
		if (hasWakupPattern == 1) {
			if (isStopMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
				return (SIGNAL_PATTERN_STOP + SIGNAL_PATTERN_WAKUP);
			}
		}
		//first detect start pattern, it is 2 packets protocol
		if (isStartMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
			//DYIRDAIKIN_DEBUG_PRINTLN("SB");
			wakePatternCounter = 0;
			irPatternStateMachine = 1;
			packetLength = (hasWakupPattern == 1 ? 3: 2);
			receiveBufferBitPtr = 0;
			receiveBufferIndex = 0;
			memset(receiveBuffer,0,26);
			return SIGNAL_PATTERN_START;
		}
		return 0;
	}

	if (irPatternStateMachine == 1) {
		if (isStopMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
			irPatternStateMachine = 0;
			packetCounter++;
			return SIGNAL_PATTERN_STOP;
		}
		if (isZeroMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
			fillBitToByte(receiveBuffer, 0, &receiveBufferBitPtr, &receiveBufferIndex);
		}else if (isOneMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1){
			fillBitToByte(receiveBuffer, 1, &receiveBufferBitPtr, &receiveBufferIndex);
		}else {
			fillBitToByte(receiveBuffer, 1, &receiveBufferBitPtr, &receiveBufferIndex);
		}
		return SIGNAL_PATTERN_PACKET;
	}
	return 0;
}

uint8_t DYIRDaikinRecv::dumpPackets() {

  for(;;) {
	irState = digitalRead(irPin);
	//irState = (uint8_t)((PIND & B00010000) >> 4);
	if (irState == 1) {
		return 0;
	}else {
		break;
	}
  }
  //start sniffer
  signalCounter = 0;
  bitCounter = 0;
  duationCounter = 0;
  //
  signalTimeoutCounter = 0;
  packetTimeoutCounter = 0;
  //
  irLastState = irState = 0;

  //searching ir data
  uint8_t result = 0;
  while (1) {
    irState = digitalRead(irPin);
    //irState = (uint8_t)((PIND & B00010000) >> 4);
    if (irState != irLastState) {
		irSignalState = irLastState;
		irSignalDuation = duationCounter;
		signalCounter++;
		if (isSignalLowHighPaired() == SIGNAL_PAIRED) {
			bitCounter++;
			signalTimeoutCounter = 0;
			result = decodePerPacket();
			if (result == SIGNAL_PATTERN_PACKET_ERROR) {
				//packet is fail ,restart
				irPatternStateMachine = 0;
				DYIRDAIKIN_DEBUG_PRINTLN("packet detect is error,restart");
				DYIRDAIKIN_DEBUG_PRINT(irStateBuf[0],DEC);
				DYIRDAIKIN_DEBUG_PRINT(",");
				DYIRDAIKIN_DEBUG_PRINTLN(irStateDurationBuf[0],DEC);
				DYIRDAIKIN_DEBUG_PRINT(irStateBuf[1],DEC);
				DYIRDAIKIN_DEBUG_PRINT(",");
				DYIRDAIKIN_DEBUG_PRINTLN(irStateDurationBuf[1],DEC);
				return 0;
			}
			if (result == SIGNAL_PATTERN_WAKUP) {

			}
			if (result == SIGNAL_PATTERN_START) {
				signalCounter = 2;
				bitCounter = -1;
			}
			//
			//if ((signalCounter == 300) && (result == SIGNAL_PATTERN_PACKET)) {
				//DYIRDAIKIN_DEBUG_PRINT(irStateBuf[0],DEC);
				//DYIRDAIKIN_DEBUG_PRINT(",");
				//DYIRDAIKIN_DEBUG_PRINTLN(irStateDurationBuf[0],DEC);
				//DYIRDAIKIN_DEBUG_PRINT(irStateBuf[1],DEC);
				//DYIRDAIKIN_DEBUG_PRINT(",");
				//DYIRDAIKIN_DEBUG_PRINTLN(irStateDurationBuf[1],DEC);
			//}
		}
		//clear counterS
		duationCounter = 0;
		signalTimeoutCounter = 0;
		packetTimeoutCounter= 0;
    }

    if (signalTimeoutCounter > SIGNAL_TIMEOUT__COUNT) {
		if (irLastState == 1) {
			irSignalState = irLastState;
			irSignalDuation = duationCounter;
			signalCounter++;
			if (isSignalLowHighPaired() == SIGNAL_PAIRED) {
				bitCounter++;
				result = decodePerPacket();
				if (result == SIGNAL_PATTERN_STOP) {
					irReceiveDataLen = receiveBufferIndex + 1;
					uint8_t offsetIndex = 0;

					if (packetCounter == packetLength) {
						for (int idx = 0;idx < irReceiveDataLen;idx++) {
							irReceiveDataP0[idx] = receiveBuffer[idx];
						}
						if (checkSum(receiveBuffer,irReceiveDataLen) == 0) {
							DYIRDAIKIN_DEBUG_PRINTLN("CRC ERR");
						}
						//#ifdef DEBUG_IR_PRINT
							//DYIRDAIKIN_DEBUG_PRINTLN("=Decoded=");
							//DYIRDAIKIN_DEBUG_PRINT("scount:");
							//DYIRDAIKIN_DEBUG_PRINTLN(signalCounter,DEC);
							//DYIRDAIKIN_DEBUG_PRINT("bytes:");
							//DYIRDAIKIN_DEBUG_PRINTLN(irReceiveDataLen,DEC);
							//DYIRDAIKIN_DEBUG_PRINT("pcount:");
							//DYIRDAIKIN_DEBUG_PRINTLN(packetCounter,DEC);
							//DYIRDAIKIN_DEBUG_PRINTLN("--");
							//for (int idx = 0;idx < irReceiveDataLen;idx++) {
								//if (receiveBuffer[idx] < 16) {
									//DYIRDAIKIN_DEBUG_PRINT("0");
									//DYIRDAIKIN_DEBUG_PRINT(receiveBuffer[idx],HEX);
								//}else{
									//DYIRDAIKIN_DEBUG_PRINT(receiveBuffer[idx],HEX);
								//}
								//DYIRDAIKIN_DEBUG_PRINT("-");
							//}
							//DYIRDAIKIN_DEBUG_PRINTLN("");
							//if (checkSum(receiveBuffer,irReceiveDataLen) == 1) {
								//DYIRDAIKIN_DEBUG_PRINTLN("CRC OK");
							//}else {
								//DYIRDAIKIN_DEBUG_PRINTLN("CRC ERR");
							//}
							//DYIRDAIKIN_DEBUG_PRINTLN("--");
						//#endif
						//clear
						packetCounter = 0;
						hasWakupPattern = 0;
					}
					return irReceiveDataLen;

				}else if (result == (SIGNAL_PATTERN_STOP + SIGNAL_PATTERN_WAKUP)) {
					DYIRDAIKIN_DEBUG_PRINTLN("=WAKEUP+STOP=1=");
					return 0;
				}else {
					DYIRDAIKIN_DEBUG_PRINTLN("=Timeout=2=");
					return 0;
				}
			}else if (result == SIGNAL_PATTERN_WAKUP) {
				DYIRDAIKIN_DEBUG_PRINTLN("=WAKEUP=RETURN=2=");
				return 0;
			}else {
			#ifdef DEBUG_IR_PRINT
				DYIRDAIKIN_DEBUG_PRINTLN("=Timeout=1=");
			#endif
				hasWakupPattern = 0;
				return 0;
			}
			duationCounter = 0;
			signalCounter = 0;
		}
		signalTimeoutCounter = 0;
	}//signal timeout
	if (packetTimeoutCounter > PACKET_TIMEOUT__COUNT) {
		#ifdef DEBUG_IR_PRINT
			DYIRDAIKIN_DEBUG_PRINTLN("=Packet=Timeout=0=");
		#endif
		hasWakupPattern = 0;
		packetCounter = 0;
		return 0;
	}

    irLastState = irState;
	_delay_us(SAMPLE_DELAY_TIME);
	signalTimeoutCounter++;
	packetTimeoutCounter++;
    duationCounter++;
  //------------------------------------------------------

  }//while
    return 0;
}

uint8_t DYIRDaikinRecv::checkSum(uint8_t *buffer,uint8_t len)
{
	uint8_t sum = 0;
	for (uint8_t i =0;i< (len - 1);i++) {
		sum = (uint8_t)(sum + buffer[i]);
	}
	DYIRDAIKIN_DEBUG_PRINTLN();
	if (sum < 16) {
		DYIRDAIKIN_DEBUG_PRINT("0");
		DYIRDAIKIN_DEBUG_PRINT(sum,HEX);
	}else {
		DYIRDAIKIN_DEBUG_PRINT(sum,HEX);
	}
	DYIRDAIKIN_DEBUG_PRINT("-");
	if (buffer[len - 1] < 16) {
		DYIRDAIKIN_DEBUG_PRINT("0");
		DYIRDAIKIN_DEBUG_PRINT(buffer[len - 1],HEX);
	}else {
		DYIRDAIKIN_DEBUG_PRINT(buffer[len - 1],HEX);
	}
	DYIRDAIKIN_DEBUG_PRINTLN();
	if (buffer[len - 1] == sum) {
		return 1;
	}
	return 0;
}

void DYIRDaikinRecv::fillBitToByte(uint8_t *buffer, uint8_t value, uint8_t *bitPtr, uint8_t *bufferIndex) {
	uint8_t _bitPtr;
	uint8_t _bufferIndex;
	_bitPtr = *bitPtr;
	_bufferIndex = *bufferIndex;
	if (_bitPtr > 7) {
		_bufferIndex++;
		_bitPtr = 0;
	}else {
		buffer[_bufferIndex] = buffer[_bufferIndex] >> 1;
	}
	if (value == 1) {
		buffer[_bufferIndex] = buffer[_bufferIndex] | B10000000;
	}
	if (value == 0) {
		buffer[_bufferIndex] = buffer[_bufferIndex] & B01111111;
	}
	_bitPtr++;
	*bitPtr = _bitPtr;
	*bufferIndex = _bufferIndex;
}
//
void DYIRDaikinRecv::descriptionARC(uint8_t *recvData) {
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

	uint8_t swing = (recvData[8] & 0x0f) >> 3;
	uint8_t swingLR = (recvData[9] & 0x0f) >> 3;
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

	//{ 0x6,0x3,0x2,0x4,0x00};
	//0 FAN 1 COOL 2 DRY 3 HEAT
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

	uint8_t econo = (recvData[16] & B00000100) >> 2;

	DYIRDAIKIN_DEBUG_PRINT("\r\n===\r\n");
	DYIRDAIKIN_DEBUG_PRINT("Power:");
	DYIRDAIKIN_DEBUG_PRINT(powerState,DEC);
	DYIRDAIKIN_DEBUG_PRINTLN();
	DYIRDAIKIN_DEBUG_PRINT("Mode:");
	DYIRDAIKIN_DEBUG_PRINT(mode,DEC);
	if (mode == 0) {
		DYIRDAIKIN_DEBUG_PRINT(" (FAN");
	}
	if (mode == 1) {
		DYIRDAIKIN_DEBUG_PRINT(" (COOL");
	}
	if (mode == 2) {
		DYIRDAIKIN_DEBUG_PRINT(" (DRY");
	}
	if (mode == 3) {
		DYIRDAIKIN_DEBUG_PRINT(" (HEAT");
	}
	if (mode == 4) {
		DYIRDAIKIN_DEBUG_PRINT(" (AUTO");
	}

	DYIRDAIKIN_DEBUG_PRINTLN();
	DYIRDAIKIN_DEBUG_PRINT("Fan:");
	DYIRDAIKIN_DEBUG_PRINT(fan,DEC);
	DYIRDAIKIN_DEBUG_PRINTLN();
	DYIRDAIKIN_DEBUG_PRINT("Temperature:");
	DYIRDAIKIN_DEBUG_PRINT(temperature,DEC);
	DYIRDAIKIN_DEBUG_PRINTLN();
	DYIRDAIKIN_DEBUG_PRINT("Swing:");
	DYIRDAIKIN_DEBUG_PRINT(swing,DEC);
	DYIRDAIKIN_DEBUG_PRINTLN();
	DYIRDAIKIN_DEBUG_PRINT("SwingLR:");
	DYIRDAIKIN_DEBUG_PRINT(swingLR,DEC);
	DYIRDAIKIN_DEBUG_PRINTLN();
	//~ DYIRDAIKIN_DEBUG_PRINT("Econo:");
	//~ DYIRDAIKIN_DEBUG_PRINT(econo,DEC);
	//~ DYIRDAIKIN_DEBUG_PRINTLN();
	//~ DYIRDAIKIN_DEBUG_PRINT("Timer On:");
	//~ DYIRDAIKIN_DEBUG_PRINT(timerOn,DEC);
	//~ DYIRDAIKIN_DEBUG_PRINTLN();
	//~ DYIRDAIKIN_DEBUG_PRINT("Timer On Value:");
	//~ DYIRDAIKIN_DEBUG_PRINT((timerOnValue / 60),DEC);
	//~ DYIRDAIKIN_DEBUG_PRINT(":");
	//~ DYIRDAIKIN_DEBUG_PRINT((timerOnValue % 60),DEC);
	//~ DYIRDAIKIN_DEBUG_PRINTLN();
	//~ DYIRDAIKIN_DEBUG_PRINT("Timer Off:");
	//~ DYIRDAIKIN_DEBUG_PRINT(timerOff,DEC);
	//~ DYIRDAIKIN_DEBUG_PRINTLN();
	//~ DYIRDAIKIN_DEBUG_PRINT("Timer Off Value:");
	//~ DYIRDAIKIN_DEBUG_PRINT((timerOffValue / 60),DEC);
	//~ DYIRDAIKIN_DEBUG_PRINT(":");
	//~ DYIRDAIKIN_DEBUG_PRINT((timerOffValue % 60),DEC);
	//~ DYIRDAIKIN_DEBUG_PRINTLN();
	//~ DYIRDAIKIN_DEBUG_PRINT("Timer Now:");
	//~ DYIRDAIKIN_DEBUG_PRINT((timeNow / 60),DEC);
	//~ DYIRDAIKIN_DEBUG_PRINT(":");
	//~ DYIRDAIKIN_DEBUG_PRINT((timeNow % 60),DEC);
	//~ DYIRDAIKIN_DEBUG_PRINTLN();
}

uint8_t DYIRDaikinRecv::isOneMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{
	if ((lowTimeCounter > 15 && lowTimeCounter < 56) && ((highTimecounter) >= (lowTimeCounter + lowTimeCounter)  && highTimecounter < 150)) {
		return 1;
	}
	return 0;
}

uint8_t DYIRDaikinRecv::isZeroMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{

	if ((lowTimeCounter > 15 && lowTimeCounter < 60) && (highTimecounter >= 10 && highTimecounter < 58)) {
		return 1;
	}
	return 0;
}

uint8_t DYIRDaikinRecv::isStartMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{
	if ((lowTimeCounter > 50 && lowTimeCounter < 450) && (highTimecounter > 70  && highTimecounter < 250)) {
		return 1;
	}
	return 0;
}

uint8_t DYIRDaikinRecv::isStopMatched(uint16_t lowTimeCounter,uint16_t highTimecounter)
{
	if ((lowTimeCounter > 20 && lowTimeCounter < 500) && (highTimecounter > 200)) {
		return 1;
	}
	return 0;
}
