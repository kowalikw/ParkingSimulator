#include "PathPlanner.h"

PathPlanner::PathPlanner()
{
	simulation = new Simulation();

	//temp
	/*this->map = new Map(567, 1024);
	map->AddMapElement(new Obstacle(glm::vec2(100, 50), glm::vec2(300, 400), Building, "lala"));

	vehicle = new Vehicle(200, 55);*/
}

PathPlanner::PathPlanner(Map map, Vehicle vehicle)
{
	this->map = &map;
	this->vehicle = &vehicle;
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

Path * PathPlanner::CreateAdmissiblePath(ParkingSpace * start, ParkingSpace * end)
{
	return nullptr;
}

Path * PathPlanner::CreateAdmissiblePath(ParkingSpace * start, glm::vec2 end)
{
	return nullptr;
}

Path * PathPlanner::CreateAdmissiblePath(glm::vec2 start, ParkingSpace * end)
{
	return nullptr;
}

Path * PathPlanner::CreateAdmissiblePath(glm::vec2 start, glm::vec2 end)
{
	float expandSize = expandSizePercent / 100.0 *  vehicle->GetTrack();

	expandedMap = map->GetExpandedMap(expandSize);

	int indexStart, indexEnd;
	glm::vec2 startDirection = *GetStartDirection();
	glm::vec2 endDirection = *GetEndDirection();
	Line* startLine = new Line(start, start + (float)vehicle->GetWheelbase() / 2.0f * startDirection);
	Line* endLine = new Line(end, end + (float)vehicle->GetWheelbase() / 2.0f * endDirection);

	voronoiGraph = new Graph(true);
	voronoiGraph->CreateVoronoiVisibilityFullGraph(expandedMap, startLine, endLine, &indexStart, &indexEnd);

	polylinePath = voronoiGraph->FindPath(indexStart, indexEnd);

	int element1, element2;
	while (!checkPolylinePathCorectness(polylinePath, &element1, &element2) && polylinePath->GetElements().size() > 1)
	{
		Line *line;
		if (element2 != polylinePath->GetElements().size() - 1)
			line = dynamic_cast<Line*>(polylinePath->GetAt(element2));
		else
			line = dynamic_cast<Line*>(polylinePath->GetAt(element1));

		voronoiGraph->RemoveEdge(line->GetV1(), line->GetV2());

		polylinePath = voronoiGraph->FindPath(indexStart, indexEnd);
	}

	finalPath = CreateAdmissiblePath(polylinePath);

	GraphEdge *collisionEdge = ChackPathCollision(finalPath, map);
	while (collisionEdge != NULL && finalPath->GetElements().size() > 0)
	{
		voronoiGraph->RemoveEdge(collisionEdge);

		polylinePath = voronoiGraph->FindPath(indexStart, indexEnd);

		finalPath = CreateAdmissiblePath(polylinePath);

		if (finalPath->GetElements().size() == 0)
			return nullptr;

		collisionEdge = ChackPathCollision(finalPath, map);
	}

	return nullptr;
}

Path * PathPlanner::CreateAdmissiblePath(Path * path)
{
	std::vector<glm::vec2> points;

	std::vector<PathElement*> pathElements = path->GetElements();
	for (int i = 0; i < pathElements.size(); i++)
	{
		if (i == 0)
			points.push_back(dynamic_cast<Line*>(pathElements[i])->GetFrom());
		points.push_back(dynamic_cast<Line*>(pathElements[i])->GetTo());
	}

	return CreateAdmissiblePath(points);
}

Path * PathPlanner::CreateAdmissiblePath(vector<glm::vec2> points)
{
	Path *pathTmp = new Path();
	Path *path = new Path();

	// path contains less than 3 points
	/*if (points.size() < 3)
	{
		for (int i = 0; i < points.size(); i++)
			path.AddElement(points[i]);
		return path;
	}*/

	if (pathPlanningAlgorithm == PathPlanningAlgorithm::Spline)
	{
		path->AddElement(new BSpline(points));

		return path;
	}
	else
	{
		pathTmp = createArcsBetweenSegments(points);

		int iter = 0;
		int arc1, arc2;
		/*while (!checkArcsCorrectness(pathTmp, &arc1, &arc2) && iter < 100)
		{
			std::vector<glm::vec2> newPoints;
			for (int i = 0; i < points.size() - 2; i++)
			{
				Circle *circle = dynamic_cast<Circle*>(pathTmp->GetAt(arc2));
				if (!epsilonEquals(points[i].x, circle->circleBasePoints[1].x) || !epsilonEquals(points[i].y, circle->circleBasePoints[1].y))
					newPoints.push_back(points[i]);
			}
			newPoints.push_back(points[(int)points.size() - 2]);
			newPoints.push_back(points[(int)points.size() - 1]);

			points = newPoints;
			pathTmp = createArcsBetweenSegments(newPoints);

			iter++;
		}*/

		/*Path * newPathTmp = new Path();
		for (int i = 0; i < pathTmp->GetElements().size(); i++)
		{
			std::vector<int> invalidArcs;
			invalidArcs.push_back(i);
			while (i < pathTmp->GetElements().size() - 1 && !checkArcsCorrectness(pathTmp, i, i + 1))
			{
				invalidArcs.push_back(i + 1);

				if (invalidArcs.size() < 4)
					i++;
				else
					break;
			}

			//tutaj naprawianie
			if (invalidArcs.size() == 4)
			{
				Circle *circle1 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[0]));
				Circle *circle2 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[1]));
				Circle *circle3 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[2]));
				Circle *circle4 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[3]));

				std::vector<glm::vec2> bsplinePoints;
				bsplinePoints.push_back(circle1->GetPoint(1));
				bsplinePoints.push_back(circle1->GetPoint(1));
				bsplinePoints.push_back(circle1->GetPoint(1));
				bsplinePoints.push_back(circle2->GetCircleBasePoints()[1]);
				bsplinePoints.push_back(circle3->GetCircleBasePoints()[1]);
				bsplinePoints.push_back(circle4->GetPoint(0));
				bsplinePoints.push_back(circle4->GetPoint(0));
				bsplinePoints.push_back(circle4->GetPoint(0));

				BSpline *bspline = new BSpline(bsplinePoints);
				newPathTmp->AddElement(bspline);
			}
			else if (invalidArcs.size() == 3)
			{
				Circle *circle1 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[0]));
				Circle *circle2 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[1]));
				Circle *circle3 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[2]));

				std::vector<glm::vec2> bsplinePoints;
				bsplinePoints.push_back(circle1->GetPoint(0));
				bsplinePoints.push_back(circle1->GetPoint(0));
				bsplinePoints.push_back(circle1->GetPoint(0));
				bsplinePoints.push_back(circle1->GetCircleBasePoints()[1]);
				bsplinePoints.push_back(circle2->GetCircleBasePoints()[1]);
				bsplinePoints.push_back(circle3->GetPoint(0));
				bsplinePoints.push_back(circle3->GetPoint(0));
				bsplinePoints.push_back(circle3->GetPoint(0));

				BSpline *bspline = new BSpline(bsplinePoints);
				newPathTmp->AddElement(bspline);
			}
			else if (invalidArcs.size() == 2)
			{
				Circle *circle1 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[0]));
				Circle *circle2 = dynamic_cast<Circle*>(pathTmp->GetAt(invalidArcs[1]));

				std::vector<glm::vec2> bsplinePoints;
				bsplinePoints.push_back(circle1->GetPoint(0));
				bsplinePoints.push_back(circle1->GetPoint(0));
				bsplinePoints.push_back(circle1->GetPoint(0));
				bsplinePoints.push_back(circle1->GetCircleBasePoints()[1]);
				bsplinePoints.push_back(circle2->GetCircleBasePoints()[1]);
				bsplinePoints.push_back(circle2->GetPoint(1));
				bsplinePoints.push_back(circle2->GetPoint(1));
				bsplinePoints.push_back(circle2->GetPoint(1));

				BSpline *bspline = new BSpline(bsplinePoints);
				newPathTmp->AddElement(bspline);
			}
			else
			{
				newPathTmp->AddElement(pathTmp->GetAt(i));
			}
		}*/

		Path *newPathTmp = pathTmp;

		for (int i = 0; i < newPathTmp->GetElements().size(); i++)
		{
			if (i == 0)
			{
				path->AddElement(new Line(points[0], newPathTmp->GetAt(0)->GetFirstPoint()));
			}

			if (i > 0)
			{
				path->AddElement(new Line(newPathTmp->GetAt(i - 1)->GetLastPoint(), newPathTmp->GetAt(i)->GetFirstPoint()));
			}

			path->AddElement(newPathTmp->GetAt(i));

			if (i == (int)newPathTmp->GetElements().size() - 1)
			{
				path->AddElement(new Line(newPathTmp->GetAt(newPathTmp->GetElements().size() - 1)->GetLastPoint(), points[points.size() - 1]));
			}
		}

		return path;
	}
}

void PathPlanner::NewSimulation()
{
	simulation = new Simulation();

	simulation->SetMap(map);
	simulation->SetVehicle(vehicle);
	simulation->SetFinalPath(CreateAdmissiblePath(UserPoints()));
}

void PathPlanner::OpenSimulation(string filePath)
{
	simulation = new Simulation();

	std::ifstream ifs(filePath);

	boost::archive::text_iarchive ia(ifs);

	ia >> *simulation;
}

void PathPlanner::SaveSimulation(string filePath)
{
	simulation = new Simulation();
	simulation->SetMap(map);
	simulation->SetExpandedMap(expandedMap);
	simulation->SetVehicle(vehicle);
	simulation->SetPolylinePath(polylinePath);
	simulation->SetFinalPath(finalPath);
	simulation->SetStartPoint(startPoint);
	simulation->SetEndPoint(endPoint);
	simulation->SetStartDirection(startDirection);
	simulation->SetEndDirection(endDirection);
	simulation->SetStartParkingSpace(startParkingSpace);
	simulation->SetEndParkingSpace(endParkingSpace);
	simulation->SetExpandSize(expandSizePercent);
	simulation->SetCollisionDetectionDensity(collisionDetectionDensity);
	simulation->SetPathPlanningAlgorithm(pathPlanningAlgorithm);
	simulation->SetUseAdmissibleArcsOnly(useAdmissibleArcsOnly);

	ofstream f(filePath, ios::out);

	boost::archive::text_oarchive oa(f);

	oa << *simulation;
}

double PathPlanner::GetExpandSizePercent()
{
	return this->expandSizePercent;
}

void PathPlanner::SetExpandSizePercent(double expandSizePercent)
{
	this->expandSizePercent = expandSizePercent;
}

double PathPlanner::GetCollisionDetectionDensity()
{
	return this->collisionDetectionDensity;
}

void PathPlanner::SetCollisionDetectionDensity(double collisionDetectionDensity)
{
	this->collisionDetectionDensity = collisionDetectionDensity;
}

PathPlanningAlgorithm PathPlanner::GetAlgorithm()
{
	return this->pathPlanningAlgorithm;
}

void PathPlanner::SetAlgorithm(PathPlanningAlgorithm algorithm)
{
	this->pathPlanningAlgorithm = algorithm;
}

bool PathPlanner::GetUseAdmissibleArcsOnly()
{
	return this->useAdmissibleArcsOnly;
}

void PathPlanner::SetUseAdmissibleArcsOnly(bool useAdmissibleArcsOnly)
{
	this->useAdmissibleArcsOnly = useAdmissibleArcsOnly;
}

GraphEdge * PathPlanner::ChackPathCollision(Path * path, Map * Map)
{
	double dt = 1.0 / collisionDetectionDensity;
	auto mapElements = map->GetMapElements();
	for (double t = 0; t < 1; t += dt)
	{
		SimulationState simulationState = path->GetSimulationState(t);
		vehicle->UpdateState(simulationState);
		for (int i = 0; i < mapElements.size(); i++)
		{
			if (GeometryHelper::CheckPolygonIntersection(mapElements[i]->GetPoints(), vehicle->GetPoints()))
			{
				Line *line = dynamic_cast<Line*>(polylinePath->GetElement(t));
				return voronoiGraph->GetEdge(line->GetV1(), line->GetV2());
			}
		}
	}
	return nullptr;
}

bool PathPlanner::epsilonEquals(float f1, float f2)
{
	return abs(f2 - f1) < EPS;
}

Path * PathPlanner::createArcsBetweenSegments(vector<glm::vec2> points)
{
	Path *pathArcs = new Path();

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
		double radiusMin = vehicle->GetRMin(vehicle->maxInsideAngle);
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
		} while (radius - RADIUS_EPS > radiusMin); // minimalizacja promienia

		while (useAdmissibleArcsOnly && radius + RADIUS_EPS < radiusMin)
		{
			glm::vec2 P = p2 - (float)length * dir1;
			glm::vec2 K = p2 + (float)length * dir2;

			glm::vec2 P1 = P;
			glm::vec2 P2 = P + n1;
			glm::vec2 P3 = K;
			glm::vec2 P4 = K + n2;

			center = GeometryHelper::GetLineIntersectionPoint(P1, P2, P3, P4);
			radius = glm::length(P - center);

			length += 0.1;
		}

		CircleType arcType = GeometryHelper::GetVectorsDirection(p1, p3, p2);
		double angleFrom = GeometryHelper::GetAngleVector(p1, p2, arcType);
		double angleTo = GeometryHelper::GetAngleVector(p2, p3, arcType);

		if (angleTo < angleFrom)
			angleTo += 2 * M_PI;

		pathArcs->AddElement(new Circle(center, radius, angleFrom, angleTo, arcBasePoints, arcType));
	}

	return pathArcs;
}

bool PathPlanner::checkArcsCorrectness(Path *pathArcs, int *arc1, int *arc2)
{
	std::vector<PathElement*> arcs = pathArcs->GetElements();
	for (int i = 0; i < (int)arcs.size() - 1; i++)
	{
		Circle *circle = dynamic_cast<Circle*>(arcs[i]);
		Circle *nextCircle = dynamic_cast<Circle*>(arcs[i + 1]);

		auto p1 = circle->GetPointForAngle(circle->angleTo);
		auto p2 = nextCircle->GetPointForAngle(nextCircle->angleFrom);

		if (p1 == p2) continue;

		auto dir1 = GeometryHelper::GetLineDirection(p1, p2);
		auto dir2 = GeometryHelper::GetLineDirection(circle->circleBasePoints[1], circle->circleBasePoints[2]);

		if (!epsilonEquals(dir1.x, dir2.x) || !epsilonEquals(dir1.y, dir2.y)) 
		{
			*arc1 = i;
			*arc2 = i + 1;
			return false;
		}
	}

	return true;
}

bool PathPlanner::checkArcsCorrectness(Path *pathArcs, int arc1, int arc2)
{
	std::vector<PathElement*> arcs = pathArcs->GetElements();

	Circle *circle = dynamic_cast<Circle*>(arcs[arc1]);
	Circle *nextCircle = dynamic_cast<Circle*>(arcs[arc2]);

	auto p1 = circle->GetPointForAngle(circle->angleTo);
	auto p2 = nextCircle->GetPointForAngle(nextCircle->angleFrom);

	if (p1 == p2) return true;

	auto dir1 = GeometryHelper::GetLineDirection(p1, p2);
	auto dir2 = GeometryHelper::GetLineDirection(circle->circleBasePoints[1], circle->circleBasePoints[2]);

	if (!epsilonEquals(dir1.x, dir2.x) || !epsilonEquals(dir1.y, dir2.y))
		return false;

	return true;
}

bool PathPlanner::checkPolylinePathCorectness(Path *path, int * element1, int * element2)
{
	if (path->GetElements().size() < 2) return true;

	for (int i = 0; i < path->GetElements().size() - 2; i++)
	{
		Line *line1 = dynamic_cast<Line*>(path->GetElements()[i]);
		Line *line2 = dynamic_cast<Line*>(path->GetElements()[i + 1]);

		glm::vec2 v1 = line1->GetFrom() - line1->GetTo();
		glm::vec2 v2 = line2->GetFrom() - line2->GetTo();

		if (GeometryHelper::GetAngleBetweenVectors(v1, v2) > 9 * M_PI / 10.0f)
		{
			*element1 = i;
			*element2 = i + 1;
			return false;
		}
	}

	return true;
}

Map * PathPlanner::GetMap()
{
	return this->map;
}

Map * PathPlanner::GetExpandedMap()
{
	return this->expandedMap;
}

Vehicle * PathPlanner::GetVehicle()
{
	return this->vehicle;
}

Path * PathPlanner::GetPolylinePath()
{
	return this->polylinePath;
}

Path * PathPlanner::GetFinalPath()
{
	return this->finalPath;
}

Graph * PathPlanner::GetVoronoiGraph()
{
	return this->voronoiGraph;
}

Graph * PathPlanner::GetFullVoronoiVisibilityGraph()
{
	return this->fullVoronoiVisibilityGraph;
}

Simulation * PathPlanner::GetSimulation()
{
	simulation = new Simulation(map, vehicle, finalPath);

	return this->simulation;
}

glm::vec2 * PathPlanner::GetStartPoint()
{
	return this->startPoint;
}

glm::vec2 * PathPlanner::GetEndPoint()
{
	return this->endPoint;
}

void PathPlanner::SetStartPoint(glm::vec2 * startPoint)
{
	this->startPoint = startPoint;
}

void PathPlanner::SetEndPoint(glm::vec2 * endPoint)
{
	this->endPoint = endPoint;
}

glm::vec2 * PathPlanner::GetStartDirection()
{
	return this->startDirection;
}

glm::vec2 * PathPlanner::GetEndDirection()
{
	return this->endDirection;
}

void PathPlanner::SetStartDirection(glm::vec2 * startDirection)
{
	this->startDirection = startDirection;
}

void PathPlanner::SetEndDrection(glm::vec2 * endDirection)
{
	this->endDirection = endDirection;
}

ParkingSpace * PathPlanner::GetStartParkingSpace()
{
	return this->startParkingSpace;
}

ParkingSpace * PathPlanner::GetEndParkingSpace()
{
	return this->endParkingSpace;
}

void PathPlanner::SetStartParkingSpace(ParkingSpace * startParkingSpace)
{
	this->startParkingSpace = startParkingSpace;
}

void PathPlanner::SetEndParkingSpace(ParkingSpace * endParkingSpace)
{
	this->endParkingSpace = endParkingSpace;
}

void PathPlanner::SetMap(Map * map)
{
	this->map = map;
}

void PathPlanner::SetVehicle(Vehicle * vehicle)
{
	this->vehicle = vehicle;
}

Path * PathPlanner::createParkingPath(Vehicle vehicle, ParkingSpace parkingSpace, ParkManeuverType parkManeuverType)
{
	PathElement *lastElement;
	Path *path = new Path();

	if (parkingSpace.GetType() == ParkingSpaceType::Paralell)
	{
		if (parkingSpace.GetSize().x < vehicle.GetWheelbase())
			return path;

		// zak³adamy, ¿e pojazd jest w tej samej orientacji co miejsce parkingowe


		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(vehicle.GetWheelbase() / 2.0) * vehicle.GetDirWheelbase()); // ustawienie auta na œrodku miejsca

		auto p1 = vehicle.GetPosition();

		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(parkingSpace.GetSize().x / 2.0) * vehicle.GetDirWheelbase()); // przesuniêcie auta do koñca miejsca

		auto p2 = vehicle.GetPosition();

		path->AddElement(new Line(p1, p2));

		int arcCount = 0;
		double insideAngle = vehicle.GetMaxInsideAngle();
		double angleFrom = 0;
		double angleTo = 0;
		while (parkingSpace.ContainVehicle(vehicle)) // ³uki
		{
			Circle* circle = vehicle.GetTurnCircle(insideAngle, Left);

			auto pp = parkingSpace.GetPosition().x - vehicle.GetWheelbase() / 2.0;
			while (circle->GetPointForAngle(angleTo).x < pp)
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
				circle = vehicle.GetTurnCircle(insideAngle, Left, 2 * M_PI + angleFrom, 2 * M_PI - angleTo, ManeuverType::Back);
				break;
			case 3:
				circle = vehicle.GetTurnCircle(insideAngle, Right, angleTo, angleFrom, ManeuverType::Back);
				break;
			}

			arcCount++;
			path->AddElement(circle);
			vehicle.UpdateState(circle->GetPointForAngle(circle->angleTo), circle->angleTo);
		}
	}
	else if (parkingSpace.GetType() == ParkingSpaceType::Perpendicular)
	{
		// TODO:
	}

	path->AddElement(new Line(vehicle.GetPosition(), vehicle.GetPosition() + (float)vehicle.GetWheelbase() * vehicle.GetDirWheelbase()));

	glm::vec2 pathTranslation = glm::vec2((float)vehicle.GetWheelbase() / 2.0f, 0.0f);
	auto pathElements = path->GetElements();
	for (int i = 0; i < pathElements.size(); i++)
	{
		if (dynamic_cast<Line*>(pathElements[i]) != NULL)
		{
			Line *line = dynamic_cast<Line*>(pathElements[i]);
			line->SetFrom(line->GetFrom() + pathTranslation);
			line->SetTo(line->GetTo() + pathTranslation);
		}
		else if (dynamic_cast<Circle*>(pathElements[i]) != NULL)
		{
			Circle *circle = dynamic_cast<Circle*>(pathElements[i]);
			circle->center = circle->center + pathTranslation;
		}
	}

	if (parkManeuverType == ParkManeuverType::Exit)
	{
		std::vector<PathElement*> reversedPathElements;
		std::vector<PathElement*> pathElements = path->GetElements();
		
		for (int i = pathElements.size() - 1; i >= 0; i--)
		{
			ManeuverType maneuverType = pathElements[i]->GetManeuverType();
			pathElements[i]->SetManeuverType(maneuverType == Front ? Back : Front);
			reversedPathElements.push_back(pathElements[i]);
		}

		path->SetElements(reversedPathElements);
	}

	return path;
}

bool PathPlanner::GetSetStartPosition()
{
	return this->setStartPosition;
}

bool PathPlanner::GetSetEndPosition()
{
	return this->setEndPosition;
}

bool PathPlanner::GetSetStartDirection()
{
	return this->setStartDirection;
}

bool PathPlanner::GetSetEndDirection()
{
	return this->setEndDirection;
}

bool PathPlanner::GetStartPositionChanged()
{
	return this->startPositionChanged;
}

bool PathPlanner::GetEndPositionChanged()
{
	return this->endPositionChanged;
}

bool PathPlanner::GetShowVoronoiGraph()
{
	return this->showVoronoiGraph;
}

bool PathPlanner::GetShowFullVoronoiVisibilityGraph()
{
	return this->showFullVoronoiVisibilityGraph;
}

bool PathPlanner::GetShowPolylinePath()
{
	return this->showPolylinePath;
}

bool PathPlanner::GetShowFinalPath()
{
	return this->showFinalPath;
}

bool PathPlanner::GetShowExpandedObstacles()
{
	return this->showExpandedObstacles;
}

void PathPlanner::SetSetStartPosition(bool setStartPosition)
{
	this->setStartPosition = setStartPosition;
}

void PathPlanner::SetSetEndPosition(bool setEndPosition)
{
	this->setEndPosition = setEndPosition;
}

void PathPlanner::SetSetStartDirection(bool setStartDirection)
{
	this->setStartDirection = setStartDirection;
}

void PathPlanner::SetSetEndDirection(bool setEndDirection)
{
	this->setEndDirection = setEndDirection;
}

void PathPlanner::SetStartPositionChanged(bool startPositionChanged)
{
	this->startPositionChanged = startPositionChanged;
}

void PathPlanner::SetEndPositionChanged(bool endPositionChanged)
{
	this->endPositionChanged = endPositionChanged;
}

void PathPlanner::SetShowVoronoiGraph(bool showVoronoiGraph)
{
	this->showVoronoiGraph = showVoronoiGraph;
}

void PathPlanner::SetShowFullVoronoiVisibilityGraph(bool showFullVoronoiVisibilityGraph)
{
	this->showFullVoronoiVisibilityGraph = showFullVoronoiVisibilityGraph;
}

void PathPlanner::SetShowPolylinePath(bool showPolylinePath)
{
	this->showPolylinePath = showPolylinePath;
}

void PathPlanner::SetShowFinalPath(bool showFinalPath)
{
	this->showFinalPath = showFinalPath;
}

void PathPlanner::SetShowExpandedObstacles(bool showExpandedObstacles)
{
	this->showExpandedObstacles = showExpandedObstacles;
}

MapElement * PathPlanner::GetHoverElement(glm::vec2 mousePosition)
{
	MapElement *hoverElement = nullptr;
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
		{
			if (mapElements[i] != hoverElement && (mapElements[i]->IsMoveActive() || mapElements[i]->IsRotationActive() || mapElements[i]->IsResizeActive()))
				return nullptr;
			if (GeometryHelper::CheckPolygonContainsPoint(mapElements[i]->GetPoints(), mousePosition))
				hoverElement = mapElements[i];
		}
	}
	return hoverElement;
}

void PathPlanner::FindPath(int *error)
{
	*error = 0;
	if (startPoint != nullptr && endPoint != nullptr)
	{
		CreateAdmissiblePath(*startPoint, *endPoint);
	}
	else if (startPoint != nullptr && endParkingSpace != nullptr)
	{
		CreateAdmissiblePath(*startPoint, endParkingSpace);
	}
	else if (startParkingSpace != nullptr && endPoint != nullptr)
	{
		CreateAdmissiblePath(startParkingSpace, *endPoint);
	}
	else if (startParkingSpace != nullptr && endParkingSpace != nullptr)
	{
		CreateAdmissiblePath(startParkingSpace, endParkingSpace);
	}
	else
	{
		*error = 1;
	}
}
