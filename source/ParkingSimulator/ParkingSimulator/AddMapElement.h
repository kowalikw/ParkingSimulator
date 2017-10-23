#pragma once

#include <QDialog>
#include <vector>
#include "ui_AddMapElement.h"
#include "Settings.h"
#include "MapElement.h"
#include "Obstacle.h"
#include "ParkingSpace.h"
#include "Terrain.h"
#include "Language.h"

using namespace Ui;

enum AddMapElementType
{
	B,
	D,
	C,
	P,
	T
};

class AddMapElement : public QDialog
{
	Q_OBJECT

public:
	AddMapElement(AddMapElementType type, QWidget *parent = Q_NULLPTR);
	~AddMapElement();
	MapElement * GetNewMapElement();
	Terrain * GetNewTerrain();

private:
	AddMapElementClass ui;
	AddMapElementType type;

	Terrain *newTerrain = NULL;
	MapElement *newMapElement = NULL;
	MapElementModel *model = NULL;

	std::vector<MapElementModel> buildings;
	std::vector<MapElementModel> decorations;
	std::vector<MapElementModel> parkingPlaces;
	std::vector<MapElementModel> vehicles;
	std::vector<MapElementModel> terrains;

	void initBuildings();
	void initDecorations();
	void initParkingSpaces();
	void initVehicles();
	void initTerrains();
	void createElement(int index);
public slots:
	void addElement();
	void clickedSlot();
};
