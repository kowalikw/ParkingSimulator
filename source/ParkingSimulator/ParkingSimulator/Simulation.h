#ifndef SIMULATION_H
#define SIMULATION_H

#include <QTimer>
#include "Map.h"
#include "Vehicle.h"
#include "Path.h"

class Simulation
{
public:
	Simulation();
	Simulation(Map *map, Vehicle *vehicle, Path *path);
	Simulation(Map *map, Vehicle *vehicle, Path *path, float simulationTime);
	Map *GetMap();
	Vehicle *GetVehicle();
	Path *GetPath();
	float GetSimulationTime();
	float GetCurrentSimulationTime();
	void SetMap(Map *map);
	void SetVehicle(Vehicle *vehicle);
	void SetPath(Path *path);
	void SetSimulationTime(float simulationTime);

	void Start();
	void Pause();
	void Stop();
	void Step();
private:
	Map *map;
	Vehicle *vehicle;
	Path *path;
	float simulationTime;
	float currentSimulationTime;
	QTimer *simulationTimer;
};

#endif