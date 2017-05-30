#ifndef POINT_H
#define POINT_H

#include "PathElement.h"

class Point : public PathElement
{
public:
	Point(double x, double y, double z);
	double GetX() const;
	double GetY() const;
	double GetZ() const;
private:
	double x;
	double y;
	double z;
};

#endif