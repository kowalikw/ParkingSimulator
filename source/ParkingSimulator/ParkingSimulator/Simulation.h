#ifndef SIMULATION_H
#define SIMULATION_H

#include <QElapsedTimer>
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
	void SetCurrentSimulationTime(float currentSimulationPath);
	void UpdateSimulationState();

	void Start();
	void Pause();
	void Stop();
	void Step();

	bool IsStarted();
	bool IsPaused();
	bool IsStopped();

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & *map;
		ar & *vehicle;
		ar & *path;

		ar & (startPoint != NULL);
		ar & (endPoint != NULL);
		ar & (startParkingSpace != NULL);
		ar & (endParkingSpace != NULL);

		if (startPoint != NULL) ar & *startPoint;
		if (endPoint != NULL) ar & *endPoint;
		if (startParkingSpace != NULL) ar & *startParkingSpace;
		if (endParkingSpace != NULL) ar & *endParkingSpace;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		map = new Map();
		vehicle = new Vehicle();
		path = new Path();
		bool isStartPoint, isEndPoint;
		bool isStartParkingPlace, isEndParkingPlace;

		ar & *map;
		ar & *vehicle;
		ar & *path;

		ar & isStartPoint;
		ar & isEndPoint;
		ar & isStartParkingPlace;
		ar & isEndParkingPlace;

		if (isStartPoint) ar & *startPoint;
		if (isEndPoint) ar & *endPoint;
		if (isStartParkingPlace) ar & *startParkingSpace;
		if (isEndParkingPlace) ar & *endParkingSpace;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	Map *map;
	Vehicle *vehicle;
	Path *path;

	glm::vec2 *startPoint = NULL;
	glm::vec2 *endPoint = NULL;

	ParkingSpace *startParkingSpace = NULL;
	ParkingSpace *endParkingSpace = NULL;

	float simulationTime;
	float currentSimulationTime;
	bool isStarted = false;
	bool isPaused = false;
	QElapsedTimer simulationTimer;
};

#endif