
#include <DYIRDaikinRecv.h>

//SAMPLE_DELAY_TIME US
#if (defined(__AVR_ATmega328P__) | defined(__AVR_ATmega8__) | defined(__AVR_ATmega8P__) | defined(__AVR_ATmega32U4__))
#define SAMPLE_DELAY_TIME (6)
#elif defined(__AVR_ATmega2560__)
#define SAMPLE_DELAY_TIME (2)
#else
#define SAMPLE_DELAY_TIME (10)
#endif

#define IDLE_TIMER_COUNT ((1000*13)/SAMPLE_DELAY_TIME)
#define BUFFER_SIZE (310)
#define NORMAL_INPUT_STATE (1)
//
#define SIGNAL_TIMEOUT__COUNT (6000/SAMPLE_DELAY_TIME)
#define PACKET_TIMEOUT__COUNT (40000/SAMPLE_DELAY_TIME)

// 0:none,1:start,2:packet,3:stop,4 packet error,5:wake
#define SIGNAL_PATTERN_START 		(B00000001)
#define SIGNAL_PATTERN_PACKET 		(B00000010)
#define SIGNAL_PATTERN_STOP  		(B00000100)
#define SIGNAL_PATTERN_WAKEUP 		(B00001000)
#define SIGNAL_PATTERN_PACKET_ERROR (B00010000)
//
#define SIGNAL_PAIRED	(2)

//
#define DYIRDAIKIN_DEBUG_STATE
#define DYIRDAIKIN_DEBUG_PRINT_RAW_BYTES
//FOR MEASURE LOOP TIME
//if define,clock wave output at D3

//#define DYIRDAIKIN_MEASURE_LOOP_TIME

#ifdef DYIRDAIKIN_MEASURE_LOOP_TIME
#if (defined(__AVR_ATmega328P__) | defined(__AVR_ATmega8__) | defined(__AVR_ATmega8P__))
#define DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
#define DYIRDAIKIN_MEASURE_LOOP_TIME_HIGH PORTD = PORTD | B00001000;
#define DYIRDAIKIN_MEASURE_LOOP_TIME_LOW  PORTD = PORTD & B11110111;
#elif defined(__AVR_ATmega32U4__)
#warning "__AVR_ATmega32U4__"
#define DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
#define DYIRDAIKIN_MEASURE_LOOP_TIME_HIGH PORTD = PORTD | B00000001;
#define DYIRDAIKIN_MEASURE_LOOP_TIME_LOW  PORTD = PORTD & B11111110;
#elif defined(__AVR_ATmega2560__)
#warning "__AVR_ATmega2560__"
#define DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
#define DYIRDAIKIN_MEASURE_LOOP_TIME_HIGH PORTE = PORTE | B00100000;
#define DYIRDAIKIN_MEASURE_LOOP_TIME_LOW  PORTE = PORTE & B11011111;
#elif defined(ARDUINO_AMEBA)
#define DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
#define DYIRDAIKIN_MEASURE_LOOP_TIME_HIGH digitalWrite(3,HIGH);
#define DYIRDAIKIN_MEASURE_LOOP_TIME_LOW  digitalWrite(3,LOW);
#else
#define DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
#define DYIRDAIKIN_MEASURE_LOOP_TIME_HIGH digitalWrite(3,HIGH);
#define DYIRDAIKIN_MEASURE_LOOP_TIME_LOW  digitalWrite(3,LOW);
#endif
#endif


uint8_t DYIRDaikinRecv::begin(uint8_t pin,uint8_t *buffer,uint8_t buffer_size)
{
	if (buffer_size <24) {
		return 0;
	}
	irPin = pin;
	pinMode(irPin,INPUT);
	bitMask = B00000001;
	bitMask = bitMask << (uint8_t)irPin;
	#ifdef DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
	pinMode(3,OUTPUT);
	#endif
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
	irStateDurationBuf[irStateBufIdx] = irSignalDuation;
	#ifdef DYIRDAIKIN_DEBUG_PRINT_SIGNAL_DUATION
	if (signalCounter < 310) {
		irStateDebugBuf[irStateDebugIdx] = irSignalState;
		irStateDurationDebugBuf[irStateDebugIdx] = irSignalDuation;
		irStateDebugIdx++;
	}
	#endif
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
					#ifdef DYIRDAIKIN_DEBUG_STATE
					DYIRDAIKIN_DEBUG_PRINTLN("wake");
					#endif
					hasWakupPattern = 1;
				}
				return SIGNAL_PATTERN_WAKEUP;
			}
		}
		if (hasWakupPattern == 1) {
			if (isStopMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
				//end of wake pattern
				return (SIGNAL_PATTERN_STOP | SIGNAL_PATTERN_WAKEUP);
			}
		}
		//first detect start pattern, it is 2 packets protocol
		if (isStartMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
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
			wakePatternCounter = 0;
			packetCounter++;
			endTime = millis();
			startTime = endTime;
			return SIGNAL_PATTERN_STOP;
		}
		if (isZeroMatched(irStateDurationBuf[0],irStateDurationBuf[1]) == 1) {
			fillBitToByte(receiveBuffer, 0, &receiveBufferBitPtr, &receiveBufferIndex);
		}else {
			fillBitToByte(receiveBuffer, 1, &receiveBufferBitPtr, &receiveBufferIndex);
		}
		return SIGNAL_PATTERN_PACKET;
	}
	return 0;
}

uint8_t DYIRDaikinRecv::dumpPackets() {

  for(;;) {
	  irState = readIR(irPin);
	if (irState != irLastState) {
		if ((irLastState == 1) && (irState == 0)) {
			startTime = millis();//reset big timeout
			endTime = startTime;
			break;
		}
	}else {
		if (packetCounter > 0) {
			endTime = millis();
			if ((endTime - startTime) > 80) {
			  //big timeout need reset packetCounter, sometimes packet is not completed
			  hasWakupPattern = 0;
			  packetCounter = 0;
			  DYIRDAIKIN_DEBUG_PRINTLN("detect packet is timeout,restart");
			}
		}
	}
	irLastState = irState;
	return 0;
  }
  signalCounter = 0;
  bitCounter = 0;
  duationCounter = 0;
  //
  signalTimeoutCounter = 0;
  packetTimeoutCounter = 0;

  irLastState = irState = 0;
//parse IR signal
  uint8_t result = B00000000;
	#ifdef DYIRDAIKIN_DEBUG_PRINT_SIGNAL_DUATION
	memset(irStateDebugBuf,0,310);
	memset(irStateDurationDebugBuf,0,310);
	irStateDebugIdx = 0;
	#endif
  while (1) {
	#ifdef DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
	DYIRDAIKIN_MEASURE_LOOP_TIME_LOW
	DYIRDAIKIN_MEASURE_LOOP_TIME_HIGH
	#endif
	  irState = readIR(irPin);
    if (irState != irLastState) {
		irSignalState = irLastState;
		irSignalDuation = duationCounter;
		signalCounter++;
		if (isSignalLowHighPaired() == SIGNAL_PAIRED) {
			bitCounter++;
			signalTimeoutCounter = 0;
			result = decodePerPacket();
			if ((result & SIGNAL_PATTERN_PACKET_ERROR) == SIGNAL_PATTERN_PACKET_ERROR) {
				//packet is fail ,restart
				irPatternStateMachine = 0;
				DYIRDAIKIN_DEBUG_PRINTLN("packet detect is error,restart");
				hasWakupPattern = 0;
				packetCounter = 0;
				break;
			}
			if ((result & SIGNAL_PATTERN_WAKEUP) == SIGNAL_PATTERN_WAKEUP) {
				duationCounter = 0;
				signalCounter = 0;
				signalTimeoutCounter = 0;
				packetCounter = 0;
				break;
			}
			if ((result & SIGNAL_PATTERN_START) == SIGNAL_PATTERN_START) {
				signalCounter = 2;
				bitCounter = -1;
			}
			//
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
				if (result & SIGNAL_PATTERN_STOP) {
					irReceiveDataLen = receiveBufferIndex + 1;
					uint8_t offsetIndex = 0;
					for (int idx = 0;idx < irReceiveDataLen;idx++) {
						irReceiveDataP0[idx] = receiveBuffer[idx];
					}
					#ifdef DYIRDAIKIN_DEBUG_PRINT_RAW_BYTES
						DYIRDAIKIN_DEBUG_PRINTLN("=Decoded=");
						DYIRDAIKIN_DEBUG_PRINT("scount:");
						DYIRDAIKIN_DEBUG_PRINTLN(signalCounter,DEC);
						DYIRDAIKIN_DEBUG_PRINT("bytes:");
						DYIRDAIKIN_DEBUG_PRINTLN(irReceiveDataLen,DEC);
						DYIRDAIKIN_DEBUG_PRINT("pcount:");
						DYIRDAIKIN_DEBUG_PRINTLN(packetCounter,DEC);
						DYIRDAIKIN_DEBUG_PRINT("plength:");
						DYIRDAIKIN_DEBUG_PRINTLN(packetLength,DEC);
						DYIRDAIKIN_DEBUG_PRINTLN("--");
						for (int idx = 0;idx < irReceiveDataLen;idx++) {
							if (receiveBuffer[idx] < 16) {
								DYIRDAIKIN_DEBUG_PRINT("0");
								DYIRDAIKIN_DEBUG_PRINT(receiveBuffer[idx],HEX);
							}else{
								DYIRDAIKIN_DEBUG_PRINT(receiveBuffer[idx],HEX);
							}
							DYIRDAIKIN_DEBUG_PRINT("-");
						}
						DYIRDAIKIN_DEBUG_PRINTLN("");
						DYIRDAIKIN_DEBUG_PRINTLN("--");
					#endif
					if (checkSum(receiveBuffer,irReceiveDataLen) == 0) {
						packetCounter = 0;
						DYIRDAIKIN_DEBUG_PRINTLN("-check sum error-restart");
						break;
					}
					if (packetCounter == packetLength) {
						//last packet and data length need than 8 bytes
						if (irReceiveDataLen <= 8) {
							DYIRDAIKIN_DEBUG_PRINTLN("=data length error=");
							hasWakupPattern = 0;
							packetCounter = 0;
							wakePatternCounter = 0;;
							break;
						}
						DYIRDAIKIN_DEBUG_PRINTLN("=data packet=");
						#ifdef DYIRDAIKIN_DEBUG_PRINT_SIGNAL_DUATION
						for (int idx = 0;idx < 310;idx++) {
							if (irStateDurationDebugBuf[idx] == 0) {
								continue;
							}
							if (irStateDebugBuf[idx] == 0) {
								DYIRDAIKIN_DEBUG_PRINT(idx,DEC);
								DYIRDAIKIN_DEBUG_PRINT("[");
							}
							DYIRDAIKIN_DEBUG_PRINT(irStateDebugBuf[idx],DEC);
							DYIRDAIKIN_DEBUG_PRINT(",");
							DYIRDAIKIN_DEBUG_PRINT(irStateDurationDebugBuf[idx],DEC);
							if (irStateDebugBuf[idx] == 0) {
								DYIRDAIKIN_DEBUG_PRINT("-");
							}
							if (irStateDebugBuf[idx] == 1) {
								DYIRDAIKIN_DEBUG_PRINTLN("]");
							}
						}
						DYIRDAIKIN_DEBUG_PRINT("---");
						#endif
						//clear
						hasWakupPattern = 0;
						packetCounter = 0;
						#ifdef DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
						DYIRDAIKIN_MEASURE_LOOP_TIME_LOW
						#endif
						return irReceiveDataLen;
					}
					DYIRDAIKIN_DEBUG_PRINTLN("=PSP=");
				}else if (((result & SIGNAL_PATTERN_STOP))
								&& (result & SIGNAL_PATTERN_WAKEUP)) {
						DYIRDAIKIN_DEBUG_PRINTLN("=WAKEUP+STOP=1=");
						break;
				}else {
					DYIRDAIKIN_DEBUG_PRINTLN("=Timeout=2=restart");
					hasWakupPattern = 0;
					packetCounter = 0;
					break;
				}
			}else if ((result & SIGNAL_PATTERN_WAKEUP)) {
				#ifdef DYIRDAIKIN_DEBUG_STATE
				DYIRDAIKIN_DEBUG_PRINTLN("=WAKEUP=RET=2=");
				#endif
				break;
			}else {
			#ifdef DYIRDAIKIN_DEBUG_STATE
				DYIRDAIKIN_DEBUG_PRINTLN("=Timeout=1=restart");
			#endif
				hasWakupPattern = 0;
				break;
			}
			duationCounter = 0;
			signalCounter = 0;
		}
		signalTimeoutCounter = 0;
		receiveBufferIndex = 0;
		break;//###//
	}//signal timeout
	if (packetTimeoutCounter > PACKET_TIMEOUT__COUNT) {
		#ifdef DYIRDAIKIN_DEBUG_STATE
			DYIRDAIKIN_DEBUG_PRINTLN("=Detect Packet=Timeout=restart");
		#endif
		hasWakupPattern = 0;
		packetCounter = 0;
		//sometimes overfollow
		bitMask = B00000001;
		bitMask = bitMask << (uint8_t)irPin;
		receiveBufferIndex = 0;
		#ifdef DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
		DYIRDAIKIN_MEASURE_LOOP_TIME_LOW
		#endif
		return 0;
	}

    irLastState = irState;
	_delay_us(SAMPLE_DELAY_TIME);
	signalTimeoutCounter++;
	packetTimeoutCounter++;
    duationCounter++;
  //------------------------------------------------------
	#ifdef DYIRDAIKIN_MEASURE_LOOP_TIME_OUTPUT
	DYIRDAIKIN_MEASURE_LOOP_TIME_LOW
	#endif
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
	if (buffer[len - 1] == sum) {
		DYIRDAIKIN_DEBUG_PRINTLN(" CRC OK");
		return 1;
	}
	DYIRDAIKIN_DEBUG_PRINTLN(" CRC ERR");
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

	DYIRDAIKIN_DEBUG_PRINT(F("\r\n===\r\n"));
	DYIRDAIKIN_DEBUG_PRINT(F("Power:"));
	DYIRDAIKIN_DEBUG_PRINT(powerState,DEC);
	DYIRDAIKIN_DEBUG_PRINTLN();
	DYIRDAIKIN_DEBUG_PRINT(F("Mode:"));
	DYIRDAIKIN_DEBUG_PRINT(mode,DEC);
	if (mode == 0) {
		DYIRDAIKIN_DEBUG_PRINT(F(" (FAN"));
	}
	if (mode == 1) {
		DYIRDAIKIN_DEBUG_PRINT(F(" (COOL"));
	}
	if (mode == 2) {
		DYIRDAIKIN_DEBUG_PRINT(F(" (DRY"));
	}
	if (mode == 3) {
		DYIRDAIKIN_DEBUG_PRINT(F(" (HEAT"));
	}
	if (mode == 4) {
		DYIRDAIKIN_DEBUG_PRINT(F(" (AUTO"));
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
	if ((lowTimeCounter > 15 && lowTimeCounter < 60) && ((highTimecounter) >= (lowTimeCounter + lowTimeCounter)  && highTimecounter < 150)) {
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
