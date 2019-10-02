#include "Inteligence.h"

Inteligence::Inteligence()
{
}

void Inteligence::think(std::shared_ptr<Sven> sven, std::shared_ptr<Map> map, int* pathLenght)
{
	sven->move();
	switch (map->getPlace()) {
	case Place::Restaurant: sven->eat(50); break;
	case Place::Work: sven->expUp(15); break;
	case Place::Home: sven->sleep(); break;
	case Place::Toilet: sven->poop(sven->getToilet()); break;
	case Place::Crossroad: break;
	}

	printf("\033[0;36m"); //cyan
	if (sven->getHunger() >= 50 && sven->getToilet() <= sven->getHunger()) {
		*pathLenght = map->setDirections(0); // 0 - restaurant
		printf("Going to restaurant...\n");
	}
	else if (sven->getToilet() >= 50 && sven->getHunger() <= sven->getToilet()) {
		*pathLenght = map->setDirections(6); // 6 - toilet
		printf("Going to toilet...\n");
	}
	else if (sven->getHunger() >= 20) {
		*pathLenght = map->setDirections(4); // 4 - work
		printf("Going to work...\n");
	}
	else {
		*pathLenght = map->setDirections(1); // 1 - home
		printf("Going to home...\n");
	}
	printf("\033[0;0m"); //normal
}


Inteligence::~Inteligence()
{
}
