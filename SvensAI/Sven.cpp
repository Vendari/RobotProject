#include "Sven.h"

Sven::Sven(int startHunger, int startToilet, int startLevel) : 
	m_hunger{ startHunger }, m_toilet{ startToilet }, m_level{ startLevel }, m_experience{ 0 }, m_feel{Happy} {}

void Sven::move()
{
	m_hunger += 5;
	m_toilet += 1;
	m_experience += 1;
	int statsAvg = (this->getHunger() + this->getToilet()) / 2 - this->getLevel();
	if (statsAvg <= 25)
		m_feel = Happy;
	else if (statsAvg <= 50)
		m_feel = Moderate;
	else
		m_feel = Sad;
}

void Sven::sleep()
{
	m_hunger += 2;
	m_toilet += 2;
	m_experience += 3;
}

void Sven::eat(int n)
{
	printf("\033[1;35m"); //bold magenta
	m_hunger -= n;
	m_experience += 5;
	printf("Eating %d food\n", n);
	printf("\033[0;0m"); //normal
}

void Sven::poop(int n)
{
	printf("\033[1;35m"); //bold magenta
	m_toilet -= n;
	m_experience += 5;
	printf("Pooping %d poop\n", n);
	printf("\033[0;0m"); //normal
}

void Sven::expUp(int n)
{
	printf("\033[1;35m"); //bold magenta
	if (m_experience + n >= 100) {
		m_level++;
		printf("Level UP!\n");
		m_experience -= 100;
	}
	m_experience += n;
	m_toilet += 5;
	m_hunger += 5;
	printf("Getting %d exp\n", n);
	printf("\033[0;0m"); //normal
}

void Sven::logStats()
{
	if (this->m_feel == Feel::Happy)
		printf("\033[0;32m");
	else if(this->m_feel==Feel::Moderate)
		printf("\033[0;33m");
	else
		printf("\033[0;31m");

	printf("Hunger: %d | Poop: %d | Level: %d | Feel: %d | Experience: %d\n", m_hunger, m_toilet, m_level, static_cast<int>(m_feel), m_experience);
	printf("\033[0m");
}

Sven::~Sven()
{
}
