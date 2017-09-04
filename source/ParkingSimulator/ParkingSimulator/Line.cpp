#include "Line.h"

Line::Line(glm::vec2 from, glm::vec2 to)
{
	this->from = from;
	this->to = to;
}

glm::vec2 Line::GetFrom() const
{
	return this->from;
}

glm::vec2 Line::GetTo() const
{
	return this->to;
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
	double x = (1 - t) * from.x + t * to.x;
	double y = (1 - t) * from.y + t * to.y;

	return glm::vec2(x, y);
}

SimulationState Line::GetSimulationState(double t)
{
	SimulationState simulationState;
	simulationState.position = GetPoint(t);
	simulationState.angle = GetAngle(t);

	return simulationState;
}