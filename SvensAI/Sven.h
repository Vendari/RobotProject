#pragma once#
#include <stdio.h>

class Sven
{
public:
	enum Feel {
		Happy = 0,
		Moderate,
		Sad
	};

	Sven(int startHunger, int startToilet, int startLevel);

	void move();
	void sleep();

	void eat(int n);
	void poop(int n);
	void expUp(int n);

	int getHunger() { return m_hunger; }
	int getToilet() { return m_toilet; }
	int getLevel()	{ return m_level;  }

	void logStats();

	~Sven();
private:
	Feel m_feel;
	int m_hunger;
	int m_toilet;
	int m_level;
	unsigned int m_experience;
};

