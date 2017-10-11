#include "Simulation.h"

Simulation::Simulation()
{
	this->simulationTime = 0;
	this->currentSimulationTime = 0;
}

Simulation::Simulation(Map *map, Vehicle *vehicle, Path *path)
{
	Simulation();

	this->map = map;
	this->vehicle = vehicle;
	this->finalPath = path;
	this->simulationTime = 0;
	this->currentSimulationTime = 0;
}

Simulation::Simulation(Map *map, Vehicle *vehicle, Path *path, float simulationTime)
{
	Simulation(map, vehicle, path);

	this->simulationTime = simulationTime;
}

std::string Simulation::GetName()
{
	return this->name;
}

Map * Simulation::GetMap()
{
	return this->map;
}

Map * Simulation::GetExpandedMap()
{
	return this->expandedMap;
}

Vehicle * Simulation::GetVehicle()
{
	return this->vehicle;
}

Path * Simulation::GetPolylinePath()
{
	return this->polylinePath;
}

Path * Simulation::GetParkingPathStart()
{
	return this->parkingPathStart;
}

Path * Simulation::GetParkingPathEnd()
{
	return this->parkingPathEnd;
}

Path * Simulation::GetFinalPath()
{
	return this->finalPath;
}

Graph * Simulation::GetVoronoiGraph()
{
	return this->voronoiGraph;
}

Graph * Simulation::GetFullVoronoiVisibilityGraph()
{
	return this->fullVoronoiVisibilityGraph;
}

glm::vec2 * Simulation::GetStartPoint()
{
	return this->startPoint;
}

glm::vec2 * Simulation::GetEndPoint()
{
	return this->endPoint;
}

glm::vec2 * Simulation::GetStartDirection()
{
	return this->startDirection;
}

glm::vec2 * Simulation::GetEndDirection()
{
	return this->endDirection;
}

ParkingSpace * Simulation::GetStartParkingSpace()
{
	return this->startParkingSpace;
}

ParkingSpace * Simulation::GetEndParkingSpace()
{
	return this->endParkingSpace;
}

double Simulation::GetExpandSize()
{
	return this->expandSize;
}

double Simulation::GetCollisionDetectionDensity()
{
	return this->collisionDetectionDensity;
}

PathPlanningAlgorithm Simulation::GetPathPlanningAlgorithm()
{
	return this->pathPlanningAlgorithm;
}

bool Simulation::GetUseAdmissibleArcsOnly()
{
	return this->useAdmissibleArcsOnly;
}

float Simulation::GetSimulationTime()
{
	return this->simulationTime;
}

float Simulation::GetCurrentSimulationTime()
{
	return this->currentSimulationTime;
}

void Simulation::SetName(std::string name)
{
	this->name = name;
}

void Simulation::SetMap(Map *map)
{
	this->map = map;
}

void Simulation::SetExpandedMap(Map * map)
{
	this->expandedMap = map;
}

void Simulation::SetVehicle(Vehicle *vehicle)
{
	this->vehicle = vehicle;
}

void Simulation::SetPolylinePath(Path * polylinePath)
{
	this->polylinePath = polylinePath;
}

void Simulation::SetParkingPathStart(Path * parkingPathStart)
{
	this->parkingPathStart = parkingPathStart;
}

void Simulation::SetParkingPathEnd(Path * parkingPathEnd)
{
	this->parkingPathEnd = parkingPathEnd;
}

void Simulation::SetFinalPath(Path * finalPath)
{
	this->finalPath = finalPath;
}

void Simulation::SetVoronoiGraph(Graph * voronoiGraph)
{
	this->voronoiGraph = voronoiGraph;
}

void Simulation::SetFullVoronoiVisibilityGraph(Graph * fullVoronoiVisibilityGraph)
{
	this->fullVoronoiVisibilityGraph = fullVoronoiVisibilityGraph;
}

void Simulation::SetStartPoint(glm::vec2 * startPoint)
{
	this->startPoint = startPoint;
}

void Simulation::SetEndPoint(glm::vec2 * endPoint)
{
	this->endPoint = endPoint;
}

void Simulation::SetStartDirection(glm::vec2 * startDirection)
{
	this->startDirection = startDirection;
}

void Simulation::SetEndDirection(glm::vec2 * endDirection)
{
	this->endDirection = endDirection;
}

void Simulation::SetStartParkingSpace(ParkingSpace * startParkingSpace)
{
	this->startParkingSpace = startParkingSpace;
}

void Simulation::SetEndParkingSpace(ParkingSpace * endParkingSpace)
{
	this->endParkingSpace = endParkingSpace;
}

void Simulation::SetExpandSize(double expandSize)
{
	this->expandSize = expandSize;
}

void Simulation::SetCollisionDetectionDensity(double collisionDetectionDensity)
{
	this->collisionDetectionDensity = collisionDetectionDensity;
}

void Simulation::SetPathPlanningAlgorithm(PathPlanningAlgorithm pathPlanningAlgorithm)
{
	this->pathPlanningAlgorithm = pathPlanningAlgorithm;
}

void Simulation::SetUseAdmissibleArcsOnly(bool useAdmissibleArcsOnly)
{
	this->useAdmissibleArcsOnly = useAdmissibleArcsOnly;
}

void Simulation::SetSimulationTime(float simulationTime)
{
	this->simulationTime = simulationTime;

	//Step();
}

void Simulation::SetCurrentSimulationTime(float currentSimulationTime)
{
	this->currentSimulationTime = currentSimulationTime;
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

void Simulation::UpdateSimulationState()
{
	double timeRatio = currentSimulationTime / simulationTime;

	if (timeRatio > 1.0)
	{
		Stop();
		return;
	}

	SimulationState simulationState = finalPath->GetSimulationState(timeRatio);

	this->vehicle->UpdateState(simulationState);
}

void Simulation::Step()
{
	UpdateSimulationState();

	currentSimulationTime += simulationTimer.elapsed() / 1000.0;
	
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
