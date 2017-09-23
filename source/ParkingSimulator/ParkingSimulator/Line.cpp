#include "Line.h"

Line::Line()
{
}

Line::Line(glm::vec2 from, glm::vec2 to, ManeuverType maneuverType)
{
	this->from = from;
	this->to = to;
	this->maneuverType = maneuverType;
}

glm::vec2 Line::GetFrom() const
{
	return this->from;
}

glm::vec2 Line::GetTo() const
{
	return this->to;
}

void Line::SetFrom(glm::vec2 from)
{
	this->from = from;
}

void Line::SetTo(glm::vec2 to)
{
	this->to = to;
}

double Line::GetLength()
{
	return sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
}

double Line::GetAngle(double t)
{
	return atan2(to.y - from.y, to.x - from.x);
}

glm::vec2 Line::GetPoint(double t)
{
	if (maneuverType == ManeuverType::Back)
		t = 1 - t;

	double x = (1 - t) * from.x + t * to.x;
	double y = (1 - t) * from.y + t * to.y;

	return glm::vec2(x, y);
}

SimulationState Line::GetSimulationState(double t)
{
	SimulationState simulationState;
	simulationState.position = GetPoint(t);
	simulationState.angle = M_PI - GetAngle(t);

	return simulationState;
}