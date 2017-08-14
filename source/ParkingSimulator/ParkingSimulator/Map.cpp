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

	this->AddMapElement(new Obstacle(glm::vec2(100, 100), glm::vec2(100, 100), ObstacleType::Building, "lalala"));
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
}

vector<MapElement*> Map::GetMapElements()
{
	return this->mapElements;
}

std::vector<glm::vec2> Map::GetPoints()
{
	return this->points;
}
