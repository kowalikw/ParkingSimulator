#pragma once

#include <QDialog>
#include <vector>
#include "ui_AddMapElement.h"
#include "Settings.h"
#include "MapElement.h"
#include "Obstacle.h"
#include "ParkingSpace.h"

using namespace Ui;

enum AddMapElementType
{
	B,
	D,
	P
};

class AddMapElement : public QDialog
{
	Q_OBJECT

public:
	AddMapElement(AddMapElementType type, QWidget *parent = Q_NULLPTR);
	~AddMapElement();
	MapElement * GetNewMapElement();

private:
	AddMapElementClass ui;
	AddMapElementType type;

	MapElement *newMapElement = NULL;
	MapElementModel *model = NULL;

	std::vector<MapElementModel> buildings;
	std::vector<MapElementModel> decorations;
	std::vector<MapElementModel> parkingPlaces;
	std::vector<MapElementModel> vehicles;

	void initBuildings();
	void initDecorations();
	void initParkingSpaces();
	void createElement(int index);
public slots:
	void addElement();
	void clickedSlot();
};
