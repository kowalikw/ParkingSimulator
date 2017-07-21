#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "MapElement.h"

enum ObstacleType
{
	Building,
	Decoration,
	Car
};

class Obstacle : public MapElement
{
public:
	Obstacle();
	Obstacle(glm::vec2 position, ObstacleType type);
	ObstacleType GetType();
private:
	ObstacleType type;
};

#endif