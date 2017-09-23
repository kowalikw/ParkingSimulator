#ifndef VEHICLE_H
#define VEHICLE_H

#include <cmath>
#include <vector>
#include "PathElement.h"
#include "MapElement.h"
#include "Line.h"
#include "Arc.h"

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
	void UpdateState(PathElement *pathElement);
	void UpdateState(SimulationState simulationState);
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

	Circle *GetTurnCircle(double insideAngle, CircleType circleType, double angleFrom = 0, double angleTo = 2 * M_PI, ManeuverType maneuverType = ManeuverType::Front);

	double wheelbase; // rozstaw osi
	double track; // rozstaw k�
	double maxInsideAngle = M_PI / 4;

	glm::vec2 dirWheelbase;
	glm::vec2 dirTrack;

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & wheelbase;
		ar & track;
		ar & maxInsideAngle;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & wheelbase;
		ar & track;
		ar & maxInsideAngle;

		dirWheelbase = glm::vec2(1.0f, 0.0f);
		dirTrack = glm::vec2(0.0f, 1.0f);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#endif