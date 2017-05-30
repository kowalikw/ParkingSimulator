#pragma once

#include <glm.hpp>
#include "Line.h"
#include "Point.h"
#include "Arc.h"

class GeometryHelper
{
public:
	static bool LineIntersection();
	static glm::vec3 GetLineIntersectionPoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
	static double GetDistanceBetweenPoints(Point p1, Point p2);
	static double GetAngleVector(glm::vec3 p1, glm::vec3 p2, ArcType direction);
	static ArcType GetVectorsDirection(glm::vec3 v1, glm::vec3 v2);
	static ArcType GetVectorsDirection(glm::vec3 p1, glm::vec3 p2, glm::vec3 commonPoint);
	//static glm::vec3 GetLineIntersectionPoint(Line line1, Line line2);
};

