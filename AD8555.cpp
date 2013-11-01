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

bool AD8555::simulate()
{
	return false;
}

bool AD8555::program()
{
	return false;
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
  
  #ifdef DBG
  Serial.print('0');
  #endif
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

  #ifdef DBG
  Serial.print('1');
  #endif
}
