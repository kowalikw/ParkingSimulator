#ifndef PATH_PLANNER_H
#define PATH_PLANNER_H

#include <vector>
#include <glm.hpp>
#include "Map.h"
#include "Vehicle.h"
#include "Path.h"
#include "Line.h"
#include "Arc.h"
#include "BSpline.h"
#include "GeometryHelper.h"
#include "Simulation.h"
#include "ParkingSpace.h"
#include "Graph.h"

#define EPS 10e-3

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
	PathPlanner(PathPlanner& pathPlanner);
	PathPlanner(Map map, Vehicle vehicle);

	void AddUserPoint(glm::vec2 userPoint);
	void SetUserPoints(vector<glm::vec2> userPoints);

	vector<glm::vec2> UserPoints();

	Path *CreateAdmissiblePath(ParkingSpace *start, ParkingSpace *end);
	Path *CreateAdmissiblePath(ParkingSpace *start, glm::vec2 end);
	Path *CreateAdmissiblePath(glm::vec2 start, ParkingSpace *end);
	Path *CreateAdmissiblePath(glm::vec2 start, glm::vec2 end);
	Path *CreateAdmissiblePath(Line *startLine, Line *endLine);

	Path *CreateAdmissiblePath(Path *path); // zak³adamy, ¿e ta œcie¿ka sk³ada siê z samych linii
	Path *CreateAdmissiblePath(vector<glm::vec2> points);

	void NewSimulation();
	void OpenSimulation(string filePath);
	void SaveSimulation(string filePath);

	Map * GetMap();
	Map * GetExpandedMap();
	Vehicle * GetVehicle();
	Vehicle * GetVehicleStart();
	Vehicle * GetVehicleEnd();
	Path * GetPolylinePath();
	Path * GetFinalPath();
	Path * GetParkingPathStart();
	Path * GetParkingPathEnd();
	Graph * GetVoronoiGraph();
	Graph * GetFullVoronoiVisibilityGraph();
	Simulation * GetSimulation();

	bool GetIsCalculationCompleted();

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

	PathElement * GetSelectedPathElement();
	void SetSelectedPathElement(PathElement * selectedPathElement);

	double GetExpandSizePercent();
	void SetExpandSizePercent(double expandSizePercent);

	double GetCollisionDetectionDensity();
	void SetCollisionDetectionDensity(double collisionDetectionDensity);

	PathPlanningAlgorithm GetAlgorithm();
	void SetAlgorithm(PathPlanningAlgorithm algorithm);

	bool GetUseAdmissibleArcsOnly();
	void SetUseAdmissibleArcsOnly(bool useAdmissibleArcsOnly);

	int GetGraphExtraVerticesAlong();
	void SetGraphExtraVerticesAlong(int graphExtraVerticesAlong);

	int GetGraphExtraVerticesAcross();
	void SetGraphExtraVerticesAcross(int graphExtraVerticesAcross);

	GraphEdge * ChackPathCollision(Path *path, Map *Map, bool useGraph = true, int start = -1, int end = -1, MapElement * exceptionElement = nullptr, bool invertPath = false);

	void SetMap(Map *map);
	void SetVehicle(Vehicle *vehicle);
	void SetVehicleStart(Vehicle *vehicleStart);
	void SetVehicleEnd(Vehicle *vehicleEnd);

	void SetExpandedMap(Map *expandedMap);
	void SetPolylinePath(Path *polylinePath);
	void SetParkingPathStart(Path *parkingPathStart);
	void SetParkingPathEnd(Path *parkingPathEnd);
	void SetFinalPath(Path *finalPath);
	void SetVoronoiGraph(Graph *voronoiGraph);
	void SetFullVoronoiVisibilityGraph(Graph *fullVoronoiVisibilityGraph);

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
	bool GetShowParkingPath();
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
	void SetShowParkingPath(bool showParkingPath);
	void SetShowFinalPath(bool showFinalPath);
	void SetShowExpandedObstacles(bool showExpandedObstacles);

	bool GetPathElementPropertiesChanged();
	void SetPathElementPropertiesChanged(bool pathElementPropertiesChanged);

	MapElement * GetHoverElement(glm::vec2 mousePosition);

	void FindPath(int *error);
	
private:
	Simulation *simulation = NULL;

	Map *map = NULL;
	Map *expandedMap = NULL;
	Vehicle *vehicle = NULL;
	Vehicle *vehicleStart = NULL;
	Vehicle *vehicleEnd = NULL;
	Path *polylinePath = NULL;
	Path *parkingPathStart = NULL;
	Path *parkingPathEnd = NULL;
	Path *finalPath = NULL;
	Graph *voronoiGraph = NULL;
	Graph *fullVoronoiVisibilityGraph = NULL;

	glm::vec2 *startPoint = NULL;
	glm::vec2 *endPoint = NULL;

	glm::vec2 *startDirection = NULL;
	glm::vec2 *endDirection = NULL;

	ParkingSpace *startParkingSpace = NULL;
	ParkingSpace *endParkingSpace = NULL;

	PathElement *selectedPathElement = NULL;

	double expandSizePercent = 0;
	double collisionDetectionDensity = 0;
	PathPlanningAlgorithm pathPlanningAlgorithm = PathPlanningAlgorithm::Arcs;
	bool useAdmissibleArcsOnly = false;
	int graphExtraVerticesAlong = 0;
	int graphExtraVerticesAcross = 0;

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
	bool showParkingPath = false;
	bool showFinalPath = false;
	bool showExpandedObstacles = false;

	bool pathElementPropertiesChanged = false;

	bool epsilonEquals(float f1, float f2);
	Path * createDirectPolylinePath(Line *startLine, Line *endLine);
	Path *createArcsBetweenSegments(vector<glm::vec2> points);
	bool checkArcsCorrectness(Path *pathArcs, int *arc1, int *arc2);
	bool checkArcsCorrectness(Path *pathArcs, int arc1, int arc2);
	bool checkPolylinePathCorectness(Path *path, int *element1, int *element2);

	const double RADIUS_EPS = 3;

	bool isCalculationCompleted = true;
};

#endif