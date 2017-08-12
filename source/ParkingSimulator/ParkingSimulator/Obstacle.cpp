#include "Obstacle.h"

Obstacle::Obstacle()
{
}

Obstacle::Obstacle(glm::vec2 position, ObstacleType type)
{
	this->position = position;
	this->type = type;
}

Obstacle::Obstacle(glm::vec2 position, glm::vec2 size, ObstacleType type, std::string name)
{
	this->position = position;
	this->size = size;
	this->type = type;
	this->name = name;
}

ObstacleType Obstacle::GetType()
{
	return this->type;
}