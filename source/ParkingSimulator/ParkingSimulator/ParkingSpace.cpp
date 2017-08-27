#include "ParkingSpace.h"

ParkingSpace::ParkingSpace()
{
}

ParkingSpace::ParkingSpace(glm::vec2 position, glm::vec2 size, ParkingSpaceType type)
{
	this->position = position;
	this->size = size;
	this->type = type;
	this->rotation = 0.0;

	this->points.push_back(glm::vec2(-size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, -size.y / 2.0f));
	this->points.push_back(glm::vec2(size.x / 2.0f, size.y / 2.0f));
	this->points.push_back(glm::vec2(-size.x / 2.0f, size.y / 2.0f));

	transform();
}

ParkingSpaceType ParkingSpace::GetType()
{
	return this->type;
}

bool ParkingSpace::ContainVehicle(Vehicle vehicle)
{
	//return true;
	return vehicle.GetPosition().x > (vehicle.GetPosition().y - GetSize().y); // TODO
}
