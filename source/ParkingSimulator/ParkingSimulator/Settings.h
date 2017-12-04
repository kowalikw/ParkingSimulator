#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>
#include <qsettings.h>
#include "nanovg.h"
#include <qcolor.h>
#include <qdir.h>

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

	int GetInt(std::string name);
	NVGcolor GetColor(std::string name);
	QColor GetQColor(std::string name);
	std::string GetString(std::string name);
	void SetInt(std::string name, int value);
	void SetColor(std::string name, NVGcolor value);
	void SetColor(std::string name, QColor value);
	void SetString(std::string name, std::string value);

	void LoadSettings();
	void SaveSettings();

	std::string GetLanguage();
	std::vector<MapElementModel> GetBuildings();
	std::vector<MapElementModel> GetDecorations();
	std::vector<MapElementModel> GetParkingPlaces();
	std::vector<MapElementModel> GetVehicles();
	std::vector<MapElementModel> GetTerrains();

	void SetLanguage(std::string language);
private:
	static Settings *s_instance;
	Settings()
	{
		LoadSettings();
	}

	QSettings *settings;

	std::string language;
	std::vector<MapElementModel> buildings;
	std::vector<MapElementModel> decorations;
	std::vector<MapElementModel> parkingPlaces;
	std::vector<MapElementModel> vehicles;
	std::vector<MapElementModel> terrains;
};

#endif