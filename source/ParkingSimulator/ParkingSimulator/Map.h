#ifndef MAP_H
#define MAP_H

#include <vector>
#include "MapElement.h"
#include "Obstacle.h"
#include "ParkingSpace.h"
#include "Vehicle.h"

#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(glm::vec2)

namespace boost
{
	namespace serialization
	{

		template<class Archive> void save(Archive& ar, const glm::vec2& v, unsigned int version)
		{
			ar & v.x & v.y;
		}

		template<class Archive> void load(Archive& ar, glm::vec2& v, unsigned int version)
		{
			ar & v.x & v.y;
		}

	}
}

using namespace std;

class Map
{
public:
	Map();
	Map(int width, int height);
	int GetWidth();
	int GetHeight();
	void AddMapElement(MapElement *mapElement);
	std::vector<MapElement*> GetMapElements();
	std::vector<Obstacle*> GetObstacles();
	std::vector<ParkingSpace*> GetParkingSpaces();
	std::vector<glm::vec2> GetPoints();
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
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		int pointsCount, obstaclesCount, parkingSpacesCount;
		glm::vec2 point;
		Obstacle *obstacle;
		ParkingSpace *parkingSpace;

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
			mapElements.push_back(obstacle);
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	int width;
	int height;
	std::vector<MapElement*> mapElements;
	std::vector<Obstacle*> obstacles;
	std::vector<ParkingSpace*> parkingSpaces;
	std::vector<glm::vec2> points;

	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	/*template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & width;
		ar & height;
		ar & points;
	}*/
};

#endif