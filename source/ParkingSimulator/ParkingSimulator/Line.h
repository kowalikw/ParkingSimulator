#ifndef LINE_H
#define LINE_H

#include "PathElement.h"
#include "Arc.h"

class Line : public PathElement
{
public:
	Line();
	Line(glm::vec2 from, glm::vec2 to, ManeuverType maneuverType = ManeuverType::Front);
	Line(glm::vec2 from, glm::vec2 to, int v1, int v2, ManeuverType maneuverType = ManeuverType::Front);
	~Line();
	glm::vec2 GetFrom() const;
	glm::vec2 GetTo() const;
	void SetFrom(glm::vec2 from);
	void SetTo(glm::vec2 to);

	int GetV1() const;
	int GetV2() const;

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
		ar & from;
		ar & to;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<PathElement>(*this);
		ar & from;
		ar & to;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	glm::vec2 from;
	glm::vec2 to;

	int v1;
	int v2;
};

#endif