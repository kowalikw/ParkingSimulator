#ifndef PATH_ELEMENT_H
#define PATH_ELEMENT_H

#include <vector>
#include <glm.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_member.hpp>

#define M_PI 3.14159265358979323846

enum CircleType
{
	Left, // ccw
	Right, // cw
	Undefined // colinear
};

struct SimulationState
{
	glm::vec2 position;
	double angle;
	double curvature;
	CircleType direction;
};

enum ManeuverType
{
	Front,
	Back
};

class PathElement
{
public:
	~PathElement();

	ManeuverType GetManeuverType();
	void SetManeuverType(ManeuverType maneuvertype);

	double GetLengthToElement();
	void SetLengthToElement(double length);

	virtual double GetLength() = 0;
	virtual double GetAngle(double t) = 0;
	virtual double GetInsideAngle(double t, double wheelbase, double track) = 0;
	virtual double GetCurvature(double t) = 0;
	virtual glm::vec2 GetFirstPoint() = 0;
	virtual glm::vec2 GetLastPoint() = 0;
	virtual glm::vec2 GetPoint(double t) = 0;
	virtual SimulationState GetSimulationState(double t) = 0;
	virtual CircleType GetDirection(double t) = 0;

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & maneuverType;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & maneuverType;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
protected:
	ManeuverType maneuverType;
	double length = 0;
	double lengthToElement = 0;
};

#endif