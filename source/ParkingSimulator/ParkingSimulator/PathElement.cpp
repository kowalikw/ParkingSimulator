#include "PathElement.h"

PathElement::PathElement()
{
}

PathElement::PathElement(glm::vec2 from, glm::vec2 to)
{
	this->from = from;
	this->to = to;
	this->type = PathElementType::Line;
}

PathElement::PathElement(glm::vec2 centre, double radius, double angleFrom, double angleTo, std::vector<glm::vec2> arcBasePoints, CircleType circleType)
{
	this->centre = centre;
	this->radius = radius;
	this->angleFrom = angleFrom;
	this->angleTo = angleTo;
	this->type = PathElementType::Circle;
	this->circleType = circleType;
	this->arcBasePoints = arcBasePoints;
}

glm::vec2 PathElement::GetFrom()
{
	return this->from;
}

glm::vec2 PathElement::GetTo()
{
	return this->to;
}

double PathElement::GetRadius()
{
	return this->radius;
}

double PathElement::GetAngleFrom()
{
	return this->angleFrom;
}

double PathElement::GetAngleTo()
{
	return this->angleTo;
}

PathElementType PathElement::GetType()
{
	return this->type;
}

glm::vec2 PathElement::GetLinePoint(double t)
{
	return glm::vec2();
}

glm::vec2 PathElement::GetCirclePoint(double angle)
{
	double x, y;

	if (circleType == CircleType::Right)
	{
		x = centre.x + radius * sin(angle);
		y = centre.y - radius * cos(angle);
	}
	else
	{
		x = centre.x + radius * sin(angle);
		y = centre.y + radius * cos(angle);
	}

	return glm::vec2(x, y);
}
