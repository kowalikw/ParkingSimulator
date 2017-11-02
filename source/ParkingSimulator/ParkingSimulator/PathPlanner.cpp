#include "PathPlanner.h"

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

#include <windows.h>

PathPlanner::PathPlanner()
{
	simulation = new Simulation();
}

PathPlanner::PathPlanner(PathPlanner & pathPlanner)
{
	this->map = pathPlanner.map;
	this->vehicle = pathPlanner.vehicle;
	this->startPoint = pathPlanner.startPoint;
	this->startDirection = pathPlanner.startDirection;
	this->startParkingSpace = pathPlanner.startParkingSpace;
	this->endPoint = pathPlanner.endPoint;
	this->endDirection = pathPlanner.endDirection;
	this->endParkingSpace = pathPlanner.endParkingSpace;
	this->pathPlanningAlgorithm = pathPlanner.pathPlanningAlgorithm;
	this->graphExtraVerticesAlong = pathPlanner.graphExtraVerticesAlong;
	this->graphExtraVerticesAcross = pathPlanner.graphExtraVerticesAcross;
	this->expandSizePercent = pathPlanner.expandSizePercent;
	this->useAdmissibleArcsOnly = pathPlanner.useAdmissibleArcsOnly;
	this->collisionDetectionDensity = pathPlanner.collisionDetectionDensity;
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
	Path *path;

	Path *pStart = createParkingPath(*vehicle, *start, Entry);
	Path *pEnd = createParkingPath(*vehicle, *end, Exit);

	parkingPathStart = pStart;
	parkingPathEnd = pEnd;

	pEnd->Reverse();
	if (pStart == nullptr || pEnd == nullptr || ChackPathCollision(pStart, map, false, 0, 0, start) != nullptr || ChackPathCollision(pEnd, map, false, 0, 0, end, true) != nullptr)
		return nullptr;
	pEnd->Reverse();

	Line* startLine = dynamic_cast<Line*>(pStart->GetLastElement());
	Line* endLine = dynamic_cast<Line*>(pEnd->GetFirstElement());

	pStart->RemoveElement(startLine);

	path = CreateAdmissiblePath(startLine, endLine);

	std::vector<Path*> pathParts;
	pathParts.push_back(pStart);
	pathParts.push_back(path);
	pathParts.push_back(pEnd);

	finalPath = new Path(pathParts);

	return finalPath;
}

Path * PathPlanner::CreateAdmissiblePath(ParkingSpace * start, glm::vec2 end)
{
	Path *pStart = createParkingPath(*vehicle, *start, Entry);

	if (pStart == nullptr || ChackPathCollision(pStart, map, false, 0, 0, start) != nullptr)
		return nullptr;

	parkingPathStart = pStart;

	Line* startLine = dynamic_cast<Line*>(pStart->GetLastElement());
	glm::vec2 endDirection = *GetEndDirection();
	Line* endLine = new Line(end - (float)vehicle->GetWheelbase() / 2.0f * endDirection, end);

	pStart->RemoveElement(pStart->GetLastElement());
	Path *path = CreateAdmissiblePath(startLine, endLine);

	std::vector<Path*> pathParts;
	pathParts.push_back(pStart);
	pathParts.push_back(path);

	finalPath = new Path(pathParts);

	return finalPath;
}

Path * PathPlanner::CreateAdmissiblePath(glm::vec2 start, ParkingSpace * end)
{
	Path *pEnd = createParkingPath(*vehicle, *end, Exit);

	pEnd->Reverse();
	if (pEnd == nullptr || ChackPathCollision(pEnd, map, false, 0, 0, end) != nullptr)
		return nullptr;
	pEnd->Reverse();

	parkingPathEnd = pEnd;

	if (pEnd == nullptr)
		return nullptr;

	glm::vec2 startDirection = *GetStartDirection();
	Line* startLine = new Line(start, start + (float)vehicle->GetWheelbase() / 2.0f * startDirection);
	Line* endLine = dynamic_cast<Line*>(pEnd->GetFirstElement());

	Path *path = CreateAdmissiblePath(startLine, endLine);

	std::vector<Path*> pathParts;
	pathParts.push_back(path);
	pathParts.push_back(pEnd);

	finalPath = new Path(pathParts);

	return finalPath;
}

Path * PathPlanner::CreateAdmissiblePath(glm::vec2 start, glm::vec2 end)
{
	glm::vec2 startDirection = *GetStartDirection();
	glm::vec2 endDirection = *GetEndDirection();
	Line* startLine = new Line(start, start + (float)vehicle->GetSize().x / 2.0f * startDirection);
	Line* endLine = new Line(end - (float)vehicle->GetSize().x / 2.0f * endDirection, end);

	return CreateAdmissiblePath(startLine, endLine);
}

Path * PathPlanner::CreateAdmissiblePath(Line *startLine, Line *endLine)
{
	Path *path;

	float expandSize = expandSizePercent / 100.0 *  vehicle->GetTrack();

	expandedMap = map->GetExpandedMap(expandSize);

	int indexStart, indexEnd;

	voronoiGraph = new Graph(true);
	voronoiGraph->CreateVoronoiVisibilityFullGraph(expandedMap, startLine, endLine, &indexStart, &indexEnd, true, graphExtraVerticesAlong, graphExtraVerticesAcross);

	//œcie¿ka bezpoœrednia

	GraphEdge *collisionEdge = nullptr;

	polylinePath = createDirectPolylinePath(startLine, endLine);

	if (polylinePath != nullptr)
	{
		finalPath = CreateAdmissiblePath(polylinePath);

		collisionEdge = ChackPathCollision(finalPath, map, false);
	}

	if (collisionEdge != nullptr || polylinePath == nullptr)
	{
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

		GraphEdge *collisionEdge = ChackPathCollision(finalPath, map, true, indexStart, indexEnd);

		//collisionEdge = nullptr;

		//TODO: nie usuwaæ pocz¹tkowej ani koñcowaj jeœli nie jest to niezbêdne!!!

		int i = 0;
		while (collisionEdge != NULL && finalPath->GetElements().size() > 0)
		{
			i++;
			/*std::ostringstream ss;
			ss << "V1: " << collisionEdge->v1->x << endl;
			ss << "V2: " << collisionEdge->v2->x << endl;
			ss << endl;
			std::string s(ss.str());

			OutputDebugStringA(s.c_str());*/

			voronoiGraph->RemoveEdge(collisionEdge);

			delete polylinePath;
			delete finalPath;
			delete collisionEdge;

			polylinePath = voronoiGraph->FindPath(indexStart, indexEnd);
			finalPath = CreateAdmissiblePath(polylinePath);

			if (finalPath == nullptr || finalPath->GetElements().size() == 0)
				return nullptr;

			collisionEdge = ChackPathCollision(finalPath, map, true, indexStart, indexEnd);
		}
	}

	return finalPath;
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

	if (points.size() == 0)
		return nullptr;

	return CreateAdmissiblePath(points);
}

Path * PathPlanner::CreateAdmissiblePath(vector<glm::vec2> points)
{
	Path *pathTmp = new Path();
	Path *path = new Path();

	// path contains less than 3 points
	if (points.size() < 3)
		return nullptr;

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

		if (path->GetElements().size() == 0)
			return nullptr;

		return path;
	}
}

void PathPlanner::NewSimulation()
{
	simulation = new Simulation();

	map = NULL;
	expandedMap = NULL;
	vehicle = NULL;
	polylinePath = NULL;
	parkingPathStart = NULL;
	parkingPathEnd = NULL;
	finalPath = NULL;
	startPoint = NULL;
	endPoint = NULL;
	startDirection = NULL;
	endDirection = NULL;
	startParkingSpace = NULL;
	endParkingSpace = NULL;
	expandSizePercent = 0;
	collisionDetectionDensity = 0;
	pathPlanningAlgorithm = PathPlanningAlgorithm::Spline;
	useAdmissibleArcsOnly = false;
}

void PathPlanner::OpenSimulation(string filePath)
{
	simulation = new Simulation();

	std::ifstream ifs(filePath);

	boost::archive::text_iarchive ia(ifs);

	ia >> *simulation;

	map = simulation->GetMap() != NULL ? simulation->GetMap() : nullptr;
	expandedMap = simulation->GetExpandedMap() != NULL ? simulation->GetExpandedMap() : nullptr;
	vehicle = simulation->GetVehicle();
	polylinePath = simulation->GetPolylinePath();
	parkingPathStart = simulation->GetParkingPathStart();
	parkingPathEnd = simulation->GetParkingPathEnd();
	finalPath = simulation->GetFinalPath();
	startPoint = simulation->GetStartPoint();
	endPoint = simulation->GetEndPoint();
	startDirection = simulation->GetStartDirection();
	endDirection = simulation->GetEndDirection();
	startParkingSpace = simulation->GetStartParkingSpace();
	endParkingSpace = simulation->GetEndParkingSpace();
	expandSizePercent = simulation->GetExpandSize();
	collisionDetectionDensity = simulation->GetCollisionDetectionDensity();
	pathPlanningAlgorithm = simulation->GetPathPlanningAlgorithm();
	useAdmissibleArcsOnly = simulation->GetUseAdmissibleArcsOnly();
}

void PathPlanner::SaveSimulation(string filePath)
{
	simulation = new Simulation();
	simulation->SetMap(map);
	simulation->SetExpandedMap(expandedMap);
	simulation->SetVehicle(vehicle);
	simulation->SetPolylinePath(polylinePath);
	simulation->SetParkingPathStart(parkingPathStart);
	simulation->SetParkingPathEnd(parkingPathEnd);
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

int PathPlanner::GetGraphExtraVerticesAlong()
{
	return this->graphExtraVerticesAlong;
}

void PathPlanner::SetGraphExtraVerticesAlong(int graphExtraVerticesAlong)
{
	this->graphExtraVerticesAlong = graphExtraVerticesAlong;
}

int PathPlanner::GetGraphExtraVerticesAcross()
{
	return this->graphExtraVerticesAcross;
}

void PathPlanner::SetGraphExtraVerticesAcross(int graphExtraVerticesAcross)
{
	this->graphExtraVerticesAcross = graphExtraVerticesAcross;
}

GraphEdge * PathPlanner::ChackPathCollision(Path * path, Map * Map, bool useGraph, int start, int end, MapElement * exceptionElement, bool invertPath)
{
	double dt = 1.0 / collisionDetectionDensity;
	auto mapElements = map->GetMapElements();
	for (double t = 0; t < 1; t += dt)
	{
		SimulationState simulationState = path->GetSimulationState(t);
		vehicle->UpdateState(simulationState);

		auto vehiclePoints = vehicle->GetPoints2();
		if (!GeometryHelper::CheckPolygonContainsPolygon(map->GetPoints(), vehicle->GetPoints2())) // check whether vehicle is all on the map
		{
			if (useGraph && polylinePath != nullptr)
			{
				Line *line = dynamic_cast<Line*>(polylinePath->GetElement(t));

				if (line->GetV1() == start || line->GetV2() == start)
					line = dynamic_cast<Line*>(polylinePath->GetNextElement(line));
				if (line->GetV2() == end || line->GetV1() == end)
					line = dynamic_cast<Line*>(polylinePath->GetPrevElement(line));

				return voronoiGraph->GetEdge(line->GetV1(), line->GetV2()); //TODO: Sprawdzic czy to dzia³a
			}
			return new GraphEdge();
		}

		for (int i = 0; i < mapElements.size(); i++)
		{
			if (exceptionElement != nullptr)
			{
				if (mapElements[i]->GetPosition() == exceptionElement->GetPosition() &&
					mapElements[i]->GetRotation() == exceptionElement->GetRotation() &&
					mapElements[i]->GetSize() == exceptionElement->GetSize())
					continue;
			}

			if (GeometryHelper::CheckPolygonIntersection(mapElements[i]->GetPoints(), vehicle->GetPoints2()))
			{
				if (useGraph && polylinePath != nullptr)
				{
					Line *line = dynamic_cast<Line*>(polylinePath->GetElement(t)); //TODO: use graph ?

					if (line->GetV1() == start)
						line = dynamic_cast<Line*>(polylinePath->GetNextElement(line));
					if (line->GetV2() == end)
						line = dynamic_cast<Line*>(polylinePath->GetPrevElement(line));

					return voronoiGraph->GetEdge(line->GetV1(), line->GetV2());
				}
				return new GraphEdge();
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

Vehicle * PathPlanner::GetVehicleStart()
{
	return this->vehicleStart;
}

Vehicle * PathPlanner::GetVehicleEnd()
{
	return this->vehicleEnd;
}

Path * PathPlanner::GetPolylinePath()
{
	return this->polylinePath;
}

Path * PathPlanner::GetParkingPathStart()
{
	return this->parkingPathStart;
}

Path * PathPlanner::GetParkingPathEnd()
{
	return this->parkingPathEnd;
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

bool PathPlanner::GetIsCalculationCompleted()
{
	return this->isCalculationCompleted;
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

PathElement * PathPlanner::GetSelectedPathElement()
{
	return this->selectedPathElement;
}

void PathPlanner::SetSelectedPathElement(PathElement * selectedPathElement)
{
	this->selectedPathElement = selectedPathElement;

	SetPathElementPropertiesChanged(true);
}

void PathPlanner::SetMap(Map * map)
{
	this->map = map;
}

void PathPlanner::SetVehicle(Vehicle * vehicle)
{
	this->vehicle = vehicle;
}

void PathPlanner::SetVehicleStart(Vehicle * vehicleStart)
{
	this->vehicleStart = vehicleStart;
}

void PathPlanner::SetVehicleEnd(Vehicle * vehicleEnd)
{
	this->vehicleEnd = vehicleEnd;
}

void PathPlanner::SetExpandedMap(Map * expandedMap)
{
	this->expandedMap = expandedMap;
}

void PathPlanner::SetPolylinePath(Path * polylinePath)
{
	this->polylinePath = polylinePath;
}

void PathPlanner::SetParkingPathStart(Path * parkingPathStart)
{
	this->parkingPathStart = parkingPathStart;
}

void PathPlanner::SetParkingPathEnd(Path * parkingPathEnd)
{
	this->parkingPathEnd = parkingPathEnd;
}

void PathPlanner::SetFinalPath(Path * finalPath)
{
	this->finalPath = finalPath;
}

void PathPlanner::SetVoronoiGraph(Graph * voronoiGraph)
{
	this->voronoiGraph = voronoiGraph;
}

void PathPlanner::SetFullVoronoiVisibilityGraph(Graph * fullVoronoiVisibilityGraph)
{
	this->fullVoronoiVisibilityGraph = fullVoronoiVisibilityGraph;
}

Path * PathPlanner::createDirectPolylinePath(Line *startLine, Line *endLine)
{
	Path *path = new Path();

	glm::vec2 intersectionPoint = GeometryHelper::GetLineIntersectionPoint(startLine->GetFrom(), startLine->GetTo(), endLine->GetFrom(), endLine->GetTo());

	if (isnan(intersectionPoint.x) || isnan(intersectionPoint.y) || isinf(intersectionPoint.x), isinf(intersectionPoint.y))
		return nullptr;

	glm::vec2 startDirection = GeometryHelper::GetLineDirection(startLine->GetFrom(), startLine->GetTo());
	glm::vec2 startIntersectionDirection = GeometryHelper::GetLineDirection(startLine->GetTo(), intersectionPoint);
	if (!epsilonEquals(startDirection.x, startIntersectionDirection.x) || !epsilonEquals(startDirection.y, startIntersectionDirection.y))
		return nullptr;

	glm::vec2 endDirection = GeometryHelper::GetLineDirection(endLine->GetFrom(), endLine->GetTo());
	glm::vec2 endIntersectionDirection = GeometryHelper::GetLineDirection(intersectionPoint, endLine->GetFrom());
	if (!epsilonEquals(endDirection.x, endIntersectionDirection.x) || !epsilonEquals(endDirection.y, endIntersectionDirection.y))
		return nullptr;

	path->AddElement(new Line(startLine->GetFrom(), intersectionPoint));
	path->AddElement(new Line(intersectionPoint, endLine->GetTo()));

	return path;
}

Path * PathPlanner::createParkingPath(Vehicle vehicle, ParkingSpace parkingSpace, ParkManeuverType parkManeuverType)
{
	PathElement *lastElement;
	Path *path = new Path();

	if (parkingSpace.GetSize().x < vehicle.GetWheelbase() || parkingSpace.GetSize().y < vehicle.GetTrack())
		return nullptr;

	glm::vec2 parkingPlaceDirection;
	if (parkingSpace.GetType() == ParkingSpaceType::Paralell)
	{
		auto rot = parkingSpace.GetRotation();
		int arcCount = 0;
		double insideAngle = vehicle.GetMaxInsideAngle();
		double angleFrom = 0;
		double angleTo = 0;

		vehicle.UpdateState(0.0);
		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(vehicle.GetSize().x / 2.0) * vehicle.GetDirWheelbase(), 0); // ustawienie auta na œrodku miejsca

		auto p1 = vehicle.GetPosition();

		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(parkingSpace.GetSize().x / 2.0) * vehicle.GetDirWheelbase()); // przesuniêcie auta do koñca miejsca

		auto p2 = vehicle.GetPosition();

		// zak³adamy, ¿e pojazd jest w tej samej orientacji co miejsce parkingowe
		Circle* circle = vehicle.GetTurnCircle(insideAngle, Left);

		parkingSpace.SetRotation(0);
		auto pp = parkingSpace.GetPosition().x - vehicle.GetSize().x / 2.0;
		while (circle->GetPointForAngle(angleTo).x < pp)
			angleTo += 0.001;
		parkingSpace.SetRotation(rot);

		vehicle.UpdateState(-parkingSpace.GetRotation());
		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(vehicle.GetSize().x / 2.0) * vehicle.GetDirWheelbase(), -parkingSpace.GetRotation()); // ustawienie auta na œrodku miejsca

		p1 = vehicle.GetPosition();

		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(parkingSpace.GetSize().x / 2.0) * vehicle.GetDirWheelbase()); // przesuniêcie auta do koñca miejsca

		p2 = vehicle.GetPosition();

		path->AddElement(new Line(p2, p1, Back));

		parkingPlaceDirection = vehicle.GetDirWheelbase();

		while (parkingSpace.ContainVehicle(vehicle) || arcCount % 2 == 1) // ³uki
		{
			switch (arcCount % 4)
			{
			case 0:
				circle = vehicle.GetTurnCircle(insideAngle, Left, angleFrom - rot, angleTo - rot);
				vehicle.UpdateState(circle->GetPointForAngle(circle->angleTo), circle->angleTo);
				break;
			case 1:
				circle = vehicle.GetTurnCircle(insideAngle, Right, 2 * M_PI - angleTo + rot, 2 * M_PI + angleFrom + rot);
				vehicle.UpdateState(circle->GetPointForAngle(circle->angleTo), -circle->angleTo);
				break;
			case 2:
				circle = vehicle.GetTurnCircle(insideAngle, Left, 2 * M_PI + angleFrom - rot, 2 * M_PI - angleTo - rot, ManeuverType::Back);
				vehicle.UpdateState(circle->GetPointForAngle(circle->angleTo), circle->angleTo);
				break;
			case 3:
				circle = vehicle.GetTurnCircle(insideAngle, Right, angleTo + rot, angleFrom + rot, ManeuverType::Back);
				vehicle.UpdateState(circle->GetPointForAngle(circle->angleTo), -circle->angleTo);
				break;
			}

			arcCount++;
			path->AddElement(circle);
			//vehicle.UpdateState(circle->GetPointForAngle(circle->angleTo), arcCount % 2 == 0 ? circle->angleTo : -circle->angleTo);
		}
	}
	else if (parkingSpace.GetType() == ParkingSpaceType::Perpendicular)
	{
		// TODO:
		vehicle.UpdateState(parkingSpace.GetRotation());
		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(vehicle.GetSize().x / 2.0) * vehicle.GetDirWheelbase(), 0); // ustawienie auta na œrodku miejsca

		auto p1 = vehicle.GetPosition();

		vehicle.UpdateState(parkingSpace.GetPosition() - (float)(parkingSpace.GetSize().x / 2.0) * vehicle.GetDirWheelbase()); // przesuniêcie auta do koñca miejsca

		auto p2 = vehicle.GetPosition();

		path->AddElement(new Line(p1, p2));
	}

	path->AddElement(new Line(vehicle.GetPosition(), vehicle.GetPosition() + (float)vehicle.GetSize().x / 2.0f * vehicle.GetDirWheelbase()));

	glm::vec2 pathTranslation;
	pathTranslation = (float)vehicle.GetSize().x / 2.0f * parkingPlaceDirection;
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

bool PathPlanner::GetShowParkingPath()
{
	return this->showParkingPath;
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

void PathPlanner::SetShowParkingPath(bool showParkingPath)
{
	this->showParkingPath = showParkingPath;
}

void PathPlanner::SetShowFinalPath(bool showFinalPath)
{
	this->showFinalPath = showFinalPath;
}

void PathPlanner::SetShowExpandedObstacles(bool showExpandedObstacles)
{
	this->showExpandedObstacles = showExpandedObstacles;
}

bool PathPlanner::GetPathElementPropertiesChanged()
{
	return this->pathElementPropertiesChanged;
}

void PathPlanner::SetPathElementPropertiesChanged(bool pathElementPropertiesChanged)
{
	this->pathElementPropertiesChanged = pathElementPropertiesChanged;
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
	this->isCalculationCompleted = false;

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

	this->isCalculationCompleted = true;
}
