#ifndef VEHICLE_EDITOR_H
#define VEHICLE_EDITOR_H

#include "CommonHelper.h"
#include "Vehicle.h"

class VehicleEditor
{
public:
	VehicleEditor();
	~VehicleEditor();

	void CreateNewVehicle();
	void SaveVehicle(std::string path);
	void OpenVehicle(std::string path);

	void LoadVehicleModel(std::string vehicleModel);
	void LoadFrontLeftWheelModel(std::string frontLeftWheelModel);
	void LoadFrontRightWheelModel(std::string frontRightWheelModel);
	void LoadRearLeftWheelModel(std::string rearLeftWheelModel);
	void LoadRearRightWheelModel(std::string rearRightWheelModel);

	void UpdateFrontLeftWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void UpdateFrontRightWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void UpdateRearLeftWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void UpdateRearRightWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	bool GetVehicleModelChanged();
	bool GetVehicleFrontLeftWheelModelChanged();
	bool GetVehicleFrontRightWheelModelChanged();
	bool GetVehicleRearLeftWheelModelChanged();
	bool GetVehicleRearRightWheelModelChanged();

	void SetVehicleModelChanged(bool vehicleModelChanged);
	void SetVehicleFrontLeftWheelModelChanged(bool vehicleFrontLeftWheelModelChanged);
	void SetVehicleFrontRightWheelModelChanged(bool vehicleFrontRightWheelModelChanged);
	void SetVehicleRearLeftWheelModelChanged(bool vehicleRearLeftWheelModelChanged);
	void SetVehicleRearRightWheelModelChanged(bool vehicleRearRightWheelModelChanged);

	Vehicle * GetVehicle();

private:
	Vehicle *vehicle = NULL;

	bool vehicleModelChanged = false;
	bool vehicleFrontLeftWheelModelChanged = false;
	bool vehicleFrontRightWheelModelChanged = false;
	bool vehicleRearLeftWheelModelChanged = false;
	bool vehicleRearRightWheelModelChanged = false;
};

#endif // !VEHICLE_EDITOR_H