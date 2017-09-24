#ifndef PATH_PLANNER_H
#define PATH_PLANNER_H

#include <vector>
#include <glm.hpp>
#include "Map.h"
#include "Vehicle.h"
#include "Path.h"
#include "Point.h"
#include "Line.h"
#include "Arc.h"
#include "BSpline.h"
#include "GeometryHelper.h"
#include "Simulation.h"

#define EPS 10e-3;

using namespace std;

enum ParkManeuverType
{
	Entry,
	Exit
};

class PathPlanner
{
public:
	PathPlanner();
	PathPlanner(Map map, Vehicle vehicle);

	void AddUserPoint(glm::vec2 userPoint);
	void SetUserPoints(vector<glm::vec2> userPoints);

	vector<glm::vec2> UserPoints();

	Path *CreateAdmissiblePath(ParkingSpace *start, ParkingSpace *end);
	Path *CreateAdmissiblePath(ParkingSpace *start, glm::vec2 end);
	Path *CreateAdmissiblePath(glm::vec2 start, ParkingSpace *end);
	Path *CreateAdmissiblePath(glm::vec2 start, glm::vec2 end);

	Path *CreateAdmissiblePath(Path *path); // zak³adamy, ¿e ta œcie¿ka sk³ada siê z samych linii
	Path *CreateAdmissiblePath(vector<glm::vec2> points);

	void NewSimulation();
	void OpenSimulation(string filePath);
	void SaveSimulation(string filePath);

	Map * GetMap();
	Vehicle * GetVehicle();

	void SetMap(Map *map);
	void SetVehicle(Vehicle *vehicle);

	Path *createParkingPath(Vehicle vehicle, ParkingSpace parkingSpace, ParkManeuverType parkManeuverType = ParkManeuverType::Entry);

	bool GetSetStartPosition();
	bool GetSetEndPosition();
	bool GetShowVoronoiGraph();
	bool GetShowFullVoronoiVisibilityGraph();
	bool ShowPolylinePath();
	bool ShowFinalPath();
	bool ShowExpandedObstacles();

	void SetSetStartPosition(bool setStartPosition);
	void SetSetEndPosition(bool setEndPosition);
	void SetShowVoronoiGraph(bool showVoronoiGraph);
	void SetShowFullVoronoiVisibilityGraph(bool showFullVoronoiVisibilityGraph);
	void SetShowPolylinePath(bool showPolylinePath);
	void SetShowFinalPath(bool showFinalPath);
	void SetShowExpandedObstacles(bool showExpandedObstacles);
	
private:
	Simulation *simulation = NULL;

	Map *map;
	Vehicle *vehicle;
	Path path;
	vector<glm::vec2> userPoints;

	bool setStartPosition = false;
	bool setEndPosition = false;
	bool showVoronoiGraph = false;
	bool showFullVoronoiVisibilityGraph = false;
	bool showPolylinePath = false;
	bool showFinalPath = false;
	bool showExpandedObstacles = false;

	bool epsilonEquals(float f1, float f2);
	Path *createArcsBetweenSegments(vector<glm::vec2> points);
	bool checkArcsCorrectness(Path *pathArcs, int *arc1, int *arc2);
	
};

#endif