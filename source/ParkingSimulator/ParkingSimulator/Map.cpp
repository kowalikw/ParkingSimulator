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

Map * Map::GetExpandedMap(float expandSize)
{
	Map * expandedMap = new Map(width, height);

	for (int i = 0; i < mapElements.size(); i++)
	{
		std::vector<glm::vec2> expandedPoints = GeometryHelper::ExpandPolygon(mapElements[i]->GetPoints(), expandSize);

		MapElement *newElement;
		if (dynamic_cast<Obstacle*>(mapElements[i]) != NULL)
		{
			Obstacle *obstacle = dynamic_cast<Obstacle*>(mapElements[i]);
			newElement = new Obstacle(obstacle->GetPosition(), obstacle->GetSize(), obstacle->GetRotation(), expandedPoints, obstacle->GetType());
		}
		else if (dynamic_cast<ParkingSpace*>(mapElements[i]) != NULL)
		{
			ParkingSpace *parkingSpace = dynamic_cast<ParkingSpace*>(mapElements[i]);
			newElement = new ParkingSpace(parkingSpace->GetPosition(), parkingSpace->GetSize(), parkingSpace->GetRotation(), expandedPoints, parkingSpace->GetType());
			
		}
		expandedMap->AddMapElement(newElement);
	}

	MapElement *mapEdgeLeft = new Obstacle(glm::vec2(expandSize / 2.0f, height / 2.0f), glm::vec2(expandSize, height - expandSize));
	MapElement *mapEdgeRight = new Obstacle(glm::vec2(width - expandSize / 2.0f, height / 2.0f), glm::vec2(expandSize, height - expandSize));
	MapElement *mapEdgeTop = new Obstacle(glm::vec2(width / 2.0f, expandSize / 2.0f), glm::vec2(width - expandSize, expandSize));
	MapElement *mapEdgeBottom = new Obstacle(glm::vec2(width / 2.0f, height - expandSize / 2.0f), glm::vec2(width - expandSize, expandSize));

	//expandedMap->AddMapElement(mapEdgeLeft);
	//expandedMap->AddMapElement(mapEdgeRight);
	/*expandedMap->AddMapElement(mapEdgeTop);
	expandedMap->AddMapElement(mapEdgeBottom);*/

	return expandedMap;
}
