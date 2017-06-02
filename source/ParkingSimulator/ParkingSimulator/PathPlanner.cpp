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
	Path pathTmp, path;

	// path contains less than 3 points
	/*if (points.size() < 3)
	{
		for (int i = 0; i < points.size(); i++)
			path.AddElement(points[i]);
		return path;
	}*/
	for (int i = 0; i < (int)points.size() - 2; i++)
	{
		glm::vec2 p1 = points[i];
		glm::vec2 p2 = points[i + 1];
		glm::vec2 p3 = points[i + 2];

		glm::vec2 dir1 = p2 - p1;
		glm::vec2 dir2 = p3 - p2;

		//double radius = vehicle.GetR(vehicle.GetMaxInsideAngle());

		double l1 = glm::length(dir1);
		double l2 = glm::length(dir2);

		dir1 = glm::normalize(dir1);
		dir2 = glm::normalize(dir2);

		glm::vec2 n1(dir1.y, -dir1.x);
		glm::vec2 n2(dir2.y, -dir2.x);

		double length = l1 > l2 ? l2 : l1;
		double radiusMin = vehicle.GetRMin(vehicle.GetMaxInsideAngle());
		double radius;
		glm::vec2 center;
		do
		{
			glm::vec2 P = p2 - (float)length * dir1;
			glm::vec2 K = p2 + (float)length * dir2;

			glm::vec2 P1 = P;
			glm::vec2 P2 = P + n1;
			glm::vec2 P3 = K;
			glm::vec2 P4 = K + n2;

			center = GeometryHelper::GetLineIntersectionPoint(P1, P2, P3, P4);
			radius = glm::length(P - center);

			length -= 0.1;
		} while (radius >= radiusMin);

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
			path.AddElement(PathElement(points[0], glm::vec2(pathTmp.GetElements()[0].GetCirclePoint(pathTmp.GetElements()[0].GetAngleFrom()))));
		
		if (i == path.GetElements().size() - 1)
			path.AddElement(PathElement(glm::vec2(pathTmp.GetElements()[pathTmp.GetElements().size() - 1].GetCirclePoint(pathTmp.GetElements()[pathTmp.GetElements().size() - 1].GetAngleTo())), points[points.size() - 1]));

		path.AddElement(pathTmp.GetAt(i));

		if (i > 0)
			path.AddElement(PathElement(glm::vec2(pathTmp.GetAt(i - 1).GetCirclePoint(pathTmp.GetAt(i - 1).GetAngleTo())), glm::vec2(pathTmp.GetAt(i).GetCirclePoint(pathTmp.GetAt(i).GetAngleFrom()))));
	}


	return path;
}
