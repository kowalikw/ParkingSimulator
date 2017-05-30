#include "MapElement.h"

MapElement::~MapElement()
{
}

double MapElement::GetPosX()
{
	return this->position.x;
}

double MapElement::GetPosY()
{
	return this->position.y;
}

double MapElement::GetLength()
{
	return this->size.x;
}

double MapElement::GetWidth()
{
	return this->size.y;
}

glm::vec2 MapElement::GetPosition()
{
	return this->position;
}

glm::vec2 MapElement::GetRotation()
{
	return this->size;
}

glm::vec2 MapElement::GetSize()
{
	return this->size;
}
