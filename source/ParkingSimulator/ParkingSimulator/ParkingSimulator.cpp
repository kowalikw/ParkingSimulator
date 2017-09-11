#include "ParkingSimulator.h"

#include <fstream>

#include <iostream>

#include <QFileDialog>

ParkingSimulator::ParkingSimulator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: #2a2a2a;");

	((MapEditorGLHost*)ui.glMapEditor)->SetMapEditor(&mapEditor);
	((VisualisationGLHost*)ui.glVisualisation)->SetVisualisation(&visualisation);

	renderTimer = new QTimer();
	renderTimer->setInterval(10);
	renderTimer->start();

	updateTimer = new QTimer();
	updateTimer->setInterval(20);
	updateTimer->start();

	connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderTimerCall()));
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerCall()));
	connect(ui.btnNewMap, SIGNAL(released()), this, SLOT(createMap()));
	connect(ui.btnSaveMap, SIGNAL(released()), this, SLOT(saveMap()));
	connect(ui.btnLoadMap, SIGNAL(released()), this, SLOT(loadMap()));
	connect(ui.btnAddBuilding, SIGNAL(released()), this, SLOT(addBuilding()));
	connect(ui.btnAddDecoration, SIGNAL(released()), this, SLOT(addDecoration()));
	connect(ui.btnAddParkPlace, SIGNAL(released()), this, SLOT(addParkPlace()));
	connect(ui.btnAddRoad, SIGNAL(released()), this, SLOT(addRoad()));	

	connect(ui.btnAddSimulation, SIGNAL(released()), this, SLOT(addSimulation()));
	connect(ui.btnRemoveSimulation, SIGNAL(released()), this, SLOT(removeSimulation()));
	connect(ui.btnInfoSimulation, SIGNAL(released()), this, SLOT(infoSimulation()));
	connect(ui.btnPlayPauseSimulation, SIGNAL(released()), this, SLOT(playPauseSimulation()));
	connect(ui.btnStopSimulation, SIGNAL(released()), this, SLOT(stopSimulation()));
	connect(ui.btnVisualisation2D, SIGNAL(released()), this, SLOT(enableVisualisation2D()));
	connect(ui.btnVisualisation3D, SIGNAL(released()), this, SLOT(enableVisualisation3D()));
	connect(ui.showPathElements, SIGNAL(stateChanged(int)), this, SLOT(showSimulationPath(int)));
	connect(ui.simulationPrograssBar, SIGNAL(sliderMoved(int)), this, SLOT(simulationProgressBarChange(int)));
	connect(ui.simulationPrograssBar, SIGNAL(sliderPressed()), this, SLOT(simulationProgressBarPressed()));
	connect(ui.simulationPrograssBar, SIGNAL(sliderReleased()), this, SLOT(simulationProgressBarReleased()));

	ui.simulationPrograssBar->setTracking(false);

	ui.treeMapElements->setColumnCount(1);
	QList<QTreeWidgetItem *> items;

	buildings = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Buildings")));
	decorations = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Decorations")));
	roads = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Roads")));
	parkPlaces = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Park places")));

	items.append(buildings);
	items.append(decorations);
	items.append(roads);
	items.append(parkPlaces);

	ui.treeMapElements->insertTopLevelItems(0, items);
}

void ParkingSimulator::renderTimerCall()
{
	ui.glMapEditor->repaint();
	ui.glPathPlanner->repaint();
	ui.glVisualisation->repaint();

	if (mapEditor.GetResetAddButtons())
	{
		clearAddButtons();
		updateMapElementsTree();
	}
}

void ParkingSimulator::updateTimerCall()
{
	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();

		float t = simulation->GetCurrentSimulationTime() / simulation->GetSimulationTime();

		ui.simulationPrograssBar->setValue(t * 1000);
	}
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
	QString filter = tr("Map files (*.map)");
	QString filePath = QFileDialog::getSaveFileName(this, "Save map", QString(), tr("All files (*.*);;Map files (*.map)"), &filter);

	if(filePath != "")
		mapEditor.SaveMap(filePath.toStdString());
}

void ParkingSimulator::loadMap()
{
	QString filter = tr("Map files (*.map)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open map", QString(), tr("All files (*.*);;Map files (*.map)"), &filter);

	if(filePath != "")
		mapEditor.LoadMap(filePath.toStdString());
}

void ParkingSimulator::addBuilding()
{
	if (mapEditor.GetMap() == NULL)
		return;

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
	if (mapEditor.GetMap() == NULL)
		return;

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
	if (mapEditor.GetMap() == NULL)
		return;

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
	if (mapEditor.GetMap() == NULL)
		return;

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

void ParkingSimulator::updateMapElementsTree()
{
	QList<QTreeWidgetItem *> items;

	std::vector<Obstacle*> obstacles = mapEditor.GetMap()->GetObstacles();
	for (int i = 0; i < mapEditor.GetMap()->GetObstacles().size(); ++i)
		if(obstacles[i]->GetType() == ObstacleType::Building)
			items.append(new QTreeWidgetItem(buildings, QStringList(QString::fromStdString(obstacles[i]->GetName()))));
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
	std::vector<ParkingSpace*> parkingSpaces = mapEditor.GetMap()->GetParkingSpaces();
	for (int i = 0; i < parkingSpaces.size(); ++i)
		items.append(new QTreeWidgetItem(buildings, QStringList(QString::fromStdString(parkingSpaces[i]->GetName()))));
	ui.treeMapElements->insertTopLevelItems(0, items);
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

#pragma region Visualisation.

void ParkingSimulator::addSimulation()
{
	/*Map map;
	Vehicle vehicle(200, 100);
	Path path;

	MapElement *parkingSpace = new ParkingSpace(glm::vec2(200, 200), glm::vec2(300, 120));
	map.AddMapElement(parkingSpace);

	Simulation *simulation = new Simulation(map, vehicle, path);
	visualisation.AddSimulation(simulation);*/

	int lala2 = 0;
}

void ParkingSimulator::removeSimulation()
{
}

void ParkingSimulator::infoSimulation()
{
}

void ParkingSimulator::playPauseSimulation()
{
	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();
		if (simulation->IsStarted())
			simulation->Pause();
		else
		{
			float t = ui.simulationPrograssBar->value() / 1000.0f;
			simulation->SetCurrentSimulationTime(t * simulation->GetSimulationTime());
			simulation->Start();
		}
	}
}

void ParkingSimulator::stopSimulation()
{
	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();

		simulation->Stop();
	}
}

void ParkingSimulator::enableVisualisation2D()
{
	visualisation.SetVisualisation2D(true);
	visualisation.SetVisualisation3D(false);
	ui.btnVisualisation2D->setStyleSheet("border: 3px solid #d86a39;");
	ui.btnVisualisation3D->setStyleSheet("");
}

void ParkingSimulator::enableVisualisation3D()
{
	visualisation.SetVisualisation2D(false);
	visualisation.SetVisualisation3D(true);
	ui.btnVisualisation2D->setStyleSheet("");
	ui.btnVisualisation3D->setStyleSheet("border: 3px solid #d86a39;");
}

void ParkingSimulator::showSimulationPath(int checked)
{
	visualisation.SetShowPath(checked == Qt::Checked);
}

void ParkingSimulator::simulationProgressBarChange(int time)
{
	float t = time / 1000.0f;

	if (t == 1.0f)
		t = 0.999f;

	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();

		simulation->SetCurrentSimulationTime(t * simulation->GetSimulationTime());

		visualisation.GetCurrentSimulation()->UpdateSimulationState();
	}
}

void ParkingSimulator::simulationProgressBarPressed()
{

}

void ParkingSimulator::simulationProgressBarReleased()
{

}

#pragma endregion