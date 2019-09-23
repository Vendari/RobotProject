#include "Map.h"

Map::Map(std::vector<Point> map, unsigned int startPossition) : m_map { map }, m_position {startPossition} {
}

void Map::move(Direction direction)
{
	switch (direction)
	{
	case north:
		if (m_map[m_position].north == -1) throw "Node does not exist!";
		m_position = m_map[m_position].north;
		break;
	case east:
		if (m_map[m_position].east == -1) throw "Node does not exist!";
		m_position = m_map[m_position].east;
		break;
	case west:
		if (m_map[m_position].west == -1) throw "Node does not exist!";
		m_position = m_map[m_position].west;
		break;
	}
}

std::stack<Map::Direction> Map::getDirections(unsigned int destinationPoint) {

	std::stack<Direction> result;

	std::vector<bool> visited(m_map.size(), false);
	std::vector<int> distances(m_map.size(), INT16_MAX);
	std::queue<Point> pointsQueue;

	int pos = m_position;
	int distance = 1;
	pointsQueue.push(m_map[pos]);
	distances[pos] = 0;

	while (true) { //BFS searching (distance marking)
		pos = pointsQueue.front().number;
		if (pos == destinationPoint)
			break;
		visited[pos] = true;
		pointsQueue.pop();
		for (int i = 0; i < 4; i++) {
			switch (i)
			{
			case 0:
				if (m_map[pos].north != -1 && !visited[m_map[pos].north]) {
					pointsQueue.push(m_map[m_map[pos].north]);
					if(distances[m_map[pos].north] > distance)
					distances[m_map[pos].north] = distance;
				}
				break;
			case 1:
				if (m_map[pos].east != -1 && !visited[m_map[pos].east]) {
					pointsQueue.push(m_map[m_map[pos].east]);
					if (distances[m_map[pos].east] > distance)
					distances[m_map[pos].east] = distance;
				}
				break;
			case 2:
				if (m_map[pos].west != -1 && !visited[m_map[pos].west]) {
					pointsQueue.push(m_map[m_map[pos].west]);
					if (distances[m_map[pos].west] > distance)
					distances[m_map[pos].west] = distance;
				}
				break;
			case 3:
				if (m_map[pos].south != -1 && !visited[m_map[pos].south]) {
					pointsQueue.push(m_map[m_map[pos].south]);
					if (distances[m_map[pos].south] > distance)
						distances[m_map[pos].south] = distance;
				}
				break;
			}
		}
		distance++;
	}

	pos = destinationPoint;
	distance = distances[destinationPoint];
	while (pos != m_position) {//right pathFinding
		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				if (m_map[pos].south != -1 && distances[m_map[pos].south] < distance) {
					result.push(north);
					pos = m_map[pos].south;
					break;
				}
			}
			else if (i == 1) {
				if (m_map[pos].east != -1 && distances[m_map[pos].east] < distance) {
					result.push(west);
					pos = m_map[pos].east;
					break;
				}
			}
			else if (i == 2) {
				if (m_map[pos].west != -1 && distances[m_map[pos].west] < distance) {
					result.push(east);
					pos = m_map[pos].west;
					break;
				}
			}
			else if (i == 3) {
				if (m_map[pos].north != -1 && distances[m_map[pos].north] < distance) {
					result.push(south);
					pos = m_map[pos].north;
					break;
				}
			}
		}
		distance--;
	}

	return result;
}
