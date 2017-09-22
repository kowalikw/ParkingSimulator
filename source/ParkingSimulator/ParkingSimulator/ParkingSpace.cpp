#include "ParkingSpace.h"

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
	auto wheelbase = vehicle.GetWheelbase();
	auto track = vehicle.GetTrack();
	auto dirWheelbase = vehicle.GetDirWheelbase();
	auto dirTrack = vehicle.GetDirTrack();

	auto p0 = position;
	auto p1 = p0 + (float)(wheelbase / 2.0) * dirWheelbase;
	auto p2 = p1 + (float)(track / 2.0) * dirTrack;
	auto p3 = p1 - (float)(track / 2.0) * dirTrack;
	auto p4 = p0 - (float)(wheelbase / 2.0) * dirWheelbase;
	auto p5 = p4 + (float)(track / 2.0) * dirTrack;
	auto p6 = p4 - (float)(track / 2.0) * dirTrack;

	auto parkingSpacePoints = GetPoints();
	
	return GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p0) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p1) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p2) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p3) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p4) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p5) ||
		GeometryHelper::CheckPolygonContainsPoint(parkingSpacePoints, p6); // TODO: Geometry helper::ContainsAnyPoint?
}
