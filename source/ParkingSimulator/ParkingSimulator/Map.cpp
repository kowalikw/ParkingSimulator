#include "Map.h"

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 
#include <Windows.h>

Map::Map()
{
	this->width = 0;
	this->height = 0;
}

Map::Map(int width, int height)
{
	this->width = width;
	this->height = height;
	this->terrainWidthCount = (int)(width / TERRAIN_SLICE_SIZE);
	this->terrainHeightCount = (int)(height / TERRAIN_SLICE_SIZE);

	for (int i = 0; i < terrainWidthCount; i++)
	{
		this->terrain.push_back(std::vector<Terrain*>());
		for (int j = 0; j < terrainHeightCount; j++)
		{
			Terrain *terrainSlice = new Terrain(glm::vec2(i * TERRAIN_SLICE_SIZE, j * TERRAIN_SLICE_SIZE));
			this->terrain[i].push_back(terrainSlice);
		}
	}

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
	else if (dynamic_cast<Vehicle*>(mapElement) != NULL)
		this->vehicles.push_back((Vehicle*)mapElement);
	else if (dynamic_cast<ParkingSpace*>(mapElement) != NULL)
		this->parkingSpaces.push_back((ParkingSpace*)mapElement);
}

void Map::RemoveMapElement(MapElement * mapElement)
{
	std::vector<MapElement*> newMapElements;
	for (int i = 0; i < mapElements.size(); i++)
		if (mapElements[i] == mapElement)
			continue;
		else
			newMapElements.push_back(mapElements[i]);
	this->mapElements = newMapElements;

	std::vector<Obstacle*> newObstacles;
	for (int i = 0; i < obstacles.size(); i++)
		if (obstacles[i] == mapElement)
			continue;
		else
			newObstacles.push_back(obstacles[i]);
	this->obstacles = newObstacles;

	std::vector<Vehicle*> newVehicles;
	for (int i = 0; i < vehicles.size(); i++)
		if (vehicles[i] == mapElement)
			continue;
		else
			newVehicles.push_back(vehicles[i]);
	this->vehicles = newVehicles;

	std::vector<ParkingSpace*> newParkingSpaces;
	for (int i = 0; i < parkingSpaces.size(); i++)
		if (parkingSpaces[i] == mapElement)
			continue;
		else
			newParkingSpaces.push_back(parkingSpaces[i]);
	this->parkingSpaces = newParkingSpaces;
}

std::vector<std::vector<Terrain*>> Map::GetTerrain()
{
	return this->terrain;
}

int Map::GetTerrainWidthCount()
{
	return this->terrainWidthCount;
}

int Map::GetTerrainHeightCount()
{
	return this->terrainHeightCount;
}

Terrain * Map::GetTerrainSlice(int i, int j)
{
	return this->terrain[i][j];
}

Terrain * Map::GetTerrainSlice(double x, double y)
{
	if (x > width || y > height || x < 0 || y < 0) return nullptr;

	int i = (int)(x / TERRAIN_SLICE_SIZE);
	int j = (int)(y / TERRAIN_SLICE_SIZE);

	return this->terrain[i][j];
}

void Map::SetTerrainSlice(Terrain *terrain, int i, int j)
{
	this->terrain[i][j] = terrain;
}

void Map::SetTerrainSlice(Terrain *terrain, double x, double y)
{
	if (x > width || y > height || x < 0 || y < 0) return;

	int i = (int)(x / TERRAIN_SLICE_SIZE);
	int j = (int)(y / TERRAIN_SLICE_SIZE);

	this->terrain[i][j] = terrain;
}

vector<MapElement*> Map::GetMapElements()
{
	return this->mapElements;
}

std::vector<Obstacle*> Map::GetObstacles()
{
	return this->obstacles;
}

std::vector<Vehicle*> Map::GetVehicles()
{
	return this->vehicles;
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
		else if (dynamic_cast<Vehicle*>(mapElements[i]) != NULL)
		{
			Vehicle *parkingSpace = dynamic_cast<Vehicle*>(mapElements[i]);
			newElement = new Vehicle(parkingSpace->GetPosition(), parkingSpace->GetSize(), parkingSpace->GetRotation(), expandedPoints);
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

	return expandedMap;
}

bool Map::IsVehicleAdmissible(Vehicle * vehicle)
{
	return MapContainsVehicle(vehicle) && !VehicleIntersectsMapElement(vehicle);
}

bool Map::MapContainsVehicle(Vehicle * vehicle)
{
	std::vector<glm::vec2> mapPolygon = GetPoints();
	std::vector<glm::vec2> vehiclePolygon = vehicle->GetPoints2();

	return GeometryHelper::CheckPolygonContainsPolygon(mapPolygon, vehiclePolygon);
}

bool Map::VehicleIntersectsMapElement(Vehicle * vehicle)
{
	std::vector<MapElement*> mapElements = GetMapElements();
	for (int i = 0; i < mapElements.size(); i++)
	{
		if (mapElements[i] == vehicle) continue;

		std::ostringstream ss;
		ss << "P0 - X: " << mapElements[i]->GetPoints()[0].x << ", Y: " << mapElements[i]->GetPoints()[0].y << endl;
		ss << "P1 - X: " << mapElements[i]->GetPoints()[1].x << ", Y: " << mapElements[i]->GetPoints()[1].y << endl;
		ss << "P2 - X: " << mapElements[i]->GetPoints()[2].x << ", Y: " << mapElements[i]->GetPoints()[2].y << endl;
		ss << "P3 - X: " << mapElements[i]->GetPoints()[3].x << ", Y: " << mapElements[i]->GetPoints()[3].y << endl;

		ss << "V0 - X: " << vehicle->GetPoints2()[0].x << ", Y: " << vehicle->GetPoints2()[0].y << endl;
		ss << "V1 - X: " << vehicle->GetPoints2()[1].x << ", Y: " << vehicle->GetPoints2()[1].y << endl;
		ss << "V2 - X: " << vehicle->GetPoints2()[2].x << ", Y: " << vehicle->GetPoints2()[2].y << endl;
		ss << "V3 - X: " << vehicle->GetPoints2()[3].x << ", Y: " << vehicle->GetPoints2()[3].y << endl;

		ss << "Position: X=" << vehicle->GetPosition().x << ", Y=" << vehicle->GetPosition().y << endl;
		ss << "Rotation: " << vehicle->GetRotation() << endl;
		ss << "Size: X=" << vehicle->GetSize().x << ", Y=" << vehicle->GetSize().y << endl;

		ss << endl;
		std::string s(ss.str());

		OutputDebugStringA(s.c_str());

		if (GeometryHelper::CheckPolygonIntersection(vehicle->GetPoints2(), mapElements[i]->GetPoints()))
			return true;
	}

	return false;
}

bool Map::IsMapElementAdmissible(MapElement * mapElement)
{
	return MapContainsMapElement(mapElement) && !MapElementIntersectsMapElement(mapElement);
}

bool Map::MapContainsMapElement(MapElement * mapElement)
{
	std::vector<glm::vec2> mapPolygon = GetPoints();
	std::vector<glm::vec2> mapElementPolygon = mapElement->GetPoints();

	return GeometryHelper::CheckPolygonContainsPolygon(mapPolygon, mapElementPolygon);
}

bool Map::MapElementIntersectsMapElement(MapElement * mapElement)
{
	std::vector<MapElement*> mapElements = GetMapElements();
	for (int i = 0; i < mapElements.size(); i++)
	{
		if (mapElements[i] == mapElement) continue;

		std::ostringstream ss;
		ss << "P0 - X: " << mapElements[i]->GetPoints()[0].x << ", Y: " << mapElements[i]->GetPoints()[0].y << endl;
		ss << "P1 - X: " << mapElements[i]->GetPoints()[1].x << ", Y: " << mapElements[i]->GetPoints()[1].y << endl;
		ss << "P2 - X: " << mapElements[i]->GetPoints()[2].x << ", Y: " << mapElements[i]->GetPoints()[2].y << endl;
		ss << "P3 - X: " << mapElements[i]->GetPoints()[3].x << ", Y: " << mapElements[i]->GetPoints()[3].y << endl;

		ss << "V0 - X: " << mapElement->GetPoints()[0].x << ", Y: " << mapElement->GetPoints()[0].y << endl;
		ss << "V1 - X: " << mapElement->GetPoints()[1].x << ", Y: " << mapElement->GetPoints()[1].y << endl;
		ss << "V2 - X: " << mapElement->GetPoints()[2].x << ", Y: " << mapElement->GetPoints()[2].y << endl;
		ss << "V3 - X: " << mapElement->GetPoints()[3].x << ", Y: " << mapElement->GetPoints()[3].y << endl;


		ss << endl;
		std::string s(ss.str());

		OutputDebugStringA(s.c_str());

		if (GeometryHelper::CheckPolygonIntersection(mapElement->GetPoints(), mapElements[i]->GetPoints()))
			return true;
	}

	return false;
}