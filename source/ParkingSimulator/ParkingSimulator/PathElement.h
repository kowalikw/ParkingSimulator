#ifndef PATH_ELEMENT_H
#define PATH_ELEMENT_H

#include <vector>
#include <glm.hpp>

#define M_PI 3.14159265358979323846

enum PathElementType
{
	Line,
	Circle
};

enum CircleType
{
	Left,
	Right
};

class PathElement
{
public:
	PathElement();
	PathElement(glm::vec2 from, glm::vec2 to);
	PathElement(glm::vec2 centre, double radius, double angleFrom, double angleTo, std::vector<glm::vec2> arcBasePoints, CircleType circleType = CircleType::Right);
	glm::vec2 GetFrom();
	glm::vec2 GetTo();
	double GetRadius();
	double GetAngleFrom();
	double GetAngleTo();
	PathElementType GetType();
	glm::vec2 GetLinePoint(double t);
	glm::vec2 GetCirclePoint(double angle);

	glm::vec2 from;
	glm::vec2 to;
	glm::vec2 centre;
	double radius;
	double angleFrom;
	double angleTo;
	PathElementType type;
	CircleType circleType;
	std::vector<glm::vec2> arcBasePoints;
};

#endif