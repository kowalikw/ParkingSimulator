#ifndef MAP_H
#define MAP_H

#include <vector>
#include <fstream>

#include "Terrain.h"
#include "MapElement.h"
#include "Obstacle.h"
#include "ParkingSpace.h"
#include "Vehicle.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

class Map
{
public:
	Map();
	Map(int width, int height);
	int GetWidth();
	int GetHeight();
	void AddMapElement(MapElement *mapElement);
	void RemoveMapElement(MapElement *mapElement);
	std::vector<std::vector<Terrain*>> GetTerrain();
	int GetTerrainWidthCount();
	int GetTerrainHeightCount();
	Terrain * GetTerrainSlice(int i, int j);
	Terrain * GetTerrainSlice(double x, double y);
	void SetTerrainSlice(Terrain *terrain, int i, int j);
	void SetTerrainSlice(Terrain *terrain, double x, double y);
	std::vector<MapElement*> GetMapElements();
	std::vector<Obstacle*> GetObstacles();
	std::vector<Vehicle*> GetVehicles();
	std::vector<ParkingSpace*> GetParkingSpaces();
	std::vector<glm::vec2> GetPoints();
	Map *GetExpandedMap(float expandSize);

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		// note, version is always the latest when saving
		ar & width;
		ar & height;
		ar & points.size();
		for(int i = 0; i < points.size(); i++)
			ar & points[i];
		ar & obstacles.size();
		for (int i = 0; i < obstacles.size(); i++)
			ar & *obstacles[i];
		ar & parkingSpaces.size();
		for (int i = 0; i < parkingSpaces.size(); i++)
			ar & *parkingSpaces[i];
		ar & vehicles.size();
		for (int i = 0; i < vehicles.size(); i++)
			ar & *vehicles[i];
		ar & terrain.size();
		ar & terrain[0].size();
		for (int i = 0; i < terrain.size(); i++)
			for (int j = 0; j < terrain[i].size(); j++)
				ar & terrain[i][j];
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		int pointsCount, obstaclesCount, parkingSpacesCount, vehiclesCount;
		int terrainWidthCount, terrainHeightCount;
		glm::vec2 point;
		Obstacle *obstacle;
		ParkingSpace *parkingSpace;
		Vehicle *vehicle;
		Terrain *terrain;

		ar & width;
		ar & height;

		ar & pointsCount;
		for (int i = 0; i < pointsCount; i++)
		{
			ar & point;
			points.push_back(point);
		}

		ar & obstaclesCount;
		for (int i = 0; i < obstaclesCount; i++)
		{
			obstacle = new Obstacle();
			ar & *obstacle;
			obstacles.push_back(obstacle);
			mapElements.push_back(obstacle);
		}

		ar & parkingSpacesCount;
		for (int i = 0; i < parkingSpacesCount; i++)
		{
			parkingSpace = new ParkingSpace();
			ar & *parkingSpace;
			parkingSpaces.push_back(parkingSpace);
			mapElements.push_back(parkingSpace);
		}

		ar & vehiclesCount;
		for (int i = 0; i < vehiclesCount; i++)
		{
			vehicle = new Vehicle();
			ar & *vehicle;
			vehicles.push_back(vehicle);
			mapElements.push_back(vehicle);
		}

		ar & terrainWidthCount;
		ar & terrainHeightCount;
		for (int i = 0; i < terrainWidthCount; i++)
		{
			this->terrain.push_back(std::vector<Terrain*>());
			for (int j = 0; j < terrainHeightCount; j++)
			{
				terrain = new Terrain();
				ar & *terrain;
				this->terrain[i].push_back(terrain);
			}
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	int width;
	int height;
	int terrainWidthCount;
	int terrainHeightCount;
	std::vector<std::vector<Terrain*>> terrain;
	std::vector<MapElement*> mapElements;
	std::vector<Obstacle*> obstacles;
	std::vector<Vehicle*> vehicles;
	std::vector<ParkingSpace*> parkingSpaces;
	std::vector<glm::vec2> points;

	friend class boost::serialization::access;
};

#endif