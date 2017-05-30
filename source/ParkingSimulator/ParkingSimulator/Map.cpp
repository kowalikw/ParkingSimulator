#include "Map.h"

Map::Map()
{
	
}

void Map::AddMapElement(MapElement *mapElement)
{
	this->mapElements.push_back(mapElement);
}

vector<MapElement*> Map::GetMapElements()
{
	return this->mapElements;
}
