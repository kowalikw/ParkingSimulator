#include "Simulation.h"

Simulation::Simulation()
{
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
	isStarted = true;
	isPaused = false;
	simulationTimer.restart();
}

void Simulation::Pause()
{
	isStarted = true;
	isPaused = true;
	simulationTimer.restart();
}

void Simulation::Stop()
{
	isStarted = false;
	isPaused = false;
	currentSimulationTime = 0;
}

void Simulation::Step()
{
	double timeRatio = currentSimulationTime / simulationTime;

	if (timeRatio > 1.0)
	{
		Stop();
		return;
	}

	PathElement *pathElement = path->GetElement(timeRatio);

	double pathLength = path->GetLength();
	double lengthToPathElementExclude = path->GetLengthToElement(pathElement);
	double lengthToPathElementInclude = pathElement->GetLength() + lengthToPathElementExclude;

	double t = abs((lengthToPathElementExclude - (timeRatio * pathLength)) / pathElement->GetLength());
	SimulationState simulationState = pathElement->GetSimulationState(t);

	this->vehicle->UpdateState(simulationState);

	currentSimulationTime += simulationTimer.elapsed() / 1000.0;
	//currentSimulationTime += 0.01;
	simulationTimer.restart();
}

bool Simulation::IsStarted()
{
	return isStarted && !isPaused;
}

bool Simulation::IsPaused()
{
	return isStarted && isPaused;
}

bool Simulation::IsStopped()
{
	return !isStarted && !isPaused;
}
