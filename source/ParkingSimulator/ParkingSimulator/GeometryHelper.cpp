#include "GeometryHelper.h"

bool GeometryHelper::LineIntersection()
{
	return false;
}

glm::vec3 GeometryHelper::GetLineIntersectionPoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) // XZ plane
{
	double a1 = (p1.z - p2.z) / (p1.x - p2.x);
	double b1 = p2.z - a1 * p2.x;

	double a2 = (p3.z - p4.z) / (p3.x - p4.x);
	double b2 = p4.z - a2 * p4.x;

	if (isinf(a1) && !isinf(a2))
	{
		return glm::vec3(p1.x, 0, a2 * p1.x + b2);
	}

	if (!isinf(a1) && isinf(a2))
	{
		return glm::vec3(p3.x, 0, a1 * p3.x + b1);
	}

	double x = (b2 - b1) / (a1 - a2);
	double y = 0;
	double z = a1 * x + b1;

	return glm::vec3(x, y, z);
}

double GeometryHelper::GetDistanceBetweenPoints(Point p1, Point p2)
{
	return sqrt((p1.GetX() - p2.GetX()) * (p1.GetX() - p2.GetX()) + (p1.GetY() - p2.GetY()) * (p1.GetY() - p2.GetY()) + (p1.GetZ() - p2.GetZ()) * (p1.GetZ() - p2.GetZ()));
}

double GeometryHelper::GetAngleVector(glm::vec3 p1, glm::vec3 p2, ArcType direction) // XZ plane
{
	double alfa = 0;

	if(direction == ArcType::Left1)
		alfa = 2 * M_PI - atan2(p2.z - p1.z, p2.x - p1.x);
	else if(direction == ArcType::Right1)
		alfa = atan2(p2.z - p1.z, p2.x - p1.x);
	
	return alfa;
}

ArcType GeometryHelper::GetVectorsDirection(glm::vec3 v1, glm::vec3 v2) // XZ plane
{
	glm::vec3 crossProduct = glm::cross(v1, v2);

	if (crossProduct.y < 0)
		return ArcType::Left1;
	if (crossProduct.y > 0)
		return ArcType::Right1;
	return ArcType::Undefined;
}

ArcType GeometryHelper::GetVectorsDirection(glm::vec3 p1, glm::vec3 p2, glm::vec3 commonPoint) // XZ plane
{
	return GeometryHelper::GetVectorsDirection(p1 - commonPoint, p2 - commonPoint);
}
