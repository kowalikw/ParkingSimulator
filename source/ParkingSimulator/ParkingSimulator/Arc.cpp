#include "Arc.h"

Arc::Arc()
{
}

Arc::Arc(glm::vec3 centre, double radius, double angleFrom, double angleTo, ArcType arcType)
{
	this->center = center;
	this->radius = radius;
	this->angleFrom = angleFrom;
	this->angleTo = angleTo;
	this->arcType = arcType;
}

double Arc::GetAngleFrom() const
{
	return this->angleFrom;
}

double Arc::GetAngleTo() const
{
	return this->angleTo;
}

double Arc::GetRadius() const
{
	return this->radius;
}

glm::vec3 Arc::GetCenter() const
{
	return this->center;
}

ArcType Arc::GetArcType() const
{
	return this->arcType;
}