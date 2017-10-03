#ifndef SIMULATION_H
#define SIMULATION_H

#include <QElapsedTimer>
#include "Map.h"
#include "Vehicle.h"
#include "Path.h"
//#include "PathPlanner.h"
#include "Graph.h"

enum PathPlanningAlgorithm
{
	Spline,
	Arcs
};

class Simulation
{
public:
	Simulation();
	Simulation(Map *map, Vehicle *vehicle, Path *path);
	Simulation(Map *map, Vehicle *vehicle, Path *path, float simulationTime);
	Map *GetMap();
	Map *GetExpandedMap();
	Vehicle *GetVehicle();
	Path *GetPolylinePath();
	Path *GetFinalPath();
	Graph *GetVoronoiGraph();
	Graph *GetFullVoronoiVisibilityGraph();
	glm::vec2 *GetStartPoint();
	glm::vec2 *GetEndPoint();
	glm::vec2 *GetStartDirection();
	glm::vec2 *GetEndDirection();
	ParkingSpace *GetStartParkingSpace();
	ParkingSpace *GetEndParkingSpace();
	double GetExpandSize();
	double GetCollisionDetectionDensity();
	PathPlanningAlgorithm GetPathPlanningAlgorithm();
	bool GetUseAdmissibleArcsOnly();
	float GetSimulationTime();
	float GetCurrentSimulationTime();


	void SetMap(Map *map);
	void SetExpandedMap(Map *map);
	void SetVehicle(Vehicle *vehicle);
	void SetPolylinePath(Path *path);
	void SetFinalPath(Path *path);
	void SetVoronoiGraph(Graph *graph);
	void SetFullVoronoiVisibilityGraph(Graph *graph);
	void SetStartPoint(glm::vec2 *startPoint);
	void SetEndPoint(glm::vec2 *endPoint);
	void SetStartDirection(glm::vec2 *startDirection);
	void SetEndDirection(glm::vec2 *endDirection);
	void SetStartParkingSpace(ParkingSpace *startParkingSpace);
	void SetEndParkingSpace(ParkingSpace *endParkingSpace);
	void SetExpandSize(double expandSize);
	void SetCollisionDetectionDensity(double collisionDetectionDensity);
	void SetPathPlanningAlgorithm(PathPlanningAlgorithm algorithm);
	void SetUseAdmissibleArcsOnly(bool useAdmissibleArcsOnly);
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
		ar & *expandedMap;
		ar & *vehicle;
		ar & *polylinePath;
		ar & *finalPath;

		ar & (startPoint != NULL);
		ar & (endPoint != NULL);
		ar & (startDirection != NULL);
		ar & (endDirection != NULL);
		ar & (startParkingSpace != NULL);
		ar & (endParkingSpace != NULL);

		if (startPoint != NULL) ar & *startPoint;
		if (endPoint != NULL) ar & *endPoint;
		if (startDirection != NULL) ar & *startDirection;
		if (endDirection != NULL) ar & *endDirection;
		if (startParkingSpace != NULL) ar & *startParkingSpace;
		if (endParkingSpace != NULL) ar & *endParkingSpace;

		ar & expandSize;
		ar & collisionDetectionDensity;
		ar & pathPlanningAlgorithm;
		ar & useAdmissibleArcsOnly;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		map = new Map();
		expandedMap = new Map();
		vehicle = new Vehicle();
		polylinePath = new Path();
		finalPath = new Path();
		bool isStartPoint, isEndPoint;
		bool isStartDirection, isEndDirection;
		bool isStartParkingPlace, isEndParkingPlace;

		ar & *map;
		ar & *expandedMap;
		ar & *vehicle;
		ar & *polylinePath;
		ar & *finalPath;

		ar & isStartPoint;
		ar & isEndPoint;
		ar & isStartDirection;
		ar & isEndDirection;
		ar & isStartParkingPlace;
		ar & isEndParkingPlace;

		if (isStartPoint)
		{
			startPoint = new glm::vec2();
			ar & *startPoint;
		}
		if (isEndPoint)
		{
			endPoint = new glm::vec2();
			ar & *endPoint;
		}
		if (isStartDirection)
		{
			startDirection = new glm::vec2();
			ar & *startDirection;
		}
		if (isEndDirection)
		{
			endDirection = new glm::vec2();
			ar & *endDirection;
		}
		if (isStartParkingPlace)
		{
			startParkingSpace = new ParkingSpace();
			ar & *startParkingSpace;
		}
		if (isEndParkingPlace)
		{
			endParkingSpace = new ParkingSpace();
			ar & *endParkingSpace;
		}

		ar & expandSize;
		ar & collisionDetectionDensity;
		ar & pathPlanningAlgorithm;
		ar & useAdmissibleArcsOnly;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	Map *map;
	Map *expandedMap;
	Vehicle *vehicle;
	Path *polylinePath;
	Path *finalPath;
	Graph *voronoiGraph;
	Graph *fullVoronoiVisibilityGraph;

	glm::vec2 *startPoint = NULL;
	glm::vec2 *endPoint = NULL;

	glm::vec2 *startDirection = NULL;
	glm::vec2 *endDirection = NULL;

	ParkingSpace *startParkingSpace = NULL;
	ParkingSpace *endParkingSpace = NULL;

	double expandSize;
	double collisionDetectionDensity;
	PathPlanningAlgorithm pathPlanningAlgorithm;
	bool useAdmissibleArcsOnly;

	float simulationTime;
	float currentSimulationTime;
	bool isStarted = false;
	bool isPaused = false;
	QElapsedTimer simulationTimer;
};

#endif