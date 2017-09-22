#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>
#include <qsettings.h>

struct MapElementModel
{
	std::string name;
	std::string thumbnail;
	std::string model;
	double defaultWidth;
	double defaultHeight;

	MapElementModel(std::string name, std::string thumbnail, std::string model, double defaultWidth, double defaultHeight) 
		: name(name), thumbnail(thumbnail), model(model), defaultWidth(defaultWidth), defaultHeight(defaultHeight) { }
};

class Settings
{
public:
	static Settings *getInstance()
	{
		if (!Settings::s_instance)
			Settings::s_instance = new Settings;
		return Settings::s_instance;
	}

	void LoadSettings();
	void SaveSettings();

	std::vector<MapElementModel> GetBuildings();
	std::vector<MapElementModel> GetDecorations();
	std::vector<MapElementModel> GetParkingPlaces();
	std::vector<MapElementModel> GetVehicles();
private:
	static Settings *s_instance;
	Settings()
	{
		// TODO:
		/*for (int i = 0; i < 6; i++)
		{
			buildings.push_back(MapElementModel("budynek", "Resources/MapEditorIcons/AddBuildingIcons/default.png", "", 100, 100));
		}
		for (int i = 0; i < 4; i++)
		{
			decorations.push_back(MapElementModel("dekoracja", "Resources/MapEditorIcons/AddBuildingIcons/default.png", "", 100, 100));
		}
		for (int i = 0; i < 2; i++)
		{
			parkingPlaces.push_back(MapElementModel("miejsce parkowania", "Resources/MapEditorIcons/AddBuildingIcons/default.png", "", 200, 100));
		}*/
		LoadSettings();
	}

	QString settingsFile = "C:\\Users\\Wojtek\\Source\\Repos\\ParkingSimulator\\source\\ParkingSimulator\\x64\\Debug\\settings.ini";

	std::vector<MapElementModel> buildings;
	std::vector<MapElementModel> decorations;
	std::vector<MapElementModel> parkingPlaces;
	std::vector<MapElementModel> vehicles;
};

#endif