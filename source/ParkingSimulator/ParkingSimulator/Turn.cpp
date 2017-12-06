#include "Turn.h"

Turn::Turn()
{
}

Turn::Turn(double angleFrom, double angleTo)
{
	this->angleFrom = angleFrom;
	this->angleTo = angleTo;
	this->length = 10 * abs(angleTo - angleFrom); // TODO:
}

Turn::Turn(double angleFrom, double angleTo, glm::vec2 position, double rotation)
{
	this->angleFrom = angleFrom;
	this->angleTo = angleTo;
	this->position = position;
	this->rotation = rotation;
	this->length = 10 * abs(angleTo - angleFrom); // TODO:
}

Turn::~Turn()
{
}

double Turn::GetAngleFrom()
{
	return this->angleFrom;
}

double Turn::GetAngleTo()
{
	return this->angleTo;
}

void Turn::SetAngleFrom(double angleFrom)
{
	this->angleFrom = angleFrom;
}

void Turn::SetAngleTo(double angleTo)
{
	this->angleTo = angleTo;
}

double Turn::GetLength()
{
	return this->length;
}
double Turn::GetAngle(double t)
{
	return rotation;
}

double Turn::GetInsideAngle(double t, double wheelbase, double track)
{
	return 0.0;
}

double Turn::GetCurvature(double t)
{
	return angleFrom + t * (angleTo - angleFrom); // TECHNICAL ISSUE TO CALCULATE IN THIS WAY
}

glm::vec2 Turn::GetFirstPoint()
{
	return position;
}

glm::vec2 Turn::GetLastPoint()
{
	return position;
}

glm::vec2 Turn::GetPoint(double t)
{
	return position;
}

SimulationState Turn::GetSimulationState(double t)
{
	SimulationState simulationState;
	simulationState.position = GetPoint(t);
	simulationState.angle = GetAngle(t);
	simulationState.curvature = GetCurvature(t);
	simulationState.direction = GetDirection(t);

	return simulationState;
}

CircleType Turn::GetDirection(double t)
{
	return CircleType();
}