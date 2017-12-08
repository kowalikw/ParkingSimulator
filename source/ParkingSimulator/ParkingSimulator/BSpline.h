#ifndef B_SPLINE_H
#define B_SPLINE_H

#include <vector>
#include "PathElement.h"
#include "Arc.h"
#include "GeometryHelper.h"

using namespace std;

class BSpline : public PathElement
{
public:
	BSpline();
	BSpline(vector<glm::vec2> controlPoints, int p = 3);
	~BSpline();

	glm::vec2 CalculatePoint(double u);
	std::vector<glm::vec2> GetControlPoints();

	double GetPointX(double t);
	double GetPointY(double t);

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
		ar & n;
		ar & m;
		ar & p;

		ar & controlPoints.size();
		for (int i = 0; i < controlPoints.size(); i++)
			ar & controlPoints[i];

		ar & knots.size();
		for (int i = 0; i < knots.size(); i++)
			ar & knots[i];
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		double knot;
		glm::vec2 point;
		int controlPointsCount, knotsCount;

		ar & boost::serialization::base_object<PathElement>(*this);
		ar & n;
		ar & m;
		ar & p;

		ar & controlPointsCount;
		for (int i = 0; i < controlPointsCount; i++)
		{
			ar & point;
			controlPoints.push_back(point);
		}

		ar & knotsCount;
		for (int i = 0; i < knotsCount; i++)
		{
			ar & knot;
			knots.push_back(knot);
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
//private:
	int n; // n+1 control points
	int m; // m+1 knots
	int p; // degree of b-spline

	vector<glm::vec2> controlPoints;
	vector<double> knots;
};

#endif