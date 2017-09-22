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

		buildings.push_back(MapElementModel(buildingName, buildingThumbnail, buildingModel));
	}

	int decorationsCount = settings.value("DecorationsCount").toInt();
	for (int i = 0; i < decorationsCount; i++)
	{
		std::string decorationName = settings.value(QString("Decoration_%1_Name").arg(i)).toString().toStdString();
		std::string decorationThumbnail = settings.value(QString("Decoration_%1_Thumbnail").arg(i)).toString().toStdString();
		std::string decorationModel = settings.value(QString("Decoration_%1_Model").arg(i)).toString().toStdString();

		decorations.push_back(MapElementModel(decorationName, decorationThumbnail, decorationModel));
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
	}

	settings.setValue("DecorationsCount", decorations.size());
	for (int i = 0; i < decorations.size(); i++)
	{
		settings.setValue(QString("Decoration_%1_Name").arg(i), QString::fromStdString(decorations[i].name));
		settings.setValue(QString("Decoration_%1_Thumbnail").arg(i), QString::fromStdString(decorations[i].thumbnail));
		settings.setValue(QString("Decoration_%1_Model").arg(i), QString::fromStdString(decorations[i].model));
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
