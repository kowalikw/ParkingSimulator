#ifndef CIRCLE_H
#define CIRCLE_H

#include "PathElement.h"
#include "GeometryHelper.h"
#include "CommonHelper.h"

class Circle : public PathElement
{
public:
	Circle();
	Circle(glm::vec2 centre, double radius, double angleFrom, double angleTo, std::vector<glm::vec2> circleBasePoints, CircleType circleType = CircleType::Right, ManeuverType maneuverType = ManeuverType::Front);
	double GetAngleFrom() const;
	double GetAngleTo() const;
	double GetRadius() const;
	void SetAngleFrom(double angleFrom);
	void SetAngleTo(double angleTo);
	void SetRadius(double radius);
	glm::vec2 GetCenter() const;
	CircleType GetCircleType() const;
	void SetCircleType(CircleType type);
	std::vector<glm::vec2> GetCircleBasePoints() const;
	glm::vec2 GetPointForAngle(double angle);

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
		ar & boost::serialization::base_object<PathElement>(*this);
		ar & angleFrom;
		ar & angleTo;
		ar & radius;
		ar & center;
		ar & circleType;

		ar & circleBasePoints.size();
		for (int i = 0; i < circleBasePoints.size(); i++)
			ar & circleBasePoints[i];
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		glm::vec2 point;
		int circleBasePointsCount;

		ar & boost::serialization::base_object<PathElement>(*this);
		ar & angleFrom;
		ar & angleTo;
		ar & radius;
		ar & center;
		ar & circleType;

		ar & circleBasePointsCount;
		for (int i = 0; i < circleBasePointsCount; i++)
		{
			ar & point;
			circleBasePoints.push_back(point);
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

//private:
	double angleFrom;
	double angleTo;
	double radius;
	glm::vec2 center;
	CircleType circleType;
	std::vector<glm::vec2> circleBasePoints;
};

#endif