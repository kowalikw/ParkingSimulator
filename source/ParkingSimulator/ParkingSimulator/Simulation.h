#ifndef SIMULATION_H
#define SIMULATION_H

#include <QElapsedTimer>
#include "Map.h"
#include "Vehicle.h"
#include "Path.h"
#include "Graph.h"
#include <string.h>

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

	std::string GetName();
	Map *GetMap();
	Map *GetExpandedMap();
	Vehicle *GetVehicle();
	Path *GetPolylinePath();
	Path *GetParkingPathStart();
	Path *GetParkingPathEnd();
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
	int GetExtraVerticesAlong();
	int GetExtraVerticesAcross();
	float GetSimulationTime();
	float GetCurrentSimulationTime();

	void SetName(std::string name);
	void SetMap(Map *map);
	void SetExpandedMap(Map *map);
	void SetVehicle(Vehicle *vehicle);
	void SetPolylinePath(Path *path);
	void SetParkingPathStart(Path *parkingPathStart);
	void SetParkingPathEnd(Path *parkingPathEnd);
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
	void SetExtraVerticesAlong(int extraVerticesAlong);
	void SetExtraVerticesAcross(int extraVerticesAcross);
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
		ar & (map != NULL);
		ar & (expandedMap != NULL);
		ar & (vehicle != NULL);
		ar & (polylinePath != NULL);
		ar & (parkingPathStart != NULL);
		ar & (parkingPathEnd != NULL);
		ar & (finalPath != NULL);
		ar & (startPoint != NULL);
		ar & (endPoint != NULL);
		ar & (startDirection != NULL);
		ar & (endDirection != NULL);
		ar & (startParkingSpace != NULL);
		ar & (endParkingSpace != NULL);

		if (map != NULL) ar & *map;
		if (expandedMap != NULL) ar & *expandedMap;
		if (vehicle != NULL) ar & *vehicle;
		if (polylinePath != NULL) ar & *polylinePath;
		if (parkingPathStart != NULL) ar & *parkingPathStart;
		if (parkingPathEnd != NULL) ar & *parkingPathEnd;
		if (finalPath != NULL) ar & *finalPath;
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
		ar & extraVerticesAlong;
		ar & extraVerticesAcross;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		bool isMap, isExpandedMap, isVehicle;
		bool isPolylinePath, isParkingPathStart, isParkingPathEnd, isFinalPath;
		bool isStartPoint, isEndPoint;
		bool isStartDirection, isEndDirection;
		bool isStartParkingPlace, isEndParkingPlace;

		ar & isMap;
		ar & isExpandedMap;
		ar & isVehicle;
		ar & isPolylinePath;
		ar & isParkingPathStart;
		ar & isParkingPathEnd;
		ar & isFinalPath;
		ar & isStartPoint;
		ar & isEndPoint;
		ar & isStartDirection;
		ar & isEndDirection;
		ar & isStartParkingPlace;
		ar & isEndParkingPlace;

		if (isMap)
		{
			map = new Map();
			ar & *map;
		}

		if (isExpandedMap)
		{
			expandedMap = new Map();
			ar & *expandedMap;
		}

		if (isVehicle)
		{
			vehicle = new Vehicle();
			ar & *vehicle;
		}

		if (isPolylinePath)
		{
			polylinePath = new Path();
			ar & *polylinePath;
		}

		if (isParkingPathStart)
		{
			parkingPathStart = new Path();
			ar & *parkingPathStart;
		}

		if (isParkingPathEnd)
		{
			parkingPathEnd = new Path();
			ar & *parkingPathEnd;
		}

		if (isFinalPath)
		{
			finalPath = new Path();
			ar & *finalPath;
		}

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
		ar & extraVerticesAlong;
		ar & extraVerticesAcross;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	std::string name;

	Map *map;
	Map *expandedMap;
	Vehicle *vehicle;
	Path *polylinePath;
	Path *parkingPathStart;
	Path *parkingPathEnd;
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
	int extraVerticesAlong;
	int extraVerticesAcross;

	float simulationTime;
	float currentSimulationTime;
	bool isStarted = false;
	bool isPaused = false;
	QElapsedTimer simulationTimer;
};

#endif