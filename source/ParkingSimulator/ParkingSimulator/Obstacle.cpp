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
	this->rotation = 0.0;
	this->type = type;
	this->name = name;

	this->points.push_back(glm::vec2(-size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, size.y / 2.0f));
	this->points.push_back(glm::vec2(-size.x / 2.0f, size.y / 2.0f));

	transform();
}

ObstacleType Obstacle::GetType()
{
	return this->type;
}