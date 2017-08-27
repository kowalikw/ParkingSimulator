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
	
	pathTmp = createArcsBetweenSegments(points);

	int iter = 0;
	int arc1, arc2;
	while (!checkArcsCorrectness(pathTmp, &arc1, &arc2) && iter < 100)
	{
		std::vector<glm::vec2> newPoints;
		for (int i = 0; i < points.size() - 2; i++)
		{
			if (!epsilonEquals(points[i].x, pathTmp.GetAt(arc2).arcBasePoints[1].x) || !epsilonEquals(points[i].y, pathTmp.GetAt(arc2).arcBasePoints[1].y))
				newPoints.push_back(points[i]);
			//else
			//	newPoints.push_back(pathTmp.GetAt(arc2).GetCirclePoint(pathTmp.GetAt(arc2).angleTo));
		}
		newPoints.push_back(points[(int)points.size() - 2]);
		newPoints.push_back(points[(int)points.size() - 1]);

		points = newPoints;
		pathTmp = createArcsBetweenSegments(newPoints);

		iter++;
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

bool PathPlanner::epsilonEquals(float f1, float f2)
{
	return abs(f2 - f1) < EPS;
}

Path PathPlanner::createArcsBetweenSegments(vector<glm::vec2> points)
{
	Path pathArcs;

	for (int i = 0; i < (int)points.size() - 2; i++)
	{
		std::vector<glm::vec2> arcBasePoints;
		for (int j = 0; j < 3; j++)
			arcBasePoints.push_back(points[i + j]);

		glm::vec2 p1 = points[i];
		glm::vec2 p2 = points[i + 1];
		glm::vec2 p3 = points[i + 2];

		glm::vec2 dir1 = p2 - p1;
		glm::vec2 dir2 = p3 - p2;

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

		pathArcs.AddElement(PathElement(center, radius, angleFrom, angleTo, arcBasePoints, arcType == Right1 ? Right : Left));
	}

	return pathArcs;
}

bool PathPlanner::checkArcsCorrectness(Path pathArcs, int *arc1, int *arc2)
{
	std::vector<PathElement> arcs = pathArcs.GetElements();
	for (int i = 0; i < (int)arcs.size() - 1; i++)
	{
		auto p1 = arcs[i].GetCirclePoint(arcs[i].angleTo);
		auto p2 = arcs[i + 1].GetCirclePoint(arcs[i + 1].angleFrom);

		if (p1 == p2) continue;

		auto dir1 = GeometryHelper::GetLineDirection(p1, p2);
		auto dir2 = GeometryHelper::GetLineDirection(arcs[i].arcBasePoints[1], arcs[i].arcBasePoints[2]);

		if (!epsilonEquals(dir1.x, dir2.x) || !epsilonEquals(dir1.y, dir2.y)) 
		{
			*arc1 = i;
			*arc2 = i + 1;
			return false;
		}
	}

	return true;
}

Path PathPlanner::createParkingPath(Vehicle vehicle, ParkingSpace parkingSpace)
{
	Path path;

	if (parkingSpace.GetType() == ParkingSpaceType::Paralell)
	{
		if (parkingSpace.GetSize().x < vehicle.GetWheelbase())
			return path;

		// zak³adamy, ¿e pojazd jest w tej samej orientacji co miejsce parkingowe


		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(vehicle.GetWheelbase() / 2.0) * vehicle.GetDirWheelbase()); // ustawienie auta na œrodku miejsca

		auto p1 = vehicle.GetPosition();

		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(parkingSpace.GetSize().x / 2.0) * vehicle.GetDirWheelbase()); // przesuniêcie auta do koñca miejsca

		auto p2 = vehicle.GetPosition();

		path.AddElement(PathElement(p1, p2));

		int arcCount = 0;
		double insideAngle = vehicle.GetMaxInsideAngle();
		double angleFrom = 0;
		double angleTo = 0;
		while (parkingSpace.ContainVehicle(vehicle)) // ³uki
		{
			PathElement circle = vehicle.GetTurnCircle(insideAngle, Left);

			while (circle.GetCirclePoint(angleTo).x < parkingSpace.GetPosition().x - vehicle.GetWheelbase() / 2.0)
				angleTo += 0.001;

			switch (arcCount % 4)
			{
			case 0:
				circle = vehicle.GetTurnCircle(insideAngle, Left, angleFrom, angleTo);
				break;
			case 1:
				circle = vehicle.GetTurnCircle(insideAngle, Right, 2 * M_PI - angleTo, 2 * M_PI + angleFrom);
				break;
			case 2:
				circle = vehicle.GetTurnCircle(insideAngle, Left, 2 * M_PI + angleFrom, 2 * M_PI - angleTo);
				break;
			case 3:
				circle = vehicle.GetTurnCircle(insideAngle, Right, angleTo, angleFrom);
				break;
			}

			/*switch (arcCount % 4)
			{
			case 0:
			circle = vehicle.GetTurnCircle(insideAngle, Right, angleFrom, angleTo);
			break;
			case 1:
			circle = vehicle.GetTurnCircle(insideAngle, Left, angleFrom, 2 * M_PI - angleTo);
			break;
			case 2:
			circle = vehicle.GetTurnCircle(insideAngle, Right, 2 * M_PI + angleFrom, 2 * M_PI - angleTo);
			break;
			case 3:
			circle = vehicle.GetTurnCircle(insideAngle, Left, angleTo, angleFrom);
			break;
			}*/

			arcCount++;
			path.AddElement(circle);
			vehicle.UpdateState(circle.GetCirclePoint(circle.angleTo), circle.angleTo);

			//if (arcCount == 2) break;
		}





		//}
	}
	else if (parkingSpace.GetType() == ParkingSpaceType::Perpendicular)
	{

	}

	return path;
}
