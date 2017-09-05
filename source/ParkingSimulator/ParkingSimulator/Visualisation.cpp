#include "Visualisation.h"

#include "PathPlanner.h"

Visualisation::Visualisation()
{
	this->visualisation2D = true;
	this->visualisation3D = false;

	//tymczasowo
	Map *mapa = new Map(1000, 550);
	Vehicle *vehicle = new Vehicle(100, 50);
	Path *path = new Path();

	PathPlanner pathPlanner(*mapa, *vehicle);
	MapElement *parkingSpace = new ParkingSpace(glm::vec2(200, 200), glm::vec2(300, 120), ParkingSpaceType::Paralell);

	auto pS = dynamic_cast<ParkingSpace*>(parkingSpace);

	Path *p = pathPlanner.createParkingPath(*vehicle, *pS);

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
	
	currentSimulation = new Simulation(mapa, vehicle, p2);

	currentSimulation->SetSimulationTime(10);

	currentSimulation->Start();
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
