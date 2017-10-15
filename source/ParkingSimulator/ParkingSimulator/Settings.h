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
	int r;
	int g;
	int b;

	MapElementModel(std::string name, std::string thumbnail, std::string model, double defaultWidth, double defaultHeight) 
		: name(name), thumbnail(thumbnail), model(model), defaultWidth(defaultWidth), defaultHeight(defaultHeight) { }

	MapElementModel(std::string name, std::string thumbnail, std::string model, int r, int g, int b)
		: name(name), thumbnail(thumbnail), model(model), r(r), g(g), b(b) { }
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
	std::vector<MapElementModel> GetTerrains();
private:
	static Settings *s_instance;
	Settings()
	{
		LoadSettings();
	}

	QString settingsFile = "C:\\Users\\Wojtek\\Source\\Repos\\ParkingSimulator-NEW\\source\\ParkingSimulator\\x64\\Debug\\settings.ini";

	std::vector<MapElementModel> buildings;
	std::vector<MapElementModel> decorations;
	std::vector<MapElementModel> parkingPlaces;
	std::vector<MapElementModel> vehicles;
	std::vector<MapElementModel> terrains;
};

#endif