#include "Arc.h"

Circle::Circle()
{
}

Circle::Circle(glm::vec2 centre, double radius, double angleFrom, double angleTo, std::vector<glm::vec2> circleBasePoints, CircleType circleType, ManeuverType maneuverType)
{
	this->center = centre;
	this->radius = radius;
	this->angleFrom = angleFrom;
	this->angleTo = angleTo;
	this->circleBasePoints = circleBasePoints;
	this->circleType = circleType;
	this->maneuverType = maneuverType;
	this->length = abs(angleFrom - angleTo) * radius;
}

double Circle::GetAngleFrom() const
{
	return this->angleFrom;
}

double Circle::GetAngleTo() const
{
	return this->angleTo;
}

double Circle::GetRadius() const
{
	return this->radius;
}

void Circle::SetAngleFrom(double angleFrom)
{
	this->angleFrom = angleFrom;
}

void Circle::SetAngleTo(double angleTo)
{
	this->angleTo = angleTo;
}

void Circle::SetRadius(double radius)
{
	this->radius = radius;
}

glm::vec2 Circle::GetCenter() const
{
	return this->center;
}

CircleType Circle::GetCircleType() const
{
	return this->circleType;
}

void Circle::SetCircleType(CircleType type)
{
	this->circleType = type;
}

std::vector<glm::vec2> Circle::GetCircleBasePoints() const
{
	return this->circleBasePoints;
}

glm::vec2 Circle::GetPointForAngle(double angle)
{
	double x, y;

	if (circleType == CircleType::Right)
	{
		x = center.x + radius * sin(angle);
		y = center.y - radius * cos(angle);
	}
	else
	{
		x = center.x + radius * sin(angle);
		y = center.y + radius * cos(angle);
	}

	return glm::vec2(x, y);
}

double Circle::GetLength()
{
	if(this->length == 0)
		this->length = abs(angleFrom - angleTo) * radius;
	return this->length;
}
double Circle::GetAngle(double t)
{
	if(maneuverType == ManeuverType::Back)
		t = 1 - t;
	return angleFrom < angleTo ? angleFrom + t * (angleTo - angleFrom) : angleTo + t * (angleFrom - angleTo);
}

double Circle::GetInsideAngle(double t, double wheelbase, double track)
{
	double angle = atan(wheelbase / (radius - track / 2.0f));
	if (circleType == Right)
		angle *= -1;
	return angle;
}

double Circle::GetCurvature(double t)
{
	if (t == 0.0) t += 0.01;
	if (t == 1.0) t -= 0.01;

	auto d1 = GetPoint(t - 0.01f) - GetPoint(t);
	auto d2 = GetPoint(t) - GetPoint(t + 0.01f);

	double angle = GeometryHelper::GetAngleBetweenVectors(d1, d2);
	double length = 0;
	for (double u = t - 0.01 + 0.002; u < t + 0.01 + 0.002; u += 0.002)
	{
		length += GeometryHelper::GetDistanceBetweenPoints(GetPoint(u - 0.002), GetPoint(u));
	}

	return abs(angle / length);
}

glm::vec2 Circle::GetFirstPoint()
{
	return GetPointForAngle(angleFrom);
}

glm::vec2 Circle::GetLastPoint()
{
	return GetPointForAngle(angleTo);
}

glm::vec2 Circle::GetPoint(double t)
{
	double angle = GetAngle(t);

	return GetPointForAngle(angle);
}

SimulationState Circle::GetSimulationState(double t)
{
	SimulationState simulationState;
	simulationState.position = GetPoint(t);
	simulationState.angle = circleType == CircleType::Right ?  -GetAngle(t) : GetAngle(t);
	simulationState.curvature = GetCurvature(t);
	simulationState.direction = GetDirection(t);

	return simulationState;
}

CircleType Circle::GetDirection(double t)
{
	return CircleType();
}
