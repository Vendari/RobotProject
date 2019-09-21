#include "Map.h"
#include <functional>

Map::Map(std::vector<Point> map, unsigned int startPossition) : m_map { map }, m_position {startPossition} {
}

void Map::move(Direction direction)
{
	switch (direction)
	{
	case foward:
		m_position = m_map[m_position].foward;
		break;
	case right:
		m_position = m_map[m_position].right;
		break;
	case left:
		m_position = m_map[m_position].left;
		break;
	}
}

std::stack<Map::Direction> Map::getDirections(unsigned int destinationPoint) {

	std::stack<Direction> result;

	int distance_global = 0;
	int distance_local = 0;

	std::vector<bool> visited(m_map.size(), false);
	std::vector<int> distances(m_map.size(), 0);
	std::queue<Point> pointsQueue;

	int pos = m_position;
	int distance = 1;
	pointsQueue.push(m_map[pos]);

	while (true) { //BFS searching
		pos = pointsQueue.front().number;
		if (pos == destinationPoint)
			break;
		visited[pos] = true;
		pointsQueue.pop();
		for (int i = 0; i < 3; i++) {
			switch (i)
			{
			case 0:
				if (m_map[pos].foward != -1 && !visited[m_map[pos].foward]) {
					pointsQueue.push(m_map[m_map[pos].foward]);
					distances[m_map[pos].foward] = distance;
				}
				break;
			case 1:
				if (m_map[pos].right != -1 && !visited[m_map[pos].right]) {
					pointsQueue.push(m_map[m_map[pos].right]);
					distances[m_map[pos].right] = distance;
				}
				break;
			case 2:
				if (m_map[pos].left != -1 && !visited[m_map[pos].left]) {
					pointsQueue.push(m_map[m_map[pos].left]);
					distances[m_map[pos].left] = distance;
				}
				break;
			}
		}
		distance++;
	}

	pos = destinationPoint;
	distance = distances[destinationPoint];
	while (pos != m_position) {
		for (int i = 0; i < 3; i++) {
			if (i == 0)
				if (m_map[pos].back != -1 && distances[m_map[pos].back] < distance) {
					result.push(foward);
					break;
				}
				else if (i == 1) {
					if (m_map[pos].right != -1 && distances[m_map[pos].right] < distance) {
						result.push(left);
						break;
					}
				}
				else if (i == 2) {
					if (m_map[pos].left != -1 && distances[m_map[pos].left] < distance) {
						result.push(right);
						break;
					}
				}
		}
		distance--;
	}

	return result;
}
