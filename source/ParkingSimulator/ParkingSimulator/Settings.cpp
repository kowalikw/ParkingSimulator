#include "Settings.h"

Settings* Settings::s_instance = NULL;

void Settings::LoadSettings()
{
	QSettings settings(settingsFile, QSettings::IniFormat);

	int buildingsCount = settings.value("BuildingsCount").toInt();
	for (int i = 0; i < buildingsCount; i++)
	{
		std::string buildingName = settings.value(QString("Building_%1_Name").arg(i)).toString().toStdString();
		std::string buildingThumbnail = settings.value(QString("Building_%1_Thumbnail").arg(i)).toString().toStdString();
		std::string buildingModel = settings.value(QString("Building_%1_Model").arg(i)).toString().toStdString();
		double buildingWidth = settings.value(QString("Building_%1_DefaultWidth").arg(i)).toDouble();
		double buildingHeight = settings.value(QString("Building_%1_DefaultHeight").arg(i)).toDouble();

		buildings.push_back(MapElementModel(buildingName, buildingThumbnail, buildingModel, buildingWidth, buildingHeight));
	}

	int decorationsCount = settings.value("DecorationsCount").toInt();
	for (int i = 0; i < decorationsCount; i++)
	{
		std::string decorationName = settings.value(QString("Decoration_%1_Name").arg(i)).toString().toStdString();
		std::string decorationThumbnail = settings.value(QString("Decoration_%1_Thumbnail").arg(i)).toString().toStdString();
		std::string decorationModel = settings.value(QString("Decoration_%1_Model").arg(i)).toString().toStdString();
		double decorationWidth = settings.value(QString("Decoration_%1_DefaultWidth").arg(i)).toDouble();
		double decorationHeight = settings.value(QString("Decoration_%1_DefaultHeight").arg(i)).toDouble();

		decorations.push_back(MapElementModel(decorationName, decorationThumbnail, decorationModel, decorationWidth, decorationHeight));
	}

	int parkingPlacesCount = settings.value("ParkingPlacesCount").toInt();
	for (int i = 0; i < parkingPlacesCount; i++)
	{
		std::string parkingPlaceName = settings.value(QString("ParkingPlace_%1_Name").arg(i)).toString().toStdString();
		std::string parkingPlaceThumbnail = settings.value(QString("ParkingPlace_%1_Thumbnail").arg(i)).toString().toStdString();
		std::string parkingPlaceModel = settings.value(QString("ParkingPlace_%1_Model").arg(i)).toString().toStdString();
		double parkingPlaceWidth = settings.value(QString("ParkingPlace_%1_DefaultWidth").arg(i)).toDouble();
		double parkingPlaceHeight = settings.value(QString("ParkingPlace_%1_DefaultHeight").arg(i)).toDouble();

		parkingPlaces.push_back(MapElementModel(parkingPlaceName, parkingPlaceThumbnail, parkingPlaceModel, parkingPlaceWidth, parkingPlaceHeight));
	}
}

void Settings::SaveSettings()
{
	QSettings settings(settingsFile, QSettings::IniFormat);

	settings.setValue("BuildingsCount", buildings.size());
	for (int i = 0; i < buildings.size(); i++)
	{
		settings.setValue(QString("Building_%1_Name").arg(i), QString::fromStdString(buildings[i].name));
		settings.setValue(QString("Building_%1_Thumbnail").arg(i), QString::fromStdString(buildings[i].thumbnail));
		settings.setValue(QString("Building_%1_Model").arg(i), QString::fromStdString(buildings[i].model));
		settings.setValue(QString("Building_%1_DefaultWidth").arg(i), buildings[i].defaultWidth);
		settings.setValue(QString("Building_%1_DefaultHeight").arg(i), buildings[i].defaultHeight);
	}

	settings.setValue("DecorationsCount", decorations.size());
	for (int i = 0; i < decorations.size(); i++)
	{
		settings.setValue(QString("Decoration_%1_Name").arg(i), QString::fromStdString(decorations[i].name));
		settings.setValue(QString("Decoration_%1_Thumbnail").arg(i), QString::fromStdString(decorations[i].thumbnail));
		settings.setValue(QString("Decoration_%1_Model").arg(i), QString::fromStdString(decorations[i].model));
		settings.setValue(QString("Decoration_%1_DefaultWidth").arg(i), decorations[i].defaultWidth);
		settings.setValue(QString("Decoration_%1_DefaultHeight").arg(i), decorations[i].defaultHeight);
	}

	settings.setValue("ParkingPlacesCount", parkingPlaces.size());
	for (int i = 0; i < parkingPlaces.size(); i++)
	{
		settings.setValue(QString("ParkingPlace_%1_Name").arg(i), QString::fromStdString(parkingPlaces[i].name));
		settings.setValue(QString("ParkingPlace_%1_Thumbnail").arg(i), QString::fromStdString(parkingPlaces[i].thumbnail));
		settings.setValue(QString("ParkingPlace_%1_Model").arg(i), QString::fromStdString(parkingPlaces[i].model));
		settings.setValue(QString("ParkingPlace_%1_DefaultWidth").arg(i), parkingPlaces[i].defaultWidth);
		settings.setValue(QString("ParkingPlace_%1_DefaultHeight").arg(i), parkingPlaces[i].defaultHeight);
	}
}

std::vector<MapElementModel> Settings::GetBuildings()
{
	return this->buildings;
}

std::vector<MapElementModel> Settings::GetDecorations()
{
	return this->decorations;
}

std::vector<MapElementModel> Settings::GetParkingPlaces()
{
	return this->parkingPlaces;
}

std::vector<MapElementModel> Settings::GetVehicles()
{
	return this->vehicles;
}
