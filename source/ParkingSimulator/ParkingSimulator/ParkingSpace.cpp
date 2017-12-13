#include "ParkingSpace.h"

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 
#include <Windows.h>

ParkingSpace::ParkingSpace()
{
}

ParkingSpace::ParkingSpace(glm::vec2 position, ParkingSpaceType type)
{
	this->position = position;
	this->size = glm::vec2(0, 0);
	this->type = type;
	this->rotation = 0.0;
	this->isRectangular = true;

	this->points.push_back(glm::vec2(-size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, size.y / 2.0f));
	this->points.push_back(glm::vec2(-size.x / 2.0f, size.y / 2.0f));

	transform();
}

ParkingSpace::ParkingSpace(glm::vec2 position, glm::vec2 size, ParkingSpaceType type)
{
	this->position = position;
	this->size = size;
	this->type = type;
	this->rotation = 0.0;
	this->isRectangular = true;

	this->points.push_back(glm::vec2(-size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, size.y / 2.0f));
	this->points.push_back(glm::vec2(-size.x / 2.0f, size.y / 2.0f));

	transform();
}

ParkingSpace::ParkingSpace(glm::vec2 position, glm::vec2 size, double rotation, std::vector<glm::vec2> points, ParkingSpaceType type)
{
	this->position = position;
	this->size = size;
	this->rotation = rotation;
	this->points = points;
	this->type = type;
	this->isRectangular = false;
}

ParkingSpace::~ParkingSpace()
{
}

ParkingSpaceType ParkingSpace::GetType()
{
	return this->type;
}

bool ParkingSpace::ContainVehicle(Vehicle vehicle)
{
	auto position = vehicle.GetPosition();
	auto length = vehicle.GetSize().x;
	auto width = vehicle.GetSize().y + 10;
	auto dirWheelbase = vehicle.GetDirWheelbase();
	auto dirTrack = vehicle.GetDirTrack();

	std::vector<glm::vec2> vehiclePoints;
	auto p0 = position;
	auto p1 = p0 + (float)(length / 2.0) * dirWheelbase;
	auto p2 = p1 + (float)(width / 2.0) * dirTrack;
	auto p3 = p1 - (float)(width / 2.0) * dirTrack;
	auto p4 = p0 - (float)(length / 2.0) * dirWheelbase;
	auto p5 = p4 + (float)(width / 2.0) * dirTrack;
	auto p6 = p4 - (float)(width / 2.0) * dirTrack;

	vehiclePoints.push_back(p0);
	vehiclePoints.push_back(p1);
	vehiclePoints.push_back(p2);
	vehiclePoints.push_back(p3);
	vehiclePoints.push_back(p4);
	vehiclePoints.push_back(p5);
	vehiclePoints.push_back(p6);

	auto parkingSpacePoints = GetPoints();

	return GeometryHelper::CheckPolygonContainsAnyPoint(parkingSpacePoints, vehiclePoints);
}
