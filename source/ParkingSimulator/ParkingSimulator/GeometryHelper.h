#ifndef GEOMETRY_HELPER_H
#define GEOMETRY_HELPER_H

#include <glm.hpp>
#include "Line.h"
#include "Point.h"
#include "Arc.h"

class GeometryHelper
{
public:
	static double CrossProduct(glm::vec2 v1, glm::vec2 v2);
	static double DotProduct(glm::vec2 v1, glm::vec2 v2);
	static double GetDistanceBetweenPoints(Point p1, Point p2);
	static double GetAngleVector(glm::vec2 p1, glm::vec2 p2, ArcType direction);
	static ArcType GetVectorsDirection(glm::vec2 v1, glm::vec2 v2);
	static ArcType GetVectorsDirection(glm::vec2 p1, glm::vec2 p2, glm::vec2 commonPoint);
	static glm::vec2 GetLineIntersectionPoint(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
};

#endif