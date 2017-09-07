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

	MapElement *obstacle = new Obstacle(glm::vec2(400, 250), glm::vec2(50, 50), ObstacleType::Decoration, "name");

	mapa->AddMapElement(parkingSpaceStart);
	mapa->AddMapElement(parkingSpaceEnd);
	mapa->AddMapElement(obstacle);

	auto pS = dynamic_cast<ParkingSpace*>(parkingSpaceStart);
	auto pE = dynamic_cast<ParkingSpace*>(parkingSpaceEnd);

	Path *pStart = pathPlanner.createParkingPath(*vehicle, *pS);
	Path *pEnd = pathPlanner.createParkingPath(*vehicle, *pE);

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

	Path *p2 = pathPlanner.CreateAdmissiblePath(pathPlanner.UserPoints());

	Line* start = dynamic_cast<Line*>(pStart->GetLastElement());
	Line* end = dynamic_cast<Line*>(pEnd->GetLastElement());

	int indexStart, indexEnd;
	voronoi.CreateVoronoiVisibilityFullGraph(mapa, start, end, &indexStart, &indexEnd);

	double **estimated = new double*[voronoi.VerticesCount()];
	for (int i = 0; i < voronoi.VerticesCount(); i++)
		estimated[i] = new double[voronoi.VerticesCount()];

	for (int i = 0; i < voronoi.VerticesCount(); i++)
		for (int j = 0; j < voronoi.VerticesCount(); j++)
			estimated[i][j] = 999;
	
	
	auto path2 = voronoi.FindPath(indexStart, indexEnd, estimated);
		
	currentSimulation = new Simulation(mapa, vehicle, pStart);

	currentSimulation->SetSimulationTime(15);
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
