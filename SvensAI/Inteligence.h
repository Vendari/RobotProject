#pragma once
#include <memory>

#include "Map.h"
#include "Sven.h"
#include "Place.h"

class Inteligence
{
public:
	Inteligence();

	void think(std::shared_ptr<Sven> sven, std::shared_ptr<Map> map, int* pathLenght);

	~Inteligence();
};

