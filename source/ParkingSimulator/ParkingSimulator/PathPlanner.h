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

#define EPS 10e-3;

using namespace std;

class PathPlanner
{
public:
	PathPlanner();
	PathPlanner(Map map, Vehicle vehicle);

	void AddUserPoint(glm::vec2 userPoint);
	void SetUserPoints(vector<glm::vec2> userPoints);

	vector<glm::vec2> UserPoints();
	vector<glm::vec2> VoronoiPoints();

	Path CreateAdmissiblePath(vector<glm::vec2> points);
//private:
	Map map;
	Vehicle vehicle;
	vector<glm::vec2> userPoints;
	vector<glm::vec2> voronoiPoints;

	bool epsilonEquals(float f1, float f2);
	Path createArcsBetweenSegments(vector<glm::vec2> points);
	bool checkArcsCorrectness(Path pathArcs, int *arc1, int *arc2);
	Path createParkingPath(Vehicle vehicle, ParkingSpace parkingSpace);
};

#endif