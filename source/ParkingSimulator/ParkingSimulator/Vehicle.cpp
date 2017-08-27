#include "Vehicle.h"

Vehicle::Vehicle()
{
}

Vehicle::Vehicle(double wheelbase, double track)
{
	this->position = glm::vec2(0.0f, 0.0f);
	this->rotation = 0.0;
	this->size = glm::vec2(1.0f, 1.0f);
	this->wheelbase = wheelbase;
	this->track = track;
	this->dirWheelbase = glm::vec2(1.0f, 0.0f);
	this->dirTrack = glm::vec2(0.0f, 1.0f);
}

void Vehicle::UpdateState(double angle)
{
	this->rotation = angle;
}

void Vehicle::UpdateState(glm::vec2 position)
{
	this->position = position;
}

void Vehicle::UpdateState(glm::vec2 position, double angle)
{
	this->position = position;
	this->rotation = angle;
}

void Vehicle::UpdateState(double x, double y, double z, double angle)
{
	this->position = glm::vec2(x, y);
	this->rotation = angle;
}

void Vehicle::UpdateState(PathElement pathElement)
{
	if (pathElement.type == PathElementType::Line)
		UpdateState(pathElement.to);
	else if (pathElement.type == PathElementType::Circle)
		UpdateState(pathElement.GetCirclePoint(pathElement.angleTo), pathElement.circleType == Right ? -pathElement.angleTo : pathElement.angleTo);
}

double Vehicle::GetR(double angle)
{
	return GetRMin(angle) + this->track / 2;
}

double Vehicle::GetRMin(double insideAngle)
{
	return abs(wheelbase / tan(insideAngle));
}

double Vehicle::GetRMax(double insideAngle)
{
	double outsideAngle = atan(wheelbase / (GetRMin(insideAngle) + track));

	return abs(wheelbase / sin(outsideAngle));
}

glm::vec2 Vehicle::GetTurnCentre(double insideAngle, CircleType circleType)
{
	glm::vec2 dirTrack = GetDirTrack();

	int sgn = 1;
	if (circleType == CircleType::Left)
		sgn *= -1;

	auto lala =  this->position + sgn * (float)(GetRMin(insideAngle) + this->track / 2.0) * dirTrack;

	return lala;
}

double Vehicle::GetAngle()
{
	return this->rotation;
}

double Vehicle::GetTrack()
{
	return this->track;
}

double Vehicle::GetWheelbase()
{
	return this->wheelbase;
}

double Vehicle::GetMaxInsideAngle()
{
	return this->maxInsideAngle;
}

double Vehicle::GetMinOneManeouverLength(double insideAngle)
{
	return sqrt(GetRMax(insideAngle) * GetRMax(insideAngle) + GetRMin(insideAngle) * GetRMin(insideAngle));
}

glm::vec2 Vehicle::GetDirTrack()
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::degrees((float)GetAngle()), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 dirTrack = rotationMatrix * glm::vec4(this->dirTrack, 0.0f, 1.0f);

	return glm::vec2(dirTrack);
}

glm::vec2 Vehicle::GetDirWheelbase()
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::degrees((float)GetAngle()), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 dirWheelbase = rotationMatrix * glm::vec4(this->dirWheelbase, 0.0f, 1.0f);

	return glm::vec2(dirWheelbase);
}

PathElement Vehicle::GetTurnCircle(double insideAngle, CircleType circleType, double angleFrom, double angleTo)
{
	insideAngle = abs(insideAngle);
	auto centre = GetTurnCentre(insideAngle, circleType);
	auto radius = GetR(insideAngle);

	std::vector<glm::vec2> basePoints;

	return PathElement(centre, radius, angleFrom, angleTo, basePoints, circleType);
}
