#ifndef PARKING_SPACE_H
#define PARKING_SPACE_H

#include "GeometryHelper.h"
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
	ParkingSpace(glm::vec2 position, ParkingSpaceType type = ParkingSpaceType::Both);
	ParkingSpace(glm::vec2 position, glm::vec2 size, ParkingSpaceType type = ParkingSpaceType::Both);
	ParkingSpace(glm::vec2 position, glm::vec2 size, double rotation, std::vector<glm::vec2> points, ParkingSpaceType type = ParkingSpaceType::Both);
	~ParkingSpace();

	ParkingSpaceType GetType();
	bool ContainVehicle(Vehicle vehicle);

#pragma region Boost serialization.

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<MapElement>(*this);
		ar & type;
	}

#pragma endregion

private:
	ParkingSpaceType type;
};

#endif