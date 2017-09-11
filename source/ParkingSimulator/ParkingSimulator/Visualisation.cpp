#include "Visualisation.h"

#include "PathPlanner.h"

Visualisation::Visualisation()
{
	this->visualisation2D = true;
	this->visualisation3D = false;

	//tymczasowo
	Map *mapa = new Map(1000, 750);
	Vehicle *vehicle = new Vehicle(100, 50);
	Path *path = new Path();

	PathPlanner pathPlanner(*mapa, *vehicle);
	MapElement *parkingSpaceStart = new ParkingSpace(glm::vec2(150, 200), glm::vec2(200, 120), ParkingSpaceType::Paralell);
	MapElement *parkingSpaceEnd = new ParkingSpace(glm::vec2(600, 500), glm::vec2(200, 120), ParkingSpaceType::Paralell);

	MapElement *obstacle = new Obstacle(glm::vec2(500, 250), glm::vec2(50, 50), ObstacleType::Decoration, "name");
	MapElement *obstacle2 = new Obstacle(glm::vec2(450, 380), glm::vec2(50, 50), ObstacleType::Decoration, "name");
	MapElement *obstacle3 = new Obstacle(glm::vec2(340, 250), glm::vec2(50, 50), ObstacleType::Decoration, "name");

	mapa->AddMapElement(parkingSpaceStart);
	mapa->AddMapElement(parkingSpaceEnd);
	mapa->AddMapElement(obstacle);
	mapa->AddMapElement(obstacle2);

	auto pS = dynamic_cast<ParkingSpace*>(parkingSpaceStart);
	auto pE = dynamic_cast<ParkingSpace*>(parkingSpaceEnd);

	Path *pStart = pathPlanner.createParkingPath(*vehicle, *pS, Entry);
	Path *pEnd = pathPlanner.createParkingPath(*vehicle, *pE, Exit);

	pathPlanner.AddUserPoint(glm::vec2(200, 300));
	pathPlanner.AddUserPoint(glm::vec2(200, 500));
	pathPlanner.AddUserPoint(glm::vec2(500, 500));
	pathPlanner.AddUserPoint(glm::vec2(500, 300));

	pathPlanner.AddUserPoint(glm::vec2(700, 300));
	pathPlanner.AddUserPoint(glm::vec2(300, 100));

	pathPlanner.AddUserPoint(glm::vec2(100, 100));

	pathPlanner.AddUserPoint(glm::vec2(100, 300));
	pathPlanner.AddUserPoint(glm::vec2(310, 300));

	pathPlanner.AddUserPoint(glm::vec2(305, 171));

	pathPlanner.AddUserPoint(glm::vec2(678, 186));
	pathPlanner.AddUserPoint(glm::vec2(697, 488));
	pathPlanner.AddUserPoint(glm::vec2(988, 517));
	pathPlanner.AddUserPoint(glm::vec2(1000, 100));

	///Path *p2 = pathPlanner.CreateAdmissiblePath(pathPlanner.UserPoints());

	Line* start = dynamic_cast<Line*>(pStart->GetLastElement());
	Line* end = dynamic_cast<Line*>(pEnd->GetFirstElement());
	pStart->RemoveElement(start);
	//pEnd->RemoveElement(end);

	//if (end->GetManeuverType() == Back)
	//	end = new Line(end->GetTo(), end->GetFrom(), Back);

	int indexStart, indexEnd;

	auto expandedMap = mapa->GetExpandedMap(3.0f * vehicle->GetTrack() / 4.0f);

	//voronoi.CreateVoronoiGraph(expandedMap);
	voronoi.CreateVoronoiVisibilityFullGraph(expandedMap, start, end, &indexStart, &indexEnd);

	double **estimated = new double*[voronoi.VerticesCount()];
	for (int i = 0; i < voronoi.VerticesCount(); i++)
		estimated[i] = new double[voronoi.VerticesCount()];

	for (int i = 0; i < voronoi.VerticesCount(); i++)
		for (int j = 0; j < voronoi.VerticesCount(); j++)
			estimated[i][j] = 999;
	
	
	auto path2 = voronoi.FindPath(indexStart, indexEnd, estimated);
		
	auto path3 = pathPlanner.CreateAdmissiblePath(path2);

	std::vector<Path*> pathParts;
	pathParts.push_back(pStart);
	pathParts.push_back(path3);
	pathParts.push_back(pEnd);

	Path *finalPath = new Path(pathParts);
	
	mapa2 = mapa;
	currentSimulation = new Simulation(mapa, vehicle, finalPath);

	currentSimulation->SetSimulationTime(10);
}

bool Visualisation::GetVisualisation2D()
{
	return this->visualisation2D;
}

bool Visualisation::GetVisualisation3D()
{
	return this->visualisation3D;
}

void Visualisation::SetVisualisation2D(bool visualisation2D)
{
	this->visualisation2D = visualisation2D;
}

void Visualisation::SetVisualisation3D(bool visualisation3D)
{
	this->visualisation3D = visualisation3D;
}

bool Visualisation::GetShowPath()
{
	return this->showPath;
}

void Visualisation::SetShowPath(bool showPath)
{
	this->showPath = showPath;
}

void Visualisation::SetCurrentSimulation(Simulation * simulation)
{
	this->currentSimulation = simulation;
}

Simulation * Visualisation::GetCurrentSimulation()
{
	return this->currentSimulation;
}

std::vector<Simulation*> Visualisation::GetSimulations()
{
	return this->simulations;
}

void Visualisation::AddSimulation(Simulation * simulation)
{
	this->simulations.push_back(simulation);
}

void Visualisation::RemoveSimulation(Simulation * simulation)
{
}
