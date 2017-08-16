#include "Map.h"

Map::Map()
{
	this->width = 0;
	this->height = 0;
}

Map::Map(int width, int height)
{
	this->width = width;
	this->height = height;

	this->points.push_back(glm::vec2(0.0f, 0.0f));
	this->points.push_back(glm::vec2(width, 0.0f));
	this->points.push_back(glm::vec2(width, height));
	this->points.push_back(glm::vec2(0.0f, height));
}

int Map::GetWidth()
{
	return this->width;
}

int Map::GetHeight()
{
	return this->height;
}

void Map::AddMapElement(MapElement *mapElement)
{
	this->mapElements.push_back(mapElement);
	if (dynamic_cast<Obstacle*>(mapElement) != NULL)
		this->obstacles.push_back((Obstacle*)mapElement);
	else if (dynamic_cast<ParkingSpace*>(mapElement) != NULL)
		this->parkingSpaces.push_back((ParkingSpace*)mapElement);
}

vector<MapElement*> Map::GetMapElements()
{
	return this->mapElements;
}

std::vector<Obstacle*> Map::GetObstacles()
{
	return this->obstacles;
}

std::vector<ParkingSpace*> Map::GetParkingSpaces()
{
	return this->parkingSpaces;
}

std::vector<glm::vec2> Map::GetPoints()
{
	return this->points;
}
