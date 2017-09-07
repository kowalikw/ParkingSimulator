#include "GeometryHelper.h"
#include "OpenGLHost.h"

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

double GeometryHelper::CrossProduct(glm::vec2 v1, glm::vec2 v2)
{
	return v1.x * v2.y - v2.x * v1.y;
}

double GeometryHelper::DotProduct(glm::vec2 v1, glm::vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

bool GeometryHelper::IsEnclosedByRect(glm::vec2 point, glm::vec2 cornerA, glm::vec2 cornerB)
{
	return std::min(cornerA.x, cornerB.x) <= point.x
		&& point.x <= std::max(cornerA.x, cornerB.x)
		&& std::min(cornerA.y, cornerB.y) <= point.y
		&& point.y <= std::max(cornerA.y, cornerB.y);
}

glm::vec2 GeometryHelper::GetLineIntersectionPoint(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) // XZ plane
{
	double a1 = (p1.y - p2.y) / (p1.x - p2.x);
	double b1 = p2.y - a1 * p2.x;

	double a2 = (p3.y - p4.y) / (p3.x - p4.x);
	double b2 = p4.y - a2 * p4.x;

	if (isinf(a1) && !isinf(a2))
	{
		return glm::vec2(p1.x, a2 * p1.x + b2);
	}

	if (!isinf(a1) && isinf(a2))
	{
		return glm::vec2(p3.x, a1 * p3.x + b1);
	}

	double x = (b2 - b1) / (a1 - a2);
	double z = a1 * x + b1;

	return glm::vec2(x, z);
}

bool GeometryHelper::CheckSegmentsIntersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 q1, glm::vec2 q2)
{
	double d1 = CrossProduct(glm::vec2(q2.x - q1.x, q2.y - q1.y), glm::vec2(p1.x - q1.x, p1.y - q1.y));
	double d2 = CrossProduct(glm::vec2(q2.x - q1.x, q2.y - q1.y), glm::vec2(p2.x - q1.x, p2.y - q1.y));
	double d3 = CrossProduct(glm::vec2(p2.x - p1.x, p2.y - p1.y), glm::vec2(q1.x - p1.x, q1.y - p1.y));
	double d4 = CrossProduct(glm::vec2(p2.x - p1.x, p2.y - p1.y), glm::vec2(q2.x - p1.x, q2.y - p1.y));

	double d12 = d1 * d2;
	double d34 = d3 * d4;

	if (d12 > 0 || d34 > 0)
		return false;
	if (d12 < 0 || d34 < 0)
		return true;

	return IsEnclosedByRect(p1, q1, q2)
		|| IsEnclosedByRect(p2, q1, q2)
		|| IsEnclosedByRect(q1, p1, p2)
		|| IsEnclosedByRect(q2, p1, p2);
}

bool GeometryHelper::CheckPolygonContainsPoint(std::vector<glm::vec2> polygon, glm::vec2 point)
{
	double totalAngle = GetAngle(point, polygon[polygon.size() - 1], polygon[0]);

	for (int i = 0; i < polygon.size() - 1; i++)
		totalAngle += GetAngle(point, polygon[i], polygon[i + 1]);

	return !(abs(totalAngle) < EPS);
}

bool GeometryHelper::CheckPolygonIntersection(std::vector<glm::vec2> polygon1, std::vector<glm::vec2> polygon2)
{
	for (int i = 0; i < polygon1.size(); i++)
		for (int j = 0; j < polygon2.size(); j++)
			if (CheckSegmentsIntersection(polygon1[i], polygon1[(i + 1) % polygon1.size()], polygon2[j], polygon2[(j + 1) % polygon2.size()]))
				return true;

	return false;
}

bool GeometryHelper::CheckPolygonContainsPolygon(std::vector<glm::vec2> subject, std::vector<glm::vec2> polygon)
{
	for (int i = 0; i < polygon.size(); i++)
		if (!CheckPolygonContainsPoint(subject, polygon[i]))
			return false;

	return true;
}

bool GeometryHelper::CheckPolygonSegmentIntersection(glm::vec2 p1, glm::vec2 p2, std::vector<glm::vec2> polygon)
{
	for (int i = 0; i < polygon.size(); i++)
		if (CheckSegmentsIntersection(p1, p2, polygon[i], polygon[(i + 1) % polygon.size()]))
			return true;

	return false;
}

double GeometryHelper::GetAngle(glm::vec2 pivot, glm::vec2 source, glm::vec2 dest)
{
	glm::vec2 s = glm::vec2(source.x - pivot.x, source.y - pivot.y);
	glm::vec2 d = glm::vec2(dest.x - pivot.x, dest.y - pivot.y);

	return atan2(CrossProduct(s, d), DotProduct(s, d));
}

/*double GeometryHelper::GetDistanceBetweenPoints(Point p1, Point p2)
{
	return sqrt((p1.GetX() - p2.GetX()) * (p1.GetX() - p2.GetX()) + (p1.GetY() - p2.GetY()) * (p1.GetY() - p2.GetY()) + (p1.GetZ() - p2.GetZ()) * (p1.GetZ() - p2.GetZ()));
}*/

double GeometryHelper::GetAngleVector(glm::vec2 p1, glm::vec2 p2, CircleType direction)
{
	double alfa = 0;

	if(direction == CircleType::Left)
		alfa = 2 * M_PI - atan2(p2.y - p1.y, p2.x - p1.x);
	else if(direction == CircleType::Right)
		alfa = atan2(p2.y - p1.y, p2.x - p1.x);
	
	return alfa;
}

CircleType GeometryHelper::GetVectorsDirection(glm::vec2 v1, glm::vec2 v2)
{
	double crossProduct = CrossProduct(v1, v2);

	if (crossProduct < 0)
		return CircleType::Right;
	if (crossProduct > 0)
		return CircleType::Left;
	return CircleType::Undefined;
}

CircleType GeometryHelper::GetVectorsDirection(glm::vec2 p1, glm::vec2 p2, glm::vec2 commonPoint)
{
	return GeometryHelper::GetVectorsDirection(p1 - commonPoint, p2 - commonPoint);
}

glm::vec2 GeometryHelper::GetLineDirection(glm::vec2 p1, glm::vec2 p2)
{
	glm::vec2 direction = p2 - p1;

	direction = glm::normalize(direction);

	return direction;
}
