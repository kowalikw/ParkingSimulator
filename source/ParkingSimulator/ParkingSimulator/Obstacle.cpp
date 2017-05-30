#include "Obstacle.h"

Obstacle::Obstacle()
{
}

Obstacle::Obstacle(glm::vec2 position, ObstacleType type)
{
	this->position = position;
	this->type = type;
}

ObstacleType Obstacle::GetType()
{
	return this->type;
}