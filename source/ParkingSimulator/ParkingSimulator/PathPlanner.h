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
#include "ParkingSpace.h"
#include "Graph.h"

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
	Path * GetFinalPath();

	glm::vec2 * GetStartPoint();
	glm::vec2 * GetEndPoint();
	void SetStartPoint(glm::vec2 * startPoint);
	void SetEndPoint(glm::vec2 * endPoint);

	glm::vec2 * GetStartDirection();
	glm::vec2 * GetEndDirection();
	void SetStartDirection(glm::vec2 * startDirection);
	void SetEndDrection(glm::vec2 * endDirection);

	ParkingSpace * GetStartParkingSpace();
	ParkingSpace * GetEndParkingSpace();
	void SetStartParkingSpace(ParkingSpace *startParkingSpace);
	void SetEndParkingSpace(ParkingSpace *endParkingSpace);

	void SetMap(Map *map);
	void SetVehicle(Vehicle *vehicle);

	Path *createParkingPath(Vehicle vehicle, ParkingSpace parkingSpace, ParkManeuverType parkManeuverType = ParkManeuverType::Entry);

	bool GetSetStartPosition();
	bool GetSetEndPosition();
	bool GetSetStartDirection();
	bool GetSetEndDirection();
	bool GetStartPositionChanged();
	bool GetEndPositionChanged();
	bool GetShowVoronoiGraph();
	bool GetShowFullVoronoiVisibilityGraph();
	bool GetShowPolylinePath();
	bool GetShowFinalPath();
	bool GetShowExpandedObstacles();

	void SetSetStartPosition(bool setStartPosition);
	void SetSetEndPosition(bool setEndPosition);
	void SetSetStartDirection(bool setStartDirection);
	void SetSetEndDirection(bool setEndDirection);
	void SetStartPositionChanged(bool startPositionChanged);
	void SetEndPositionChanged(bool endPositionChanged);
	void SetShowVoronoiGraph(bool showVoronoiGraph);
	void SetShowFullVoronoiVisibilityGraph(bool showFullVoronoiVisibilityGraph);
	void SetShowPolylinePath(bool showPolylinePath);
	void SetShowFinalPath(bool showFinalPath);
	void SetShowExpandedObstacles(bool showExpandedObstacles);

	MapElement * GetHoverElement(glm::vec2 mousePosition);

	void FindPath(int *error);
	
private:
	Simulation *simulation = NULL;

	Map *map = NULL;
	Vehicle *vehicle = NULL;
	Path *path = NULL;

	glm::vec2 *startPoint = NULL;
	glm::vec2 *endPoint = NULL;

	glm::vec2 *startDirection = NULL;
	glm::vec2 *endDirection = NULL;

	ParkingSpace *startParkingSpace = NULL;
	ParkingSpace *endParkingSpace = NULL;

	vector<glm::vec2> userPoints;

	bool setStartPosition = false;
	bool setEndPosition = false;
	bool setStartDirection = false;
	bool setEndDirection = false;
	bool startPositionChanged = false;
	bool endPositionChanged = false;
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