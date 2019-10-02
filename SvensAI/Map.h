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

	enum MoveDirection {
		foward = 0,
		right,
		left
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

	Map(std::vector<Point> map, unsigned int startPossition, Direction startDirection);

	///<summary>
	///Sets directions to destination point in m_moveStack and returns lenght of m_moveStack
	///</summary>
	int setDirections(unsigned int destinationPoint);

	///<summary>
	///Returns in which direction Robot should turn
	///</summary>
	MoveDirection moveNext();

	///<summary>
	///Returns actual robot position
	///</summary>
	unsigned int getPosition() { return m_position; }

	///<summary>
	///Returns actual robot positions place type
	///</summary>
	Place::Type getPlace() { return m_map[m_position].place.m_type; }

	///<summary>
	///Returns actual robot direction
	///</summary>
	Direction getDirection() { return m_direction; }

private:
	///<summary>
	///Changes current possition to node that is in passed direction
	///</summary>
	void move(Direction direction);

	std::vector<Point> m_map;		   //map data
	unsigned int m_position;		   //current possition
	Direction m_direction;			   //current robot direction
	std::stack<Direction> m_moveStack; //store current path to destination
};

