#ifndef GEOMETRY_HELPER_H
#define GEOMETRY_HELPER_H

#include <vector>
#include <glm.hpp>
#include "Line.h"
#include "Arc.h"

#define EPS 10e-6

class GeometryHelper
{
public:
	static double CrossProduct(glm::vec2 v1, glm::vec2 v2);
	static double DotProduct(glm::vec2 v1, glm::vec2 v2);
	static bool IsEnclosedByRect(glm::vec2 point, glm::vec2 cornerA, glm::vec2 cornerB);
	static double GetDistanceBetweenPoints(glm::vec2 p1, glm::vec2 p2);
	static double GetAngleVector(glm::vec2 p1, glm::vec2 p2, CircleType direction);
	static double GetAngleBetweenVectors(glm::vec2 v1, glm::vec2 v2);
	static CircleType GetVectorsDirection(glm::vec2 v1, glm::vec2 v2);
	static CircleType GetVectorsDirection(glm::vec2 p1, glm::vec2 p2, glm::vec2 commonPoint);
	static glm::vec2 GetLineDirection(glm::vec2 p1, glm::vec2 p2);
	static glm::vec2 GetLineNormal(glm::vec2 p1, glm::vec2 p2);
	static glm::vec2 GetLineIntersectionPoint(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);
	static std::vector<glm::vec2> ExpandPolygon(std::vector<glm::vec2> polygon, float expandSize);
	static bool CheckSegmentsIntersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 q1, glm::vec2 q2);
	static bool CheckPolygonContainsPoint(std::vector<glm::vec2> polygon, glm::vec2 point);
	static bool CheckPolygonContainsAnyPoint(std::vector<glm::vec2> polygon, std::vector<glm::vec2> points);
	static bool CheckPolygonIntersection(std::vector<glm::vec2> polygon1, std::vector<glm::vec2> polygon2);
	static bool CheckPolygonContainsPolygon(std::vector<glm::vec2> subject, std::vector<glm::vec2> polygon);
	static bool CheckPolygonSegmentIntersection(glm::vec2 p1, glm::vec2 p2, std::vector<glm::vec2> polygon);
private:
	static double GetAngle(glm::vec2 pivot, glm::vec2 source, glm::vec2 dest);
};

#endif