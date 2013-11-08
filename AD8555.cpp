//
//    FILE: AD8555.cpp
// AUTHORS : Ilya Volkov, Maxim Dmitrov
// VERSION: 0.1.01
//    DATE: 2013-11-07
//
// HISTORY:
// 0.1.00 - 2013-11-07 initial version
// 0.1.01 - 2013-11-07 refactored 
//
// Released to the public domain
//

#include "AD8555.h"

AD8555::AD8555(uint8_t outputPin)
{
    outPin = outputPin;
    digitalWrite(outPin, LOW);
    pinMode(outPin, OUTPUT);
}

bool AD8555::setSecondStageGain(uint8_t value)
{
    if (value > 7) return false;
    SSG = value;
    return true;
}

bool AD8555::setFirstStageGain(uint8_t value)
{
    if (value > 127) return false;
    FSG = value;
    return true;
}

bool AD8555::setOffset(uint8_t value)
{
    OFS = value;
    return true;
}

void AD8555::simulate()
{
    sendData(FUNC_SIMULATE, PAR_SSG_CODE, SSG);
    sendData(FUNC_SIMULATE, PAR_FSG_CODE, FSG);
    sendData(FUNC_SIMULATE, PAR_OFS_CODE, OFS);
}

void AD8555::sendData(uint8_t mode, uint8_t func, uint8_t value)
{
    sendStart();
    sendMode(mode);
    sendFunc(func);
    sendDummy();
    sendValue(value);
    sendStop();
}


void AD8555::program()
{
    uint8_t parity;

	for (uint8_t i = 0; i < 8; i++) {
		if ((SSG >> i) & 1) {
			parity = (1 << i);
			blowSSGFuse(parity);
		}
	}

	for (uint8_t i = 0; i < 8; i++) {
		if ((FSG >> i) & 1) {
			parity = (1 << i);
			blowFSGFuse(parity);
		}
	}

	for (uint8_t i = 0; i < 8; i++) {
		if ((OFS >> i) & 1) {
			parity = (1 << i);
			blowOFSFuse(parity);
		}
	}

	blowMSF();
}

void AD8555::blowSSGFuse(uint8_t Value)
{
    sendData(FUNC_PROGRAM, PAR_SSG_CODE, Value);
    delayMicroseconds(1100);
}

void AD8555::blowFSGFuse(uint8_t Value)
{
    sendData(FUNC_PROGRAM, PAR_FSG_CODE, Value);
    delayMicroseconds(1100);
}

void AD8555::blowOFSFuse(uint8_t Value)
{
    sendData(FUNC_PROGRAM, PAR_OFS_CODE, Value);
    delayMicroseconds(1100);
}

void AD8555::blowMSF()
{
    sendData(FUNC_PROGRAM, PAR_OTHER_FUNC, 1);
    delayMicroseconds(1100);
}

void AD8555::sendStart()
{
    sendBit(1);
    for (uint8_t i = 0; i < 10; i++) {
		sendBit(0);
	}
    sendBit(1);
}

void AD8555::sendMode(uint8_t value)
{
	sendBit((value >> 1) & 1);
	sendBit(value & 1);
}

void AD8555::sendFunc(uint8_t value)
{
	sendBit((value >> 1) & 1);  
	sendBit(value & 1);
}

void AD8555::sendDummy()
{
	sendBit(1);
	sendBit(0);
}

void AD8555::sendValue(uint8_t value)
{
    uint8_t i = 8;
    do {
        i--;
        sendBit((value >> i) & 1);
    } while(i > 0);
}

void AD8555::sendStop()
{
    sendBit(0);
    for (uint8_t i = 0; i < 10; i++)
		sendBit(1);
    sendBit(0);
}

void AD8555::sendBit(bool value)
{
	if (value)
		send1();
	else
		send0();
}

void AD8555::send0()
{
	digitalWrite(outPin, HIGH);
    digitalWrite(outPin, LOW);
    delayMicroseconds(10);
}

void AD8555::send1()
{
	digitalWrite(outPin, HIGH);
    delayMicroseconds(DELAY_US_HIGH);
	digitalWrite(outPin, LOW);
    delayMicroseconds(10);
}
