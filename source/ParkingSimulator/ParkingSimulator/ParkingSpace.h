#ifndef PARKING_SPACE_H
#define PARKING_SPACE_H

#include "MapElement.h"
#include "Vehicle.h"

enum ParkingSpaceType
{
	Paralell,
	Perpendicular,
	Both
};

class ParkingSpace : public MapElement
{
public:
	ParkingSpace();
	ParkingSpace(glm::vec2 position, glm::vec2 size, ParkingSpaceType type = ParkingSpaceType::Both);
	ParkingSpaceType GetType();
	bool ContainVehicle(Vehicle vehicle); // I assume parking space is always a rectangle
private:
	ParkingSpaceType type;
};

#endif