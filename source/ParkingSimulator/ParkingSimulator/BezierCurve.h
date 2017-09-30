#ifndef  BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include "PathElement.h"

class BezierCurve : public PathElement
{
public:
	BezierCurve();
	BezierCurve(std::vector<glm::vec2> coefficients);
	~BezierCurve();

	virtual double GetLength();
	virtual double GetAngle(double t);
	virtual glm::vec2 GetFirstPoint();
	virtual glm::vec2 GetLastPoint();
	virtual glm::vec2 GetPoint(double t);
	virtual SimulationState GetSimulationState(double t);
private:
	glm::vec2 DeCasteljau(double t);
	std::vector<glm::vec2> coefficients;
};

#endif // ! BEZIER_CURVE_H