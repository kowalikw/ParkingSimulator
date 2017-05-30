#include "PathPlanner.h"

PathPlanner::PathPlanner()
{
}

PathPlanner::PathPlanner(Map map, Vehicle vehicle)
{
	this->map = map;
	this->vehicle = vehicle;
}

void PathPlanner::AddUserPoint(glm::vec2 userPoint)
{
	this->userPoints.push_back(userPoint);
}

void PathPlanner::SetUserPoints(std::vector<glm::vec2> userPoints)
{
	this->userPoints = userPoints;
}

std::vector<glm::vec2> PathPlanner::UserPoints()
{
	return this->userPoints;
}

std::vector<glm::vec2> PathPlanner::VoronoiPoints()
{
	return this->voronoiPoints;
}

Path PathPlanner::CreateAdmissiblePath(vector<glm::vec2> points)
{
	/*Path pathTmp, path;

	// path contains less than 3 points
	if (points.size() < 3)
	{
		for (int i = 0; i < points.size(); i++)
			path.AddElement(points[i]);
		return path;
	}

	for (int i = 0; i < points.size() - 2; i++)
	{
		glm::vec3 p1(points[i].GetX(), points[i].GetY(), points[i].GetZ());
		glm::vec3 p2(points[i + 1].GetX(), points[i + 1].GetY(), points[i + 1].GetZ());
		glm::vec3 p3(points[i + 2].GetX(), points[i + 2].GetY(), points[i + 2].GetZ());

		glm::vec3 dir1 = p2 - p1;
		glm::vec3 dir2 = p3 - p2;

		//double radius = vehicle.GetR(vehicle.GetMaxInsideAngle());

		double l1 = glm::length(dir1);
		double l2 = glm::length(dir2);

		dir1 = glm::normalize(dir1);
		dir2 = glm::normalize(dir2);

		double length = l1 > l2 ? l2 : l1;
		length -= 2;
		glm::vec3 P = p2 - (float)length * dir1;
		glm::vec3 K = p2 + (float)length * dir2;

		//glm::vec3 P = p2 - (float)radius * dir1;
		//glm::vec3 K = p2 + (float)radius * dir2;

		glm::vec3 n1(dir1.z, dir1.y, -dir1.x);
		glm::vec3 n2(dir2.z, dir2.y, -dir2.x);

		glm::vec3 P1 = P;
		glm::vec3 P2 = P + n1;
		glm::vec3 P3 = K;
		glm::vec3 P4 = K + n2;

		glm::vec3 center = GeometryHelper::GetLineIntersectionPoint(P1, P2, P3, P4);
		double radius = glm::length(P - center);
		ArcType arcType = GeometryHelper::GetVectorsDirection(p1, p3, p2);
		double angleFrom = GeometryHelper::GetAngleVector(p1, p2, arcType);
		double angleTo = GeometryHelper::GetAngleVector(p2, p3, arcType);

		if (angleTo < angleFrom)
			angleTo += 2 * M_PI;

		//path.AddElement(PathElement(p1, P1));
		pathTmp.AddElement(PathElement(center, radius, angleFrom, angleTo, arcType == Right1 ? Right : Left));
		//path.AddElement(PathElement(P3, p3));
	}

	for (int i = 0; i < pathTmp.GetElements().size(); i++)
	{
		if (i == 0)
			path.AddElement(PathElement(glm::vec3(points[0].GetX(), points[0].GetY(), points[0].GetZ()), glm::vec3(pathTmp.GetElements()[0].GetCirclePoint(pathTmp.GetElements()[0].GetAngleFrom()))));
		if (i == path.GetElements().size() - 1)
			path.AddElement(PathElement(glm::vec3(pathTmp.GetElements()[pathTmp.GetElements().size() - 1].GetCirclePoint(pathTmp.GetElements()[pathTmp.GetElements().size() - 1].GetAngleTo())), glm::vec3(points[points.size() - 1].GetX(), points[points.size() - 1].GetY(), points[points.size() - 1].GetZ())));

		path.AddElement(pathTmp.GetAt(i));

		if (i > 0)
			path.AddElement(PathElement(glm::vec3(pathTmp.GetAt(i - 1).GetCirclePoint(pathTmp.GetAt(i - 1).GetAngleTo())), glm::vec3(pathTmp.GetAt(i).GetCirclePoint(pathTmp.GetAt(i).GetAngleFrom()))));
	}


	return path;*/

	return Path();
}
