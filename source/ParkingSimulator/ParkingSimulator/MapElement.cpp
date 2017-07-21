#include "MapElement.h"

MapElement::~MapElement()
{
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
