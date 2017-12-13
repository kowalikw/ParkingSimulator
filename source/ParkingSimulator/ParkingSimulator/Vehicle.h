#ifndef VEHICLE_H
#define VEHICLE_H

#include <cmath>
#include <vector>
#include "PathElement.h"
#include "MapElement.h"
#include "Line.h"
#include "Arc.h"
#include "Model.h"

#define M_PI 3.14159265358979323846

using namespace std;

#pragma region Boost glm::vec2 and glm::vec3 serialization.

BOOST_SERIALIZATION_SPLIT_FREE(glm::vec3)
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec2)

namespace boost
{
	namespace serialization
	{
		template<class Archive> void save(Archive& ar, const glm::vec2& v, unsigned int version)
		{
			ar & v.x & v.y;
		}

		template<class Archive> void load(Archive& ar, glm::vec2& v, unsigned int version)
		{
			ar & v.x & v.y;
		}

		template<class Archive> void save(Archive& ar, const glm::vec3& v, unsigned int version)
		{
			ar & v.x & v.y & v.z;
		}

		template<class Archive> void load(Archive& ar, glm::vec3& v, unsigned int version)
		{
			ar & v.x & v.y & v.z;
		}
	}
}

#pragma endregion

class Vehicle : public MapElement
{
public:
	Vehicle();
	Vehicle(glm::vec2 position, glm::vec2 size, double rotation, std::vector<glm::vec2> points);
	Vehicle(double wheelbase, double track);
	~Vehicle();

	void UpdateState(double angle);
	void UpdateState(glm::vec2 position);
	void UpdateState(glm::vec2 position, double angle);
	void UpdateState(double x, double y, double z, double angle);
	void UpdateState(PathElement *pathElement);
	void UpdateState(SimulationState simulationState);
	double GetR(double angle);
	double GetRMin(double angle);
	double GetRMax(double angle);
	double GetAngle();
	double GetTrack();
	double GetWheelbase();
	double GetMaxInsideAngle();
	double GetMinOneManeouverLength(double insideAngle);
	double GetInsideAngleForRadius(double radius, CircleType circleType);
	double GetOutsideAngleForRadius(double radius, CircleType circleType);
	glm::vec2 GetDirTrack();
	glm::vec2 GetDirWheelbase();
	glm::vec2 GetTurnCentre(double angle, CircleType circleType);
	std::vector<glm::vec2> GetPoints2();

	void SetWheelbase(double wheelbase);
	void SetTrack(double track);
	void SetMaxInsideAngle(double maxInsideAngle);

	Circle *GetTurnCircle(double insideAngle, CircleType circleType, double angleFrom = 0, double angleTo = 2 * M_PI, ManeuverType maneuverType = ManeuverType::Front);

	Model* GetVehicleModel();
	Model* GetFrontLeftWheelModel();
	Model* GetFrontRightWheelModel();
	Model* GetRearLeftWheelModel();
	Model* GetRearRightWheelModel();

	void SetVehicleModel(Model* vehicleModel);
	void SetFrontLeftWheelModel(Model* frontLeftWheelModel);
	void SetFrontRightWheelModel(Model* frontRightWheelModel);
	void SetRearLeftWheelModel(Model* rearLeftWheelModel);
	void SetRearRightWheelModel(Model* rearRightWheelModel);

#pragma region Boost serialization.

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & boost::serialization::base_object<MapElement>(*this);
		ar & wheelbase;
		ar & track;
		ar & maxInsideAngle;

		ar & vehicleModel != NULL;
		ar & frontLeftWheelModel != NULL;
		ar & frontRightWheelModel != NULL;
		ar & rearLeftWheelModel != NULL;
		ar & rearRightWheelModel != NULL;

		if (vehicleModel != NULL) ar & *vehicleModel;
		if (frontLeftWheelModel != NULL) ar & *frontLeftWheelModel;
		if (frontRightWheelModel != NULL) ar & *frontRightWheelModel;
		if (rearLeftWheelModel != NULL) ar & *rearLeftWheelModel;
		if (rearRightWheelModel != NULL) ar & *rearRightWheelModel;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		bool isVehicleModel = false;
		bool isVehicleFrontLeftWheelModel = false;
		bool isVehicleFrontRightWheelModel = false;
		bool isVehicleRearLeftWheelModel = false;
		bool isVehicleRearRightModel = false;

		ar & boost::serialization::base_object<MapElement>(*this);
		ar & wheelbase;
		ar & track;
		ar & maxInsideAngle;

		ar & isVehicleModel;
		ar & isVehicleFrontLeftWheelModel;
		ar & isVehicleFrontRightWheelModel;
		ar & isVehicleRearLeftWheelModel;
		ar & isVehicleRearRightModel;

		if (isVehicleModel)
		{
			vehicleModel = new Model();
			ar & *vehicleModel;
		}

		if (isVehicleFrontLeftWheelModel)
		{
			frontLeftWheelModel = new Model();
			ar & *frontLeftWheelModel;
		}

		if (isVehicleFrontRightWheelModel)
		{
			frontRightWheelModel = new Model();
			ar & *frontRightWheelModel;
		}

		if (isVehicleRearLeftWheelModel)
		{
			rearLeftWheelModel = new Model();
			ar & *rearLeftWheelModel;
		}

		if (isVehicleRearRightModel)
		{
			rearRightWheelModel = new Model();
			ar & *rearRightWheelModel;
		}

		dirWheelbase = glm::vec2(1.0f, 0.0f);
		dirTrack = glm::vec2(0.0f, 1.0f);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

#pragma endregion

private:
	double wheelbase; // rozstaw osi
	double track; // rozstaw kó³
	double maxInsideAngle = M_PI / 4;

	glm::vec2 dirWheelbase;
	glm::vec2 dirTrack;

	Model* vehicleModel = NULL;
	Model* frontLeftWheelModel = NULL;
	Model* frontRightWheelModel = NULL;
	Model* rearLeftWheelModel = NULL;
	Model* rearRightWheelModel = NULL;
};

#endif