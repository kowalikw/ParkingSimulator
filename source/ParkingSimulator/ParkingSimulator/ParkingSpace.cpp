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

	auto p0 = position;
	auto p1 = p0 + (float)(length / 2.0) * dirWheelbase;
	auto p2 = p1 + (float)(width / 2.0) * dirTrack;
	auto p3 = p1 - (float)(width / 2.0) * dirTrack;
	auto p4 = p0 - (float)(length / 2.0) * dirWheelbase;
	auto p5 = p4 + (float)(width / 2.0) * dirTrack;
	auto p6 = p4 - (float)(width / 2.0) * dirTrack;

	auto parkingSpacePoints = GetPoints();
	
	auto var = GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p0) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p1) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p2) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p3) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p4) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p5) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p6); // TODO: Geometry helper::ContainsAnyPoint?

	std::ostringstream ss;
	ss << "ParkingSpace P0: " << parkingSpacePoints[0].x << ", " << parkingSpacePoints[0].y << endl;
	ss << "ParkingSpace P1: " << parkingSpacePoints[1].x << ", " << parkingSpacePoints[1].y << endl;
	ss << "ParkingSpace P2: " << parkingSpacePoints[2].x << ", " << parkingSpacePoints[2].y << endl;
	ss << "ParkingSpace P3: " << parkingSpacePoints[3].x << ", " << parkingSpacePoints[3].y << endl;
	ss << "p0: " << p0.x << ", " << p0.y << endl;
	ss << "p1: " << p1.x << ", " << p1.y << endl;
	ss << "p2: " << p2.x << ", " << p2.y << endl;
	ss << "p3: " << p3.x << ", " << p3.y << endl;
	ss << "p4: " << p4.x << ", " << p4.y << endl;
	ss << "p5: " << p5.x << ", " << p5.y << endl;
	ss << "p6: " << p6.x << ", " << p6.y << endl;
	ss << "ContainVehicle: " << (var ? "True" : "False") << endl;
	ss << endl;
	std::string s(ss.str());
	OutputDebugStringA(s.c_str());

	return var;
}
