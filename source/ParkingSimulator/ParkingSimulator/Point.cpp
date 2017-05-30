#include "Point.h"

Point::Point(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

double Point::GetX() const
{
	return this->x;
}

double Point::GetY() const
{
	return this->y;
}

double Point::GetZ() const
{
	return this->z;
}
