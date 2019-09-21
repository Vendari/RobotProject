#pragma once

#include "vector"
#include "Place.h"
#include <queue>
#include <stack>

class Map
{
public:
	enum Direction {
		foward = 0,
		right,
		left
	};

	struct Point {
		Place place;
		int number;
		int foward;
		int right;
		int left;
		int back;
		std::vector<bool> connections;
	};

	Map(std::vector<Point> map, unsigned int startPossition);

	void move(Direction direction);

	std::stack<Direction> getDirections(unsigned int destinationPoint);

	unsigned int getPosition() { return m_position; }

private:
	std::vector<Point> m_map;
	unsigned int m_position;
};

