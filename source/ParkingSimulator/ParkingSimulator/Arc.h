#ifndef ARC_H
#define ARC_H

#include "PathElement.h"

enum ArcType
{
	Left1, // ccw
	Right1, // cw
	Undefined // colinear
};

class Arc : public PathElement
{
public:
	Arc();
	Arc(glm::vec3 centre, double radius, double angleFrom, double angleTo, ArcType arcType = ArcType::Right1);
	double GetAngleFrom() const;
	double GetAngleTo() const;
	double GetRadius() const;
	glm::vec3 GetCenter() const;
	ArcType GetArcType() const;
private:
	double angleFrom;
	double angleTo;
	double radius;
	glm::vec3 center;
	ArcType arcType;
};

#endif