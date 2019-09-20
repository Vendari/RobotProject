#include "SonicSensor.h"

void SonicSensor::setValue(char value)
{
	int intValue = hex2dec(value);
	if (intValue <= 5) {
		m_state = stop;
	}
	else if (intValue <= 10) {
		m_state = close;
	}
	else {
		m_state = away;
	}
}

int SonicSensor::getValue()
{
	return m_state;
}

int SonicSensor::hex2dec(char hex)
{
	switch (hex)
	{
	case '1': return 1; break;
	case '2': return 2; break;
	case '3': return 3; break;
	case '4': return 4; break;
	case '5': return 5; break;
	case '6': return 6; break;
	case '7': return 7; break;
	case '8': return 8; break;
	case '9': return 9; break;
	case 'A': return 10; break;
	case 'B': return 11; break;
	case 'C': return 12; break;
	case 'D': return 13; break;
	case 'E': return 14; break;
	case 'F': return 15; break;
	}
	return 0;
}
