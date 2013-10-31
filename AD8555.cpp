#include "arduino_ad8555.h"

AD8555::AD8555(uint8_t OutputPin)
{
	m_outPin = OutputPin;
	pinMode(m_outPin, OUTPUT);
	digitalWrite(m_outPin, LOW);
}

bool AD8555::setSecondStageGain(int value)
{
	if ((value < 0) || (value > 7))
		return false;
}

bool AD8555::setFirstStageGain(int value)
{
	if ((value < 0) || (value > 127))
		return false;
}

bool AD8555::setOffset(int value)
{
	if ((value < 0) || (value > 255))
		return false;
}

bool AD8555::simulate()
{
	return false;
}

bool AD8555::program()
{
	return false;
}
