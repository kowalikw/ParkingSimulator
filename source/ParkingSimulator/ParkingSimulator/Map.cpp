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
