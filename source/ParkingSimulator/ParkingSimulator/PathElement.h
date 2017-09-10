#ifndef PATH_ELEMENT_H
#define PATH_ELEMENT_H

#include <vector>
#include <glm.hpp>

#define M_PI 3.14159265358979323846

struct SimulationState
{
	glm::vec2 position;
	double angle;
};

enum ManeuverType
{
	Front,
	Back
};

class PathElement
{
public:
	/*PathElement();
	PathElement(glm::vec2 from, glm::vec2 to);
	PathElement(glm::vec2 centre, double radius, double angleFrom, double angleTo, std::vector<glm::vec2> arcBasePoints, CircleType circleType = CircleType::Right);
	glm::vec2 GetFrom();
	glm::vec2 GetTo();
	double GetRadius();
	double GetAngleFrom();
	double GetAngleTo();
	PathElementType GetType();
	glm::vec2 GetLinePoint(double t);
	glm::vec2 GetCirclePoint(double angle);	*/

	ManeuverType GetManeuverType();
	void SetManeuverType(ManeuverType maneuvertype);

	virtual double GetLength() = 0;
	virtual double GetAngle(double t) = 0;
	virtual glm::vec2 GetPoint(double t) = 0;
	virtual SimulationState GetSimulationState(double t) = 0;
protected:
	ManeuverType maneuverType;
};

#endif