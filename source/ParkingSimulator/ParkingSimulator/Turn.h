#pragma once
#ifndef TURN_H
#define TURN_H

#include "PathElement.h"

class Turn : public PathElement
{
public:
	Turn();
	Turn(double angleFrom, double angleTo);
	Turn(double angleFrom, double angleTo, glm::vec2 position, double rotation);
	~Turn();

	double GetAngleFrom();
	double GetAngleTo();
	void SetAngleFrom(double angleFrom);
	void SetAngleTo(double angleTo);

	virtual double GetLength();
	virtual double GetAngle(double t);
	virtual double GetInsideAngle(double t, double wheelbase, double track);
	virtual double GetCurvature(double t);
	virtual glm::vec2 GetFirstPoint();
	virtual glm::vec2 GetLastPoint();
	virtual glm::vec2 GetPoint(double t);
	virtual SimulationState GetSimulationState(double t);
	virtual CircleType GetDirection(double t);

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & angleFrom;
		ar & angleTo;
		ar & position;
		ar & rotation;
		ar & length;
		ar & lengthToElement;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & angleFrom;
		ar & angleTo;
		ar & position;
		ar & rotation;
		ar & length;
		ar & lengthToElement;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	double angleFrom;
	double angleTo;
	glm::vec2 position;
	double rotation;
};

#endif