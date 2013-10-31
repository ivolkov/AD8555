#include "arduino_ad8555.h"

AD8555:AD8555(int OutputPin)
{
	m_outPin = OutputPin;
	pinMode(m_outPin, OUTPUT);
	digitalWrite(m_outPin, LOW);
}
