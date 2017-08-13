#include "MapElement.h"

MapElement::~MapElement()
{
}

std::string MapElement::GetName()
{
	return this->name;
}

double MapElement::GetRotation()
{
	return this->rotation;
}

glm::vec2 MapElement::GetPosition()
{
	return this->position;
}

glm::vec2 MapElement::GetSize()
{
	return this->size;
}

std::vector<glm::vec2> MapElement::GetPoints()
{
	return this->points;
}

void MapElement::SetName(std::string name)
{
	this->name = name;
}

void MapElement::SetRotation(double rotation)
{
	this->rotation = rotation;

	transform();
}

void MapElement::SetPosition(glm::vec2 position)
{
	this->position = position;

	transform();
}

void MapElement::SetSize(glm::vec2 size)
{
	this->size = size;

	transform();
}

void MapElement::SetPoints(std::vector<glm::vec2> points)
{
	this->points = points;
}

void MapElement::move()
{
	for (int i = 0; i < points.size(); i++)
		points[i] = glm::vec2(points[i].x + position.x, points[i].y + position.y);
}

void MapElement::rotate()
{
	glm::mat4 mtx;
	mtx = glm::rotate(mtx, (float)rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < points.size(); i++)
	{
		glm::vec4 rotatedPoint = mtx * glm::vec4(points[i].x, points[i].y, 0.0f, 1.0f);
		points[i] = glm::vec2(rotatedPoint.x, rotatedPoint.y);
	}
}

void MapElement::resize()
{
	this->points[0] = glm::vec2(-size.x / 2.0f, -size.y / 2.0f);
	this->points[1] = glm::vec2(size.x / 2.0f, -size.y / 2.0f);
	this->points[2] = glm::vec2(size.x / 2.0f, size.y / 2.0f);
	this->points[3] = glm::vec2(-size.x / 2.0f, size.y / 2.0f);
}

void MapElement::transform()
{
	resize();
	rotate();
	move();
}
