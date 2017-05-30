#ifndef VEHICLE_H
#define VEHICLE_H

#include <cmath>
#include <vector>
#include "PathElement.h"
#include "MapElement.h"

#define M_PI 3.14159265358979323846

using namespace std;

class Vehicle : public MapElement
{
public:
	Vehicle();
	Vehicle(double wheelbase, double track);
	void UpdateState(double angle);
	void UpdateState(glm::vec2 position);
	void UpdateState(glm::vec2 position, double angle);
	void UpdateState(double x, double y, double z, double angle);
	void UpdateState(PathElement pathElement);
	double GetR(double angle);
	double GetRMin(double angle);
	double GetRMax(double angle);
	double GetAngle();
	double GetTrack();
	double GetWheelbase();
	double GetMaxInsideAngle();
	double GetMinOneManeouverLength(double insideAngle);
	glm::vec2 GetDirTrack();
	glm::vec2 GetDirWheelbase();
	glm::vec2 GetTurnCentre(double angle, CircleType circleType);

	PathElement GetTurnCircle(double insideAngle, CircleType circleType, double angleFrom = 0, double angleTo = 2 * M_PI);
private:
	double wheelbase; // rozstaw osi
	double track; // rozstaw kó³
	double maxInsideAngle = M_PI / 4;

	glm::vec2 dirWheelbase;
	glm::vec2 dirTrack;
};

#endif