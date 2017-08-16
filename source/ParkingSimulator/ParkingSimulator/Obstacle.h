#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "MapElement.h"

enum ObstacleType
{
	Building,
	Decoration
};

class Obstacle : public MapElement
{
public:
	Obstacle();
	Obstacle(glm::vec2 position, ObstacleType type);
	Obstacle(glm::vec2 position, glm::vec2 size, ObstacleType type, std::string name);
	ObstacleType GetType();

	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<MapElement>(*this);
		ar & type;

		int lala = 0;
	}
private:
	ObstacleType type;


};

#endif