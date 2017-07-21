#include "ParkingSpace.h"

ParkingSpace::ParkingSpace()
{
}

ParkingSpace::ParkingSpace(glm::vec2 position, glm::vec2 size, ParkingSpaceType type)
{
	this->position = position;
	this->size = size;
	this->type = type;
}

ParkingSpaceType ParkingSpace::GetType()
{
	return this->type;
}

bool ParkingSpace::ContainVehicle(Vehicle vehicle)
{
	return true;
	//return vehicle.GetPosY() > (GetPosY() - GetSize().y); // TODO
}
