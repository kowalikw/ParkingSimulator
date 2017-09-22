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
	Obstacle(glm::vec2 position, glm::vec2 size);
	Obstacle(glm::vec2 position, ObstacleType type);
	Obstacle(glm::vec2 position, glm::vec2 size, double rotation, std::vector<glm::vec2> points, ObstacleType type);
	Obstacle(glm::vec2 position, glm::vec2 size, ObstacleType type, std::string name);
	ObstacleType GetType();

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<MapElement>(*this);
		ar & type;
	}
private:
	ObstacleType type;
};

#endif