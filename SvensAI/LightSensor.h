#pragma once
#include "Sensor.h"
class LightSensor :
	public Sensor
{
public:
	enum LightSensorState {
		stop = 3,
		right = 2,
		left = 1,
		foward = 0
	};

	void setValue(int value);
	int getValue();

private:
	LightSensorState m_state{};
};

