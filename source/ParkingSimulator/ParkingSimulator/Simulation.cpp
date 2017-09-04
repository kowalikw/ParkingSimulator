#include "Simulation.h"

Simulation::Simulation()
{
	simulationTimer = new QTimer();

	simulationTimer->start(10);
}

Simulation::Simulation(Map *map, Vehicle *vehicle, Path *path)
{
	Simulation();

	this->map = map;
	this->vehicle = vehicle;
	this->path = path;
	this->simulationTime = 0;
	this->currentSimulationTime = 0;
}

Simulation::Simulation(Map *map, Vehicle *vehicle, Path *path, float simulationTime)
{
	Simulation(map, vehicle, path);

	this->simulationTime = simulationTime;
}

Map * Simulation::GetMap()
{
	return this->map;
}

Vehicle * Simulation::GetVehicle()
{
	return this->vehicle;
}

Path * Simulation::GetPath()
{
	return this->path;
}

float Simulation::GetSimulationTime()
{
	return this->simulationTime;
}

float Simulation::GetCurrentSimulationTime()
{
	return this->currentSimulationTime;
}

void Simulation::SetMap(Map *map)
{
	this->map = map;
}

void Simulation::SetVehicle(Vehicle *vehicle)
{
	this->vehicle = vehicle;
}

void Simulation::SetPath(Path *path)
{
	this->path = path;
}

void Simulation::SetSimulationTime(float simulationTime)
{
	this->simulationTime = simulationTime;
}

void Simulation::Start()
{
	simulationTimer->start();
}

void Simulation::Pause()
{
	simulationTimer->start();
}

void Simulation::Stop()
{
	simulationTimer->stop();
	currentSimulationTime = 0;
}

void Simulation::Step()
{
	double timeRatio = currentSimulationTime / simulationTime;

	PathElement *pathElement = path->GetElement(timeRatio);

	double pathLength = path->GetLength();
	double lengthToPathElementExclude = path->GetLengthToElement(pathElement);
	double lengthToPathElementInclude = pathElement->GetLength() + lengthToPathElementExclude;

	double t = (lengthToPathElementInclude - (timeRatio * pathLength)) / pathElement->GetLength();
	SimulationState simulationState = pathElement->GetSimulationState(t);

	this->vehicle->UpdateState(simulationState);

	float elapsedTime = 0.001f;
	currentSimulationTime += elapsedTime;
}