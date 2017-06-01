#include "GeometryHelper.h"

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

double GeometryHelper::GetDistanceBetweenPoints(Point p1, Point p2)
{
	return sqrt((p1.GetX() - p2.GetX()) * (p1.GetX() - p2.GetX()) + (p1.GetY() - p2.GetY()) * (p1.GetY() - p2.GetY()) + (p1.GetZ() - p2.GetZ()) * (p1.GetZ() - p2.GetZ()));
}

double GeometryHelper::GetAngleVector(glm::vec2 p1, glm::vec2 p2, ArcType direction)
{
	double alfa = 0;

	if(direction == ArcType::Left1)
		alfa = 2 * M_PI - atan2(p2.y - p1.y, p2.x - p1.x);
	else if(direction == ArcType::Right1)
		alfa = atan2(p2.y - p1.y, p2.x - p1.x);
	
	return alfa;
}

ArcType GeometryHelper::GetVectorsDirection(glm::vec2 v1, glm::vec2 v2)
{
	/*glm::vec2 crossProduct = glm::cross(v1, v2);

	if (crossProduct.y < 0)
		return ArcType::Left1;
	if (crossProduct.y > 0)
		return ArcType::Right1;*/
	return ArcType::Undefined;
}

ArcType GeometryHelper::GetVectorsDirection(glm::vec2 p1, glm::vec2 p2, glm::vec2 commonPoint)
{
	return GeometryHelper::GetVectorsDirection(p1 - commonPoint, p2 - commonPoint);
}
