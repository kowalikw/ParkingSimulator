#include "Vehicle.h"

#pragma Constructors and destructor.

Vehicle::Vehicle()
{
	this->position = glm::vec2(0, 0);
	this->size = glm::vec2(0, 0);
	this->rotation = 0;
	this->wheelbase = 0;
	this->track = 0;
	this->dirWheelbase = glm::vec2(1.0f, 0.0f);
	this->dirTrack = glm::vec2(0.0f, 1.0f);

	this->points.clear();
	this->points.push_back(glm::vec2(-size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, size.y / 2.0f));
	this->points.push_back(glm::vec2(-size.x / 2.0f, size.y / 2.0f));

	transform();
}

Vehicle::Vehicle(glm::vec2 position, glm::vec2 size, double rotation, std::vector<glm::vec2> points)
{
	this->position = position;
	this->size = size;
	this->rotation = rotation;
	this->points = points;
	this->isRectangular = false;
	this->dirWheelbase = glm::vec2(1.0f, 0.0f);
	this->dirTrack = glm::vec2(0.0f, 1.0f);
}

Vehicle::Vehicle(double wheelbase, double track)
{
	this->position = glm::vec2(0.0f, 0.0f);
	this->rotation = 0.0;
	this->size = glm::vec2(wheelbase, track);
	this->wheelbase = wheelbase;
	this->track = track;
	this->dirWheelbase = glm::vec2(1.0f, 0.0f);
	this->dirTrack = glm::vec2(0.0f, 1.0f);

	this->points.clear();
	this->points.push_back(glm::vec2(-size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, size.y / 2.0f));
	this->points.push_back(glm::vec2(-size.x / 2.0f, size.y / 2.0f));

	transform();
}

Vehicle::~Vehicle()
{
}

#pragma endregion

void Vehicle::UpdateState(double angle)
{
	this->rotation = angle;

	transform();
}

void Vehicle::UpdateState(glm::vec2 position)
{
	this->position = position;

	transform();
}

void Vehicle::UpdateState(glm::vec2 position, double angle)
{
	this->position = position;
	this->rotation = angle;

	transform();
}

void Vehicle::UpdateState(double x, double y, double z, double angle)
{
	this->position = glm::vec2(x, y);
	this->rotation = angle;

	transform();
}

void Vehicle::UpdateState(PathElement *pathElement)
{
	if (dynamic_cast<Line*>(pathElement) != NULL)
		UpdateState(dynamic_cast<Line*>(pathElement)->GetTo());
	else if (dynamic_cast<Circle*>(pathElement) != NULL)
	{
		Circle *circle = dynamic_cast<Circle*>(pathElement);
		UpdateState(circle->GetPointForAngle(circle->GetAngleTo()), circle->GetCircleType() == Right ? -circle->GetAngleTo() : circle->GetAngleTo());
	}

	transform();
}

void Vehicle::UpdateState(SimulationState simulationState)
{
	this->position = simulationState.position;
	this->rotation = simulationState.angle;

	transform();
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

	return this->position + sgn * (float)(GetRMin(insideAngle) + this->track / 2.0) * dirTrack;
}

std::vector<glm::vec2> Vehicle::GetPoints2()
{
	std::vector<glm::vec2> points2;

	auto p0 = position;
	auto p1 = p0 + (float)(size.x / 2.0) * GetDirWheelbase();
	auto p2 = p1 + (float)(size.y / 2.0) * GetDirTrack();
	auto p3 = p1 - (float)(size.y / 2.0) * GetDirTrack();
	auto p4 = p0 - (float)(size.x / 2.0) * GetDirWheelbase();
	auto p5 = p4 + (float)(size.y / 2.0) * GetDirTrack();
	auto p6 = p4 - (float)(size.y / 2.0) * GetDirTrack();

	points2.push_back(p3);
	points2.push_back(p2);
	points2.push_back(p5);
	points2.push_back(p6);

	return points2;
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

void Vehicle::SetMaxInsideAngle(double maxInsideAngle)
{
	this->maxInsideAngle = maxInsideAngle;
}

double Vehicle::GetMinOneManeouverLength(double insideAngle)
{
	return sqrt(GetRMax(insideAngle) * GetRMax(insideAngle) + GetRMin(insideAngle) * GetRMin(insideAngle));
}

double Vehicle::GetInsideAngleForRadius(double radius, CircleType circleType)
{
	double angle = atan(wheelbase / (radius - track / 2.0f));
	if (circleType == Right)
		angle *= -1;
	return angle;
}

double Vehicle::GetOutsideAngleForRadius(double radius, CircleType circleType)
{
	return 0.0;
}

glm::vec2 Vehicle::GetDirTrack()
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), -(float)GetAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 dirTrack = rotationMatrix * glm::vec4(this->dirTrack, 0.0f, 1.0f);

	return glm::vec2(dirTrack);
}

glm::vec2 Vehicle::GetDirWheelbase()
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), -(float)GetAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 dirWheelbase = rotationMatrix * glm::vec4(this->dirWheelbase, 0.0f, 1.0f);

	return glm::vec2(dirWheelbase);
}

void Vehicle::SetWheelbase(double wheelbase)
{
	this->wheelbase = wheelbase;
}

void Vehicle::SetTrack(double track)
{
	this->track = track;
}

Circle *Vehicle::GetTurnCircle(double insideAngle, CircleType circleType, double angleFrom, double angleTo, ManeuverType maneuverType)
{
	insideAngle = abs(insideAngle);
	auto centre = GetTurnCentre(insideAngle, circleType);
	auto radius = GetR(insideAngle);

	std::vector<glm::vec2> basePoints;

	return new Circle(centre, radius, angleFrom, angleTo, basePoints, circleType, maneuverType);
}

Model* Vehicle::GetVehicleModel()
{
	return this->vehicleModel;
}

Model* Vehicle::GetFrontLeftWheelModel()
{
	return this->frontLeftWheelModel;
}

Model* Vehicle::GetFrontRightWheelModel()
{
	return this->frontRightWheelModel;
}

Model* Vehicle::GetRearLeftWheelModel()
{
	return this->rearLeftWheelModel;
}

Model* Vehicle::GetRearRightWheelModel()
{
	return this->rearRightWheelModel;
}

void Vehicle::SetVehicleModel(Model* vehicleModel)
{
	this->vehicleModel = vehicleModel;
}

void Vehicle::SetFrontLeftWheelModel(Model* frontLeftWheelModel)
{
	this->frontLeftWheelModel = frontLeftWheelModel;
}

void Vehicle::SetFrontRightWheelModel(Model* frontRightWheelModel)
{
	this->frontRightWheelModel = frontRightWheelModel;
}

void Vehicle::SetRearLeftWheelModel(Model* rearLeftWheelModel)
{
	this->rearLeftWheelModel = rearLeftWheelModel;
}

void Vehicle::SetRearRightWheelModel(Model* rearRightWheelModel)
{
	this->rearRightWheelModel = rearRightWheelModel;
}
