#pragma once
class Sensor
{
public:
	virtual int getValue() { return 0; };
	virtual void setValue(int value) {};
	virtual void setValue(char value) {};
};

