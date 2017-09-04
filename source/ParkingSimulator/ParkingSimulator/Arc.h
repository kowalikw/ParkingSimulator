#ifndef CIRCLE_H
#define CIRCLE_H

#include "PathElement.h"

enum CircleType
{
	Left, // ccw
	Right, // cw
	Undefined // colinear
};

class Circle : public PathElement
{
public:
	Circle();
	Circle(glm::vec2 centre, double radius, double angleFrom, double angleTo, std::vector<glm::vec2> circleBasePoints, CircleType circleType = CircleType::Right);
	double GetAngleFrom() const;
	double GetAngleTo() const;
	double GetRadius() const;
	glm::vec2 GetCenter() const;
	CircleType GetCircleType() const;
	std::vector<glm::vec2> GetCircleBasePoints() const;
	glm::vec2 GetPointForAngle(double angle);

	virtual double GetLength();
	virtual double GetAngle(double t);
	virtual glm::vec2 GetPoint(double t);
	virtual SimulationState GetSimulationState(double t);
//private:
	double angleFrom;
	double angleTo;
	double radius;
	glm::vec2 center;
	CircleType circleType;
	std::vector<glm::vec2> circleBasePoints;
};

#endif