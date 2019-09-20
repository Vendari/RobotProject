#pragma once
#include "Sensor.h"
#include "math.h"
#include <string>

class SonicSensor :
	public Sensor
{
public:
	enum SonicSensorState {
		stop = 5,
		close = 10,
		away = 15
	};

	void setValue(char value);
	int getValue();

private:
	SonicSensorState m_state{};

	int hex2dec(char hex);	
};

