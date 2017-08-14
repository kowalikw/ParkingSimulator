#include "ParkingSimulator.h"

ParkingSimulator::ParkingSimulator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: #2a2a2a;");

	((MapEditorGLHost*)ui.glMapEditor)->SetMapEditor(&mapEditor);

	renderTimer = new QTimer();
	renderTimer->setInterval(10);
	renderTimer->start();

	connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderTimerCall()));
	connect(ui.btnNewMap, SIGNAL(released()), this, SLOT(createMap()));
	connect(ui.btnSaveMap, SIGNAL(released()), this, SLOT(saveMap()));
	connect(ui.btnLoadMap, SIGNAL(released()), this, SLOT(loadMap()));
	connect(ui.btnAddBuilding, SIGNAL(released()), this, SLOT(addBuilding()));
	connect(ui.btnAddDecoration, SIGNAL(released()), this, SLOT(addDecoration()));
	connect(ui.btnAddParkPlace, SIGNAL(released()), this, SLOT(addParkPlace()));
	connect(ui.btnAddRoad, SIGNAL(released()), this, SLOT(addRoad()));

	ui.treeMapElements->setColumnCount(1);
	QList<QTreeWidgetItem *> items;

	auto buildings = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Buildings")));
	auto decorations = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Decorations")));
	auto roads = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Roads")));
	auto parkPlaces = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Park places")));

	items.append(buildings);
	items.append(decorations);
	items.append(roads);
	items.append(parkPlaces);

	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 10; ++i)
		items.append(new QTreeWidgetItem(buildings, QStringList(QString("Building: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 5; ++i)
		items.append(new QTreeWidgetItem(decorations, QStringList(QString("Decoration: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 5; ++i)
		items.append(new QTreeWidgetItem(roads, QStringList(QString("Road: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 5; ++i)
		items.append(new QTreeWidgetItem(parkPlaces, QStringList(QString("Park place: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);
}

void ParkingSimulator::renderTimerCall()
{
	ui.glMapEditor->repaint();
	ui.glPathPlanner->repaint();

	if (mapEditor.GetResetAddButtons())
		clearAddButtons();
}

#pragma region Start.



#pragma endregion

#pragma region Map editor.

void ParkingSimulator::createMap()
{
	CreateMap createMapWindow;
	if (createMapWindow.exec())
	{
		int width = createMapWindow.GetMapWidth();
		int height = createMapWindow.GetMapHeight();

		mapEditor.CreateMap(width, height);
	}
}

void ParkingSimulator::saveMap()
{
}

void ParkingSimulator::loadMap()
{
}

void ParkingSimulator::addBuilding()
{
	if (mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace() || mapEditor.GetAddRoad())
		clearAddButtons();

	if (!mapEditor.GetAddBuilding())
	{
		ui.btnAddBuilding->setStyleSheet("background-color: #d86a39;");
		mapEditor.SetAddBuilding(true);
		mapEditor.AddObstacle("budynek", 100, 100, ObstacleType::Building);
	}
	else
	{
		ui.btnAddBuilding->setStyleSheet("");
		mapEditor.SetAddBuilding(false);
	}
}

void ParkingSimulator::addDecoration()
{
	if (mapEditor.GetAddBuilding() || mapEditor.GetAddParkPlace() || mapEditor.GetAddRoad())
		clearAddButtons();

	if (!mapEditor.GetAddDecoration())
	{
		ui.btnAddDecoration->setStyleSheet("background-color: #d86a39;");
		mapEditor.SetAddDecoration(true);
	}
	else
	{
		ui.btnAddDecoration->setStyleSheet("");
		mapEditor.SetAddDecoration(false);
	}
}

void ParkingSimulator::addRoad()
{
	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace())
		clearAddButtons();

	if (!mapEditor.GetAddRoad())
	{
		ui.btnAddRoad->setStyleSheet("background-color: #d86a39;");
		mapEditor.SetAddRoad(true);
	}
	else
	{
		ui.btnAddRoad->setStyleSheet("");
		mapEditor.SetAddRoad(false);
	}
}

void ParkingSimulator::addParkPlace()
{
	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddRoad())
		clearAddButtons();

	if (!mapEditor.GetAddParkPlace())
	{
		ui.btnAddParkPlace->setStyleSheet("background-color: #d86a39;");
		mapEditor.SetAddParkPlace(true);
	}
	else
	{
		ui.btnAddParkPlace->setStyleSheet("");
		mapEditor.SetAddParkPlace(false);
	}
}

void ParkingSimulator::clearAddButtons()
{
	mapEditor.SetAddBuilding(false);
	mapEditor.SetAddDecoration(false);
	mapEditor.SetAddParkPlace(false);
	mapEditor.SetAddRoad(false);

	clearAddButtonsStyle();
}

void ParkingSimulator::clearAddButtonsStyle()
{
	if (!mapEditor.GetAddBuilding())
		ui.btnAddBuilding->setStyleSheet("");
	if (!mapEditor.GetAddDecoration())
		ui.btnAddDecoration->setStyleSheet("");
	if (!mapEditor.GetAddParkPlace())
		ui.btnAddParkPlace->setStyleSheet("");
	if (!mapEditor.GetAddRoad())
		ui.btnAddRoad->setStyleSheet("");
}

#pragma endregion

#pragma region Path planner.



#pragma endregion