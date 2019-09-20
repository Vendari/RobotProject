#pragma once
#include "Sensor.h"
class LightSensor :
	public Sensor
{
public:
	enum LightSensorState {
		stop = 0,
		right,
		left,
		foward
	};

	void setValue(int value);
	int getValue();

private:
	LightSensorState m_state{};
};

