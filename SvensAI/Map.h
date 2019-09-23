#pragma once

#include "vector"
#include "Place.h"
#include <queue>
#include <stack>
#include <functional>
#include <limits>

class Map
{
public:
	enum Direction {
		north = 0,
		east,
		south,
		west
	};

	struct Point {
		Place place;
		int number;
		int north;
		int east;
		int west;
		int south;
		std::vector<bool> connections;
	};

	Map(std::vector<Point> map, unsigned int startPossition);

	///<summary>
	///Changes current possition to node that is in passed direction
	///</summary>
	void move(Direction direction);

	///<summary>
	///Returns stack data structure with directions (north, eas, south, west) to passed direction point from actual possition
	///</summary>
	std::stack<Direction> getDirections(unsigned int destinationPoint);

	///<summary>
	///Returns actual robot position
	///</summary>
	unsigned int getPosition() { return m_position; }

private:
	std::vector<Point> m_map; //map data
	unsigned int m_position;  //current possition
};

