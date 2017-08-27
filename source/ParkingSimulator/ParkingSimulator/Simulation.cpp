#include "Simulation.h"

Simulation::Simulation()
{
}

Simulation::Simulation(Map *map, Vehicle *vehicle, Path *path)
{
	this->map = map;
	this->vehicle = vehicle;
	this->path = path;
	this->simulationTime = 0;
	this->currentSimulationTime = 0;
}

Simulation::Simulation(Map *map, Vehicle *vehicle, Path *path, float simulationTime)
{
	this->map = map;
	this->vehicle = vehicle;
	this->path = path;
	this->simulationTime = simulationTime;
	this->currentSimulationTime = 0;
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
