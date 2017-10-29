#include "VehicleEditor.h"

VehicleEditor::VehicleEditor()
{
	vehicle = new Vehicle();
}

VehicleEditor::~VehicleEditor()
{
}

void VehicleEditor::CreateNewVehicle()
{
	this->vehicle = new Vehicle();
}

void VehicleEditor::SaveVehicle(std::string path)
{
	/*simulation = new Simulation();
	simulation->SetMap(map);
	simulation->SetExpandedMap(expandedMap);
	simulation->SetVehicle(vehicle);
	simulation->SetPolylinePath(polylinePath);
	simulation->SetParkingPathStart(parkingPathStart);
	simulation->SetParkingPathEnd(parkingPathEnd);
	simulation->SetFinalPath(finalPath);
	simulation->SetStartPoint(startPoint);
	simulation->SetEndPoint(endPoint);
	simulation->SetStartDirection(startDirection);
	simulation->SetEndDirection(endDirection);
	simulation->SetStartParkingSpace(startParkingSpace);
	simulation->SetEndParkingSpace(endParkingSpace);
	simulation->SetExpandSize(expandSizePercent);
	simulation->SetCollisionDetectionDensity(collisionDetectionDensity);
	simulation->SetPathPlanningAlgorithm(pathPlanningAlgorithm);
	simulation->SetUseAdmissibleArcsOnly(useAdmissibleArcsOnly);*/

	ofstream f(path, ios::out);

	boost::archive::text_oarchive oa(f);

	oa << *vehicle;
}

void VehicleEditor::OpenVehicle(std::string path)
{
	vehicle = new Vehicle();

	std::ifstream ifs(path);

	boost::archive::text_iarchive ia(ifs);

	ia >> *vehicle;

	SetVehicleModelChanged(vehicle->GetVehicleModel() != NULL);
	SetVehicleFrontLeftWheelModelChanged(vehicle->GetFrontLeftWheelModel() != NULL);
	SetVehicleFrontRightWheelModelChanged(vehicle->GetFrontRightWheelModel() != NULL);
	SetVehicleRearLeftWheelModelChanged(vehicle->GetRearLeftWheelModel() != NULL);
	SetVehicleRearRightWheelModelChanged(vehicle->GetRearRightWheelModel() != NULL);
}

void VehicleEditor::LoadVehicleModel(std::string vehicleModel)
{
	vehicle->SetVehicleModel(new Model(vehicleModel, false));

	SetVehicleModelChanged(true);
}

void VehicleEditor::LoadFrontLeftWheelModel(std::string frontLeftWheelModel)
{
	vehicle->SetFrontLeftWheelModel(new Model(frontLeftWheelModel, false));

	SetVehicleFrontLeftWheelModelChanged(true);
}

void VehicleEditor::LoadFrontRightWheelModel(std::string frontRightWheelModel)
{
	vehicle->SetFrontRightWheelModel(new Model(frontRightWheelModel, false));

	SetVehicleFrontRightWheelModelChanged(true);
}

void VehicleEditor::LoadRearLeftWheelModel(std::string rearLeftWheelModel)
{
	vehicle->SetRearLeftWheelModel(new Model(rearLeftWheelModel, false));

	SetVehicleRearLeftWheelModelChanged(true);
}

void VehicleEditor::LoadRearRightWheelModel(std::string rearRightWheelModel)
{
	vehicle->SetRearRightWheelModel(new Model(rearRightWheelModel, false));

	SetVehicleRearRightWheelModelChanged(true);
}

void VehicleEditor::UpdateFrontLeftWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	vehicle->GetFrontLeftWheelModel()->Translate(position);
	vehicle->GetFrontLeftWheelModel()->Rotate(rotation);
	vehicle->GetFrontLeftWheelModel()->Scale(scale);
}

void VehicleEditor::UpdateFrontRightWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	vehicle->GetFrontRightWheelModel()->Translate(position);
	vehicle->GetFrontRightWheelModel()->Rotate(rotation);
	vehicle->GetFrontRightWheelModel()->Scale(scale);
}

void VehicleEditor::UpdateRearLeftWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	vehicle->GetRearLeftWheelModel()->Translate(position);
	vehicle->GetRearLeftWheelModel()->Rotate(rotation);
	vehicle->GetRearLeftWheelModel()->Scale(scale);
}

void VehicleEditor::UpdateRearRightWheelProperties(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	vehicle->GetRearRightWheelModel()->Translate(position);
	vehicle->GetRearRightWheelModel()->Rotate(rotation);
	vehicle->GetRearRightWheelModel()->Scale(scale);
}

bool VehicleEditor::GetVehicleModelChanged()
{
	return this->vehicleModelChanged;
}

bool VehicleEditor::GetVehicleFrontLeftWheelModelChanged()
{
	return this->vehicleFrontLeftWheelModelChanged;
}

bool VehicleEditor::GetVehicleFrontRightWheelModelChanged()
{
	return this->vehicleFrontRightWheelModelChanged;
}

bool VehicleEditor::GetVehicleRearLeftWheelModelChanged()
{
	return this->vehicleRearLeftWheelModelChanged;
}

bool VehicleEditor::GetVehicleRearRightWheelModelChanged()
{
	return this->vehicleRearRightWheelModelChanged;
}

void VehicleEditor::SetVehicleModelChanged(bool vehicleModelChanged)
{
	this->vehicleModelChanged = vehicleModelChanged;
}

void VehicleEditor::SetVehicleFrontLeftWheelModelChanged(bool vehicleFrontLeftWheelModelChanged)
{
	this->vehicleFrontLeftWheelModelChanged = vehicleFrontLeftWheelModelChanged;
}

void VehicleEditor::SetVehicleFrontRightWheelModelChanged(bool vehicleFrontRightWheelModelChanged)
{
	this->vehicleFrontRightWheelModelChanged = vehicleFrontRightWheelModelChanged;
}

void VehicleEditor::SetVehicleRearLeftWheelModelChanged(bool vehicleRearLeftWheelModelChanged)
{
	this->vehicleRearLeftWheelModelChanged = vehicleRearLeftWheelModelChanged;
}

void VehicleEditor::SetVehicleRearRightWheelModelChanged(bool vehicleRearRightWheelModelChanged)
{
	this->vehicleRearRightWheelModelChanged = vehicleRearRightWheelModelChanged;
}

Vehicle * VehicleEditor::GetVehicle()
{
	return this->vehicle;
}