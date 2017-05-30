#include "Line.h"

Line::Line(glm::vec3 from, glm::vec3 to)
{
	this->from = from;
	this->to = to;
}

glm::vec3 Line::GetFrom() const
{
	return this->from;
}

glm::vec3 Line::GetTo() const
{
	return this->to;
}
