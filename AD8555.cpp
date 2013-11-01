#include "arduino_ad8555.h"

AD8555::AD8555(uint8_t OutputPin)
{
	outPin = OutputPin;
	pinMode(m_outPin, OUTPUT);
	digitalWrite(m_outPin, LOW);
}

bool AD8555::setSecondStageGain(int value)
{
	if ((value < 0) || (value > 7))
		return false;
		
	SSG = value;
	return true;
}

bool AD8555::setFirstStageGain(int value)
{
	if ((value < 0) || (value > 127))
		return false;
	
	FSG = value;
	return true;
}

bool AD8555::setOffset(int value)
{
	if ((value < 0) || (value > 255))
		return false;
		
	OFS = value;
}

void AD8555::simulate()
{
	sendField0();      // start packet
	sendField1(0b01);  //  simulate
	sendField2(0b00);  //  second stage gain
	sendField3();      // dummy
	sendField4(SSG);   // value 
	sendField5();      // stop packet
	  
	sendField0();      // start packet
	sendField1(0b01);  //  simulate
	sendField2(0b01);  //  first stage gain
	sendField3();      // dummy
	sendField4(FSG);   // value
	sendField5();      // stop packet
	  
	sendField0();      // start packet
	sendField1(0b01);  //  simulate
	sendField2(0b10);  //  output offset
	sendField3();      // dummy
	sendField4(OFS);   // value
	sendField5();      // stop packet
}

void AD8555::program()
{
	unsigned char par;

	for (int i = 0 ; i < 8 ; i++)
		if (((SSG >> i) & 1) == 1) {
			par = (1 << i);
			blowSSGFuse(par);
		}

	for (int i = 0 ; i < 8 ; i++)
		if (((FSG >> i) & 1) == 1) {
			par = (1 << i);
			blowFSGFuse(par);
		}

	for (int i = 0 ; i < 8 ; i++)
		if (((OFS >> i) &1) == 1) {
			par = (1 << i);
			blowOFSFuse(par);
		}

	blowMSF();
}

void AD8555::blowSSGFuse(unsigned char Value)
{
	sendField0();      // start packet
	sendField1(0b10);  // program
	sendField2(0b00);  // second stage gain
	sendField3();      // dummy
	sendField4(Value); // value
	sendField5();      // stop packet
	delay(1);
}

void AD8555::blowFSGFuse(unsigned char Value)
{
	sendField0();      // start packet
	sendField1(0b10);  // program
	sendField2(0b01);  // second stage gain
	sendField3();      // dummy
	sendField4(Value); // value
	sendField5();      // stop packet
	delay(1);
}

void AD8555::blowOFSFuse(unsigned char Value)
{
	sendField0();      // start packet
	sendField1(0b10);  // program
	sendField2(0b10);  // second stage gain
	sendField3();      // dummy
	sendField4(Value); // value
	sendField5();      // stop packet
	delay(1);
}

void AD8555::blowMSF()
{
	sendField0();            // start packet
	sendField1(0b10);        // program
	sendField2(11);          // other functions
	sendField3();            // dummy
	sendField4(0b00000001);  // master fuse
	sendField5();            // stop packet
	delay(1);
}

void AD8555::sendField0()
{
	sendBit(1);
	sendBit(0);
	sendBit(0);
	sendBit(0);

	sendBit(0);
	sendBit(0);
	sendBit(0);
	sendBit(0);

	sendBit(0);
	sendBit(0);
	sendBit(0);
	sendBit(1);
}

void AD8555::sendField1(unsigned char value)
{
	sendBit((value >> 1) & 1);
	sendBit(value & 1);
}

void AD8555::sendField2(unsigned char value)
{
	sendBit((value >> 1) & 1);  
	sendBit(value & 1);
}

void AD8555::sendField3()
{
	sendBit(1);
	sendBit(0);
}

void AD8555::sendField4(unsigned char value)
{  
	sendBit((value >> 7) & 1);
	sendBit((value >> 6) & 1);
	sendBit((value >> 5) & 1);
	sendBit((value >> 4) & 1);
	sendBit((value >> 3) & 1);
	sendBit((value >> 2) & 1);
	sendBit((value >> 1) & 1);
	sendBit(value & 1);
}

void AD8555::sendField5()
{
	sendBit(0);
	sendBit(1);
	sendBit(1);
	sendBit(1);

	sendBit(1);
	sendBit(1);
	sendBit(1);
	sendBit(1);

	sendBit(1);
	sendBit(1);
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
	digitalWrite(outpin, HIGH);
	digitalWrite(outpin, LOW);
	delay(0);
}

void AD8555::send1()
{
	digitalWrite(outpin, HIGH);
	delay(0);
	delay(0);
	delay(0);
	delay(0);
	delay(0);
	delay(0);
	delay(0);
	delay(0);
	delay(0);
	digitalWrite(outpin, LOW);
	delay(0);
}
