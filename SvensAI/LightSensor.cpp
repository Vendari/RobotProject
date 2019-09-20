#include "LightSensor.h"

void LightSensor::setValue(int value)
{
	m_state = static_cast<LightSensorState>(value);
}

int LightSensor::getValue()
{
	return m_state;
}
