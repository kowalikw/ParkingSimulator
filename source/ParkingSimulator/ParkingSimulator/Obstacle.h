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
private:
	ObstacleType type;
};

#endif