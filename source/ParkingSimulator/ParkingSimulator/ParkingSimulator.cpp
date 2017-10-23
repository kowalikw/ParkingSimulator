#include "ParkingSimulator.h"

#include <fstream>

#include <iostream>

#include <QFileDialog>

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

ParkingSimulator::ParkingSimulator(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: #2a2a2a;");

	Language::getInstance()->LoadLanguage(Settings::getInstance()->GetLanguage());

	((MapEditorGLHost*)ui.glMapEditor)->SetMapEditor(&mapEditor);
	((PathPlannerGLHost*)ui.glPathPlanner)->SetPathPlanner(&pathPlanner);
	((VisualisationGLHost*)ui.glVisualisation)->SetVisualisation(&visualisation);
	((OpenGLHost3D*)ui.glVisualisation3D)->SetVisualisation(&visualisation);

	renderTimer = new QTimer();
	renderTimer->setInterval(10);
	renderTimer->start();

	render3DTimer = new QTimer();
	render3DTimer->setInterval(10);
	render3DTimer->start();

	updateTimer = new QTimer();
	updateTimer->setInterval(20);
	updateTimer->start();

	connect(ui.btnGoToMapEditor, SIGNAL(released()), this, SLOT(goToMapEditor()));
	connect(ui.btnGoToPathPlanner, SIGNAL(released()), this, SLOT(goToPathPlanner()));
	connect(ui.btnGoToVisualisation, SIGNAL(released()), this, SLOT(goToVisualisation()));
	connect(ui.btnGoToSettings, SIGNAL(released()), this, SLOT(goToSettings()));

	connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderTimerCall()));
	connect(render3DTimer, SIGNAL(timeout()), this, SLOT(render3DTimerCall()));
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerCall()));

	connect(ui.btnNewMap, SIGNAL(released()), this, SLOT(createMap()));
	connect(ui.btnSaveMap, SIGNAL(released()), this, SLOT(saveMap()));
	connect(ui.btnLoadMap, SIGNAL(released()), this, SLOT(loadMap()));
	connect(ui.btnClearMap, SIGNAL(released()), this, SLOT(clearMap()));
	connect(ui.btnAddBuilding, SIGNAL(released()), this, SLOT(addBuilding()));
	connect(ui.btnAddDecoration, SIGNAL(released()), this, SLOT(addDecoration()));
	connect(ui.btnAddParkPlace, SIGNAL(released()), this, SLOT(addParkPlace()));
	connect(ui.btnAddCar, SIGNAL(released()), this, SLOT(addCar()));	
	connect(ui.btnAddTerrain, SIGNAL(released()), this, SLOT(addTerrain()));
	connect(ui.btnMapElementRemove, SIGNAL(released()), this, SLOT(mapElementRemove()));
	connect(ui.btnMapElementSaveProperties, SIGNAL(released()), this, SLOT(mapElementSaveProperties()));

	connect(ui.btnNewSimulation, SIGNAL(released()), this, SLOT(newSimulation()));
	connect(ui.btnOpenSimulation, SIGNAL(released()), this, SLOT(openSimulation()));
	connect(ui.btnSaveSimulation, SIGNAL(released()), this, SLOT(saveSimulation()));
	connect(ui.btnSetMap, SIGNAL(released()), this, SLOT(setMap()));
	connect(ui.btnSetVehicle, SIGNAL(released()), this, SLOT(setVehicle()));
	connect(ui.btnSetStart, SIGNAL(released()), this, SLOT(setStart()));
	connect(ui.btnSetEnd, SIGNAL(released()), this, SLOT(setEnd()));
	connect(ui.btnShowVoronoiGraph, SIGNAL(released()), this, SLOT(showVoronoiGraph()));
	connect(ui.btnShowFullVoronoiVisibilityGraph, SIGNAL(released()), this, SLOT(showFullVoronoiVisibilityGraph()));
	connect(ui.btnShowPolylinePath, SIGNAL(released()), this, SLOT(showPolylinePath()));
	connect(ui.btnShowParkingPath, SIGNAL(released()), this, SLOT(showParkingPath()));
	connect(ui.btnShowFinalPath, SIGNAL(released()), this, SLOT(showFinalPath()));
	connect(ui.btnShowExpandedObstacles, SIGNAL(released()), this, SLOT(showExpandedObstacles()));
	connect(ui.btnFindPath, SIGNAL(released()), this, SLOT(findPath()));
	connect(ui.pathElementsList, SIGNAL(itemSelectionChanged()), this, SLOT(pathElementSelectionChange()));

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
	connect(ui.listSimulations, SIGNAL(itemSelectionChanged()), this, SLOT(simulationSelectionChange()));

	//settings
	connect(ui.btnSaveSettings, SIGNAL(released()), this, SLOT(saveSettings()));

	connect(ui.treeMapElements, SIGNAL(itemSelectionChanged()), this, SLOT(treeMapElementsSelectionChanged()));

	ui.mapElementProperties->hide();
	ui.pathElementProperties->hide();

	ui.simulationPrograssBar->setTracking(false);

	ui.treeMapElements->setColumnCount(1);
	ui.treeMapElements->insertTopLevelItems(0, *mapEditor.GetMapElementsTreeItems());

	// Path planner start

	ui.mapPropertiesForm->hide();
	ui.vehiclePropertiesForm->hide();
	ui.pathElementsList->hide();

	// Path planner end

	ui.glVisualisation3D->hide();

	initializeLanguages();
}

void ParkingSimulator::renderTimerCall()
{
	ui.glMapEditor->repaint();
	ui.glPathPlanner->repaint();
	ui.glVisualisation->repaint();

	if (mapEditor.GetResetAddButtons())
	{
		clearMapEditorButtons();
		updateMapElementsTree();
	}
}

void ParkingSimulator::render3DTimerCall()
{
	ui.glVisualisation3D->repaint();
}

void ParkingSimulator::setMapProperties()
{
	if (pathPlanner.GetMap() != NULL)
	{
		Map *map = pathPlanner.GetMap();

		ui.isMapSet->hide();
		ui.mapPropertiesForm->show();
		ui.mapWidthLabel->setText(QString("%1 m").arg(map->GetWidth()));
		ui.mapHeightLabel->setText(QString("%1 m").arg(map->GetHeight()));
		ui.mapElementsCountLabel->setText(QString("%1").arg(map->GetMapElements().size()));
	}
	else
	{
		ui.isMapSet->show();
		ui.mapPropertiesForm->hide();
	}
}

void ParkingSimulator::setVehicleProperties()
{
	if (pathPlanner.GetVehicle() != NULL)
	{
		Vehicle *vehicle = pathPlanner.GetVehicle();

		ui.isVehicleSet->hide();
		ui.vehiclePropertiesForm->show();
		ui.vehicleNameLabel->setText(QString::fromStdString(vehicle->GetName()));
		ui.vehicleWheelbaseLabel->setText(QString("%1 m").arg(vehicle->GetWheelbase()));
		ui.vehicleTrackLabel->setText(QString("%1 m").arg(vehicle->GetTrack()));
		ui.vehicleMaxAngleLabel->setText(QString("%1 deg").arg(glm::degrees(vehicle->GetMaxInsideAngle())));
	}
	else
	{
		ui.isVehicleSet->show();
		ui.vehiclePropertiesForm->hide();
	}
}

void ParkingSimulator::setPathProperties()
{
	if (pathPlanner.GetStartPoint() != NULL)
	{
		glm::vec2 *startPoint = pathPlanner.GetStartPoint();
		glm::vec2 *startDirection = pathPlanner.GetStartDirection();
		float angle = glm::degrees(GeometryHelper::GetAngleBetweenVectors(glm::vec2(1, 0), *startDirection));

		ui.pathStartPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(startPoint->x).arg(startPoint->y).arg(angle));
	}

	if (pathPlanner.GetEndPoint() != NULL)
	{
		glm::vec2 *endPoint = pathPlanner.GetEndPoint();
		glm::vec2 *endDirection = pathPlanner.GetEndDirection();
		float angle = glm::degrees(GeometryHelper::GetAngleBetweenVectors(glm::vec2(1, 0), *endDirection));

		ui.pathEndPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(endPoint->x).arg(endPoint->y).arg(angle));
	}

	if (pathPlanner.GetStartParkingSpace() != NULL)
	{
		ParkingSpace *startParkingSpace = pathPlanner.GetStartParkingSpace();

		ui.pathStartPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(startParkingSpace->GetPosition().x).arg(startParkingSpace->GetPosition().y).arg(startParkingSpace->GetRotation()));
	}

	if (pathPlanner.GetEndParkingSpace() != NULL)
	{
		ParkingSpace *endParkingSpace = pathPlanner.GetEndParkingSpace();

		ui.pathEndPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(endParkingSpace->GetPosition().x).arg(endParkingSpace->GetPosition().y).arg(endParkingSpace->GetRotation()));
	}

	if (pathPlanner.GetStartPoint() == NULL && pathPlanner.GetStartParkingSpace() == NULL)
	{
		ui.pathStartPositionLabel->setText(QString("NOT SET"));
	}

	if (pathPlanner.GetEndPoint() == NULL && pathPlanner.GetEndParkingSpace() == NULL)
	{
		ui.pathEndPositionLabel->setText(QString("NOT SET"));
	}

	if (pathPlanner.GetFinalPath() != NULL && pathPlanner.GetFinalPath()->GetElements().size() > 0)
	{
		int pathElementsCount = pathPlanner.GetFinalPath()->GetElements().size();

		ui.isPathSet->hide();
		ui.pathElementsList->clear();
		for (int i = 0; i < pathElementsCount; i++)
			ui.pathElementsList->addItem(new QListWidgetItem(QString("Path Element %1").arg(i + 1)));
		ui.pathElementsList->show();
	}
	else
	{
		ui.isPathSet->show();
		ui.pathElementsList->hide();
	}
}

void ParkingSimulator::setLanguage()
{
	Language *language = Language::getInstance();
	std::map<std::string, std::string> dictionary = language->GetDictionary();

	ui.tabWidget->setTabText(0, QString::fromStdString(dictionary["Tab_Start"]));
	ui.tabWidget->setTabText(1, QString::fromStdString(dictionary["Tab_MapEditor"]));
	ui.tabWidget->setTabText(2, QString::fromStdString(dictionary["Tab_PathPlanner"]));
	ui.tabWidget->setTabText(3, QString::fromStdString(dictionary["Tab_Visualisation"]));
	ui.tabWidget->setTabText(4, QString::fromStdString(dictionary["Tab_Settings"]));
	ui.btnNewMap->setToolTip(QString::fromStdString(dictionary["MapEditor_NewMap"]));
	ui.btnLoadMap->setToolTip(QString::fromStdString(dictionary["MapEditor_OpenMap"]));
	ui.btnSaveMap->setToolTip(QString::fromStdString(dictionary["MapEditor_SaveMap"]));
	ui.btnAddBuilding->setToolTip(QString::fromStdString(dictionary["MapEditor_AddBuilding"]));
	ui.btnAddDecoration->setToolTip(QString::fromStdString(dictionary["MapEditor_AddDecoration"]));
	ui.btnAddParkPlace->setToolTip(QString::fromStdString(dictionary["MapEditor_AddParkingPlace"]));
	ui.btnAddCar->setToolTip(QString::fromStdString(dictionary["MapEditor_AddVehicle"]));
	ui.btnAddTerrain->setToolTip(QString::fromStdString(dictionary["MapEditor_AddTerrain"]));
	ui.btnClearMap->setText(QString::fromStdString(dictionary["MapEditor_ClearMap"]));

	ui.mapElementPropertiesTitleLabel->setText(QString::fromStdString(dictionary["MapEditor_MapElementProperties"]));
	ui.mapElementPropertiesNameLabel->setText(QString::fromStdString(dictionary["Common_Name"]));
	ui.mapElementPropertiesWidthLabel->setText(QString::fromStdString(dictionary["Common_Width"]));
	ui.mapElementPropertiesHeightLabel->setText(QString::fromStdString(dictionary["Common_Height"]));
	ui.mapElementPropertiesPositionLabel->setText(QString::fromStdString(dictionary["Common_Position"]));
	ui.mapElementPropertiesRotationLabel->setText(QString::fromStdString(dictionary["Common_Rotation"]));
	ui.btnMapElementSaveProperties->setText(QString::fromStdString(dictionary["Common_Save"]));
	ui.btnMapElementRemove->setText(QString::fromStdString(dictionary["Common_Remove"]));

	ui.btnNewSimulation->setToolTip(QString::fromStdString(dictionary["PathPlanner_NewSimulation"]));
	ui.btnOpenSimulation->setToolTip(QString::fromStdString(dictionary["PathPlanner_OpenSimulation"]));
	ui.btnSaveSimulation->setToolTip(QString::fromStdString(dictionary["PathPlanner_SaveSimulation"]));
	ui.btnSetMap->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetMap"]));
	ui.btnSetVehicle->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetVehicle"]));
	ui.btnSetStart->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetStart"]));
	ui.btnSetEnd->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetEnd"]));
	ui.btnFindPath->setToolTip(QString::fromStdString(dictionary["PathPlanner_FindPath"]));
	ui.btnShowExpandedObstacles->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowExpandedObstacles"]));
	ui.btnShowFinalPath->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowFinalPath"]));
	ui.btnShowFullVoronoiVisibilityGraph->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowFullVoronoiVisibilityGraph"]));
	ui.btnShowParkingPath->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowParkingPath"]));
	ui.btnShowPolylinePath->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowPolylinePath"]));
	ui.btnShowVoronoiGraph->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowVoronoiGraph"]));
	ui.mapLabel->setText(QString::fromStdString(dictionary["PathPlanner_Map"]));
	ui.isMapSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.vehicleLabel->setText(QString::fromStdString(dictionary["PathPlanner_Vehicle"]));
	ui.isVehicleSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.pathLabel->setText(QString::fromStdString(dictionary["PathPlanner_Path"]));
	ui.isPathSet->setText(QString::fromStdString(dictionary["PathPlanner_NotCreated"]));

	ui.mapWidthLabel_2->setText(QString::fromStdString(dictionary["Common_Width"]));
	ui.mapHeightLabel_2->setText(QString::fromStdString(dictionary["Common_Height"]));
	ui.mapElementsCountLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_Map"]));
	ui.vehicleNameLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_MapElementsCount"]));
	ui.vehicleWheelbaseLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_VehicleWheelbase"]));
	ui.vehicleTrackLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_VehicleTrack"]));
	ui.vehicleMaxAngleLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_VehicleMaxAngle"]));
	ui.pathStartPositionLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_StartPosition"]));
	ui.pathEndPositionLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_EndPosition"]));
	ui.PathElementPropertiesTitleLabel->setText(QString::fromStdString(dictionary["PathPlanner_PathElementProperties"]));
	ui.pathElementPropertiesTypeLabel->setText(QString::fromStdString(dictionary["Common_Type"]));
	ui.pathElementPropertiesFromLabel->setText(QString::fromStdString(dictionary["Common_From"]));
	ui.pathElementPropertiesToLabel->setText(QString::fromStdString(dictionary["Common_To"]));
	ui.pathElementPropertiesLengthLabel->setText(QString::fromStdString(dictionary["Common_Length"]));

	ui.btnAddSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_AddSimulation"]));
	ui.btnRemoveSimulation->setToolTip(QString::fromStdString(dictionary["Visualization_RemoveSimulation"]));
	ui.btnInfoSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_InfoSimulation"]));
	ui.btnPlayPauseSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_PlayPause"]));
	ui.btnStopSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_Stop"]));

	ui.simulationDurationLabel->setText(QString::fromStdString(dictionary["Visualisation_SimulationTime"]));
	ui.showPathElements->setText(QString::fromStdString(dictionary["Visualisation_ShowPathElements"]));

	updateMapElementsTree();
}

void ParkingSimulator::updateTimerCall()
{
	if (mapEditor.GetMapElementsChanged())
	{
		updateMapElementsTree();
		mapEditor.SetMapElementsChanged(false);
	}

	if (mapEditor.GetSelectedElement() != NULL && mapEditor.GetMapElementsPropertiesChanged())
	{
		MapElement *mapElement = mapEditor.GetSelectedElement();

		ui.mapElementName->setText(QString::fromStdString(mapElement->GetName()));
		ui.mapElementWidth->setValue(mapElement->GetSize().x);
		ui.mapElementHeight->setValue(mapElement->GetSize().y);
		ui.mapElementPositionX->setValue(mapElement->GetPosition().x);
		ui.mapElementPositionY->setValue(mapElement->GetPosition().y);
		ui.mapElementRotation->setValue(glm::degrees(mapElement->GetRotation()));

		mapEditor.SetMapElementsPropertiesChanged(false);
	}

	if (pathPlanner.GetSelectedPathElement() != NULL && pathPlanner.GetPathElementPropertiesChanged()) //TODO:
	{
		PathElement *pathElement = pathPlanner.GetSelectedPathElement();
		if (dynamic_cast<Line*>(pathElement) != NULL)
		{
			Line *line = dynamic_cast<Line*>(pathElement);

			ui.pathElementType->setText(QString("Line"));
			ui.pathElementFrom->setText(QString("X: %1, Y: %2").arg(pathElement->GetFirstPoint().x).arg(pathElement->GetFirstPoint().y));
			ui.pathElementTo->setText(QString("X: %1, Y: %2").arg(pathElement->GetLastPoint().x).arg(pathElement->GetLastPoint().y));
		}
		else if (dynamic_cast<Circle*>(pathElement) != NULL)
		{
			Circle *circle = dynamic_cast<Circle*>(pathElement);

			ui.pathElementType->setText(QString("Circle"));
			ui.pathElementFrom->setText(QString("%1 deg").arg(glm::degrees(circle->GetAngleFrom())));
			ui.pathElementTo->setText(QString("%1 deg").arg(glm::degrees(circle->GetAngleTo())));
		}
		else if (dynamic_cast<BSpline*>(pathElement) != NULL)
		{
			BSpline *bSpline = dynamic_cast<BSpline*>(pathElement);

			ui.pathElementType->setText(QString("B-Spline"));
			ui.pathElementFrom->setText(QString("X: %1, Y: %2").arg(pathElement->GetFirstPoint().x).arg(pathElement->GetFirstPoint().y));
			ui.pathElementTo->setText(QString("X: %1, Y: %2").arg(pathElement->GetLastPoint().x).arg(pathElement->GetLastPoint().y));
		}
		ui.pathElementLength->setText(QString("%1 m").arg(pathElement->GetLength()));
		ui.pathElementProperties->show();

		pathPlanner.SetPathElementPropertiesChanged(false);
	}

	if (mapEditor.GetSelectedElementChanged())
	{
		if (mapEditor.GetSelectedElement() == NULL)
		{
			ui.mapElementProperties->hide();
		}
		else
		{
			ui.mapElementProperties->show();

			if (dynamic_cast<Obstacle*>(mapEditor.GetSelectedElement()) != NULL)
			{
				int i = 0;
				int buildingIndex = 0;
				int decorationIndex = 0;
				while (mapEditor.GetMap()->GetObstacles()[i] != mapEditor.GetSelectedElement())
				{
					if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Building)
						buildingIndex++;
					if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Decoration)
						decorationIndex++;
					i++;
				}

				QModelIndex root = ui.treeMapElements->indexAt(QPoint(0, 0));
				QModelIndex buildings = root.child(0, 0);
				QModelIndex decorations = root.child(1, 0);

				QModelIndex newIndex;
				if (dynamic_cast<Obstacle*>(mapEditor.GetSelectedElement())->GetType() == ObstacleType::Building)
				{
					newIndex = buildings.child(buildingIndex, 0);
				}
				else if (dynamic_cast<Obstacle*>(mapEditor.GetSelectedElement())->GetType() == ObstacleType::Decoration)
				{
					newIndex = decorations.child(decorationIndex, 0);
				}
				ui.treeMapElements->setCurrentIndex(newIndex);
			}
			else if (dynamic_cast<Vehicle*>(mapEditor.GetSelectedElement()) != NULL)
			{
				int index = 0;
				while (mapEditor.GetMap()->GetVehicles()[index] != mapEditor.GetSelectedElement())
					index++;
				QModelIndex root = ui.treeMapElements->indexAt(QPoint(0, 0));
				QModelIndex parkingPlaces = root.child(2, 0);
				QModelIndex newIndex = parkingPlaces.child(index, 0);
				ui.treeMapElements->setCurrentIndex(newIndex);
			}
			else if (dynamic_cast<ParkingSpace*>(mapEditor.GetSelectedElement()) != NULL)
			{
				int index = 0;
				while (mapEditor.GetMap()->GetParkingSpaces()[index] != mapEditor.GetSelectedElement())
					index++;
				QModelIndex root = ui.treeMapElements->indexAt(QPoint(0, 0));
				QModelIndex parkingPlaces = root.child(3, 0);
				QModelIndex newIndex = parkingPlaces.child(index, 0);
				ui.treeMapElements->setCurrentIndex(newIndex);
			}
		}
		mapEditor.SetSelectedElementChanged(false);
	}

	if (pathPlanner.GetStartPositionChanged() || pathPlanner.GetEndPositionChanged())
	{
		setPathProperties();
		clearPathPlannerButtons();
		pathPlanner.SetStartPositionChanged(false);
		pathPlanner.SetEndPositionChanged(false);
	}

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

void ParkingSimulator::loadSettings()
{
	Settings::getInstance()->LoadSettings();
}

void ParkingSimulator::saveSettings()
{
	Settings::getInstance()->SetLanguage(ui.languagesList->currentText().toStdString());
	Settings::getInstance()->SaveSettings();

	Language::getInstance()->LoadLanguage(Settings::getInstance()->GetLanguage());

	setLanguage();
}

void ParkingSimulator::goToMapEditor()
{
	ui.tabWidget->setCurrentIndex(1);
}

void ParkingSimulator::goToPathPlanner()
{
	ui.tabWidget->setCurrentIndex(2);
}

void ParkingSimulator::goToVisualisation()
{
	ui.tabWidget->setCurrentIndex(3);
}

void ParkingSimulator::goToSettings()
{
	ui.tabWidget->setCurrentIndex(4);
}

void ParkingSimulator::createMap()
{
	CreateMap createMapWindow;
	createMapWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
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

void ParkingSimulator::clearMap()
{
	mapEditor.SetSelectedElement(nullptr);
	while(mapEditor.GetMap()->GetMapElements().size() > 0)
		mapEditor.GetMap()->RemoveMapElement(mapEditor.GetMap()->GetMapElements()[0]);
	mapEditor.SetMapElementsChanged(true);
	mapEditor.SetSelectedElementChanged(true);
}

void ParkingSimulator::addBuilding()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace() || mapEditor.GetAddCar() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddBuilding())
	{
		AddMapElement addMapElementWindow(AddMapElementType::B);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddBuilding->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddBuilding(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
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

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddParkPlace() || mapEditor.GetAddCar() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddDecoration())
	{
		AddMapElement addMapElementWindow(AddMapElementType::D);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddDecoration->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddDecoration(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
	}
	else
	{
		ui.btnAddDecoration->setStyleSheet("");
		mapEditor.SetAddDecoration(false);
	}
}

void ParkingSimulator::addCar()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddCar())
	{
		AddMapElement addMapElementWindow(AddMapElementType::C);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddCar->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddCar(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
	}
	else
	{
		ui.btnAddCar->setStyleSheet("");
		mapEditor.SetAddCar(false);
	}
}

void ParkingSimulator::addParkPlace()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddCar() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddParkPlace())
	{
		AddMapElement addMapElementWindow(AddMapElementType::P);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddParkPlace->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddParkPlace(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
	}
	else
	{
		ui.btnAddParkPlace->setStyleSheet("");
		mapEditor.SetAddParkPlace(false);
	}
}

void ParkingSimulator::addTerrain()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddCar() || mapEditor.GetAddParkPlace())
		clearMapEditorButtons();

	if (!mapEditor.GetAddTerrain())
	{
		AddMapElement addMapElementWindow(AddMapElementType::T);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddTerrain->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddTerrain(true);
			mapEditor.SetNewTerrain(addMapElementWindow.GetNewTerrain());
		}
	}
	else
	{
		ui.btnAddTerrain->setStyleSheet("");
		mapEditor.SetAddTerrain(false);
	}
}

void ParkingSimulator::mapElementRemove()
{
	mapEditor.GetMap()->RemoveMapElement(mapEditor.GetSelectedElement());
	mapEditor.SetSelectedElement(nullptr);
	mapEditor.SetMapElementsChanged(true);

	ui.mapElementProperties->hide();
}

void ParkingSimulator::mapElementSaveProperties()
{
	MapElement *selectedElement = mapEditor.GetSelectedElement();

	string oldName = selectedElement->GetName();
	glm::vec2 oldSize = selectedElement->GetSize();
	glm::vec2 oldPosition = selectedElement->GetPosition();
	double oldRotation = selectedElement->GetRotation();

	selectedElement->SetName(ui.mapElementName->toPlainText().toStdString());
	selectedElement->SetSize(glm::vec2(ui.mapElementWidth->value(), ui.mapElementHeight->value()));
	selectedElement->SetPosition(glm::vec2(ui.mapElementPositionX->value(), ui.mapElementPositionY->value()));
	selectedElement->SetRotation(glm::radians(ui.mapElementRotation->value()));

	if (!mapEditor.IsMapElementAdmissible(selectedElement))
	{
		selectedElement->SetName(oldName);
		selectedElement->SetSize(oldSize);
		selectedElement->SetPosition(oldPosition);
		selectedElement->SetRotation(oldRotation);

		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_MapElementNotAdmissible_Title"], Language::getInstance()->GetDictionary()["WarningError_MapElementNotAdmissible_Content"], MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
	}
}

void ParkingSimulator::updateMapElementsTree()
{
	auto mapElementsTree = *mapEditor.GetMapElementsTreeItems();
	auto mapElementsTreeExpanded = mapEditor.GetMapElementsTreeItemsExpanded();
	
	ui.treeMapElements->clear();
	ui.treeMapElements->insertTopLevelItems(0, mapElementsTree);

	for (int i = 0; i < 5; i++)
	{
		if (mapElementsTreeExpanded[i])
			ui.treeMapElements->expandItem(mapElementsTree[i]);
	}
}

void ParkingSimulator::treeMapElementsSelectionChanged()
{
	auto currentItem = ui.treeMapElements->currentItem();
	auto currentIndex = ui.treeMapElements->currentIndex();

	auto b = currentIndex.isValid();

	if (currentItem->parent() == NULL || mapEditor.SelectedElementChangedOnMap())
	{
		mapEditor.SetSelectedElementChangedOnMap(false);
		return;
	}

	QString parentItemTitle = currentItem->parent()->text(0);
	if (parentItemTitle == "Buildings")
	{
		int i = 0;
		int buildingIndex = 0;

		if (mapEditor.GetMap()->GetObstacles().size() > 0)
		{
			do
			{
				if (i >= mapEditor.GetMap()->GetObstacles().size()) return;
				if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Building)
				{
					if (buildingIndex < currentIndex.row())
					{
						buildingIndex++;
						i++;
					}
					else
						break;
				}
				else
				{
					i++;
				}
			} while (buildingIndex <= currentIndex.row());

			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetObstacles()[i]);
		}
		else
			mapEditor.SetSelectedElement(nullptr);
	}
	else if (parentItemTitle == "Decorations")
	{
		int i = 0;
		int decorationIndex = 0;

		if (mapEditor.GetMap()->GetObstacles().size() > 0)
		{
			do
			{
				if (i >= mapEditor.GetMap()->GetObstacles().size()) return;
				if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Decoration)
				{
					if (decorationIndex < currentIndex.row())
					{
						decorationIndex++;
						i++;
					}
					else
						break;
				}
				else
				{
					i++;
				}
			} while (decorationIndex <= currentIndex.row());

			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetObstacles()[i]);
		}
		else
			mapEditor.SetSelectedElement(nullptr);
	}
	else if (parentItemTitle == "Vehicles")
	{
		if (currentIndex.row() < mapEditor.GetMap()->GetVehicles().size())
			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetVehicles()[currentIndex.row()]);
	}
	else if (parentItemTitle == "Park places")
	{
		if (currentIndex.row() < mapEditor.GetMap()->GetParkingSpaces().size())
			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetParkingSpaces()[currentIndex.row()]);
	}
	else
		mapEditor.SetSelectedElement(nullptr);
}

void ParkingSimulator::clearMapEditorButtons()
{
	mapEditor.SetAddBuilding(false);
	mapEditor.SetAddDecoration(false);
	mapEditor.SetAddParkPlace(false);
	mapEditor.SetAddCar(false);
	mapEditor.SetAddTerrain(false);

	clearMapEditorButtonsStyle();
}

void ParkingSimulator::clearMapEditorButtonsStyle()
{
	if (!mapEditor.GetAddBuilding())
		ui.btnAddBuilding->setStyleSheet("");
	if (!mapEditor.GetAddDecoration())
		ui.btnAddDecoration->setStyleSheet("");
	if (!mapEditor.GetAddParkPlace())
		ui.btnAddParkPlace->setStyleSheet("");
	if (!mapEditor.GetAddCar())
		ui.btnAddCar->setStyleSheet("");
	if (!mapEditor.GetAddTerrain())
		ui.btnAddTerrain->setStyleSheet("");
}

#pragma endregion

#pragma region Path planner.

void ParkingSimulator::newSimulation()
{
	pathPlanner.NewSimulation();
	setMapProperties();
	setVehicleProperties();
	setPathProperties();
	ui.pathElementProperties->hide();
}

void ParkingSimulator::openSimulation()
{
	QString filter = tr("Simulation files (*.simulation)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open simulation", QString(), tr("All files (*.*);;Simulation files (*.simulation)"), &filter);

	if (filePath != "")
	{
		pathPlanner.OpenSimulation(filePath.toStdString());
		setMapProperties();
		setVehicleProperties();
		setPathProperties();
		ui.pathElementProperties->hide();
	}
}

void ParkingSimulator::saveSimulation()
{
	QString filter = tr("Simulation files (*.simulation)");
	QString filePath = QFileDialog::getSaveFileName(this, "Save simulation", QString(), tr("All files (*.*);;Simulation files (*.simulation)"), &filter);

	if (filePath != "")
		pathPlanner.SaveSimulation(filePath.toStdString());
}

void ParkingSimulator::setMap()
{
	SelectMap selectMapWindow(&mapEditor);
	selectMapWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	if (selectMapWindow.exec())
	{
		pathPlanner.SetMap(selectMapWindow.GetMap());
		setMapProperties();
	}
}

void ParkingSimulator::setVehicle()
{
	SelectVehicle selectVehicleWindow;
	selectVehicleWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	if (selectVehicleWindow.exec())
	{
		pathPlanner.SetVehicle(selectVehicleWindow.GetVehicle());
		pathPlanner.SetVehicleStart(new Vehicle(*selectVehicleWindow.GetVehicle()));
		pathPlanner.SetVehicleEnd(new Vehicle(*selectVehicleWindow.GetVehicle()));
		setVehicleProperties();
	}
}

void ParkingSimulator::setStart()
{
	if (pathPlanner.GetSetEndPosition())
		clearPathPlannerButtons();

	if (pathPlanner.GetSetStartPosition())
	{
		pathPlanner.SetSetStartPosition(false);
		ui.btnSetStart->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetSetStartPosition(true);
		ui.btnSetStart->setStyleSheet("background-color: #d86a39;");
	}
}

void ParkingSimulator::setEnd()
{
	if (pathPlanner.GetSetStartPosition())
		clearPathPlannerButtons();

	if (pathPlanner.GetSetEndPosition())
	{
		pathPlanner.SetSetEndPosition(false);
		ui.btnSetEnd->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetSetEndPosition(true);
		ui.btnSetEnd->setStyleSheet("background-color: #d86a39;");
	}
}

void ParkingSimulator::showVoronoiGraph()
{
	if (pathPlanner.GetShowVoronoiGraph())
	{
		pathPlanner.SetShowVoronoiGraph(false);
		ui.btnShowVoronoiGraph->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowVoronoiGraph(true);
		ui.btnShowVoronoiGraph->setStyleSheet("border: 3px solid #d86a39;");
	}
}

void ParkingSimulator::showFullVoronoiVisibilityGraph()
{
	if (pathPlanner.GetShowFullVoronoiVisibilityGraph())
	{
		pathPlanner.SetShowFullVoronoiVisibilityGraph(false);
		ui.btnShowFullVoronoiVisibilityGraph->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowFullVoronoiVisibilityGraph(true);
		ui.btnShowFullVoronoiVisibilityGraph->setStyleSheet("border: 3px solid #d86a39;");
	}
}

void ParkingSimulator::showPolylinePath()
{
	if (pathPlanner.GetShowPolylinePath())
	{
		pathPlanner.SetShowPolylinePath(false);
		ui.btnShowPolylinePath->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowPolylinePath(true);
		ui.btnShowPolylinePath->setStyleSheet("border: 3px solid #d86a39;");
	}
}

void ParkingSimulator::showParkingPath()
{
	if (pathPlanner.GetShowParkingPath())
	{
		pathPlanner.SetShowFinalPath(false);
		ui.btnShowParkingPath->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowParkingPath(true);
		ui.btnShowParkingPath->setStyleSheet("border: 3px solid #d86a39;");
	}
}

void ParkingSimulator::showFinalPath()
{
	if (pathPlanner.GetShowFinalPath())
	{
		pathPlanner.SetShowFinalPath(false);
		ui.btnShowFinalPath->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowFinalPath(true);
		ui.btnShowFinalPath->setStyleSheet("border: 3px solid #d86a39;");
	}
}

void ParkingSimulator::showExpandedObstacles()
{
	if (pathPlanner.GetShowExpandedObstacles())
	{
		pathPlanner.SetShowExpandedObstacles(false);
		ui.btnShowExpandedObstacles->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowExpandedObstacles(true);
		ui.btnShowExpandedObstacles->setStyleSheet("border: 3px solid #d86a39;");
	}
}

void ParkingSimulator::findPath()
{
	FindPath findPathWindow;
	findPathWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	if (pathPlanner.GetMap() == NULL || pathPlanner.GetVehicle() == NULL 
		|| (pathPlanner.GetStartPoint() != NULL && pathPlanner.GetStartDirection() != NULL && pathPlanner.GetStartParkingSpace() != NULL)
		|| (pathPlanner.GetEndPoint() != NULL && pathPlanner.GetEndDirection() != NULL && pathPlanner.GetEndParkingSpace() != NULL))
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_FindPathNoParameters_Title"], Language::getInstance()->GetDictionary()["WarningError_FindPathNoParameters_Content"], MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
		return;
	}

	if (findPathWindow.exec())
	{
		pathPlanner.SetSelectedPathElement(nullptr);
		pathPlanner.SetExpandSizePercent(findPathWindow.GetExpandSizePercent());
		pathPlanner.SetCollisionDetectionDensity(findPathWindow.GetCollisionDetectionDensity());
		pathPlanner.SetAlgorithm(findPathWindow.GetAlgorithm());
		pathPlanner.SetUseAdmissibleArcsOnly(findPathWindow.UseOnlyAdmissibleArcs());

		int error;
		pathPlanner.FindPath(&error);

		if (pathPlanner.GetFinalPath() == NULL)
		{
			WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_PathNotFound_Title"], Language::getInstance()->GetDictionary()["WarningError_PathNotFound_Content"], MessageType::Error);
			warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
			warningWindow.exec();
		}

		setPathProperties();
	}
}

void ParkingSimulator::clearPathPlannerButtons()
{
	pathPlanner.SetSetStartPosition(false);
	pathPlanner.SetSetEndPosition(false);

	clearPathPlannerButtonsStyle();
}

void ParkingSimulator::clearPathPlannerButtonsStyle()
{
	if (!pathPlanner.GetSetStartPosition())
		ui.btnSetStart->setStyleSheet("");
	if (!pathPlanner.GetSetEndPosition())
		ui.btnSetEnd->setStyleSheet("");
}

void ParkingSimulator::pathElementSelectionChange()
{
	int selectedIndex = ui.pathElementsList->currentRow();
	if (selectedIndex > 0 && selectedIndex < pathPlanner.GetFinalPath()->GetElements().size())
		pathPlanner.SetSelectedPathElement(pathPlanner.GetFinalPath()->GetElements()[selectedIndex]);
}

#pragma endregion

#pragma region Visualisation.

void ParkingSimulator::addSimulation()
{
	AddSimulation addSimulationWindow(&pathPlanner);
	addSimulationWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	if (addSimulationWindow.exec())
	{
		Simulation *simulation = addSimulationWindow.GetSimulation();

		visualisation.AddSimulation(simulation);
		visualisation.SetCurrentSimulation(simulation);
		ui.listSimulations->addItem(new QListWidgetItem(QString::fromStdString(simulation->GetName())));
		ui.listSimulations->setCurrentRow(visualisation.GetSimulations().size() - 1);
	}
}

void ParkingSimulator::removeSimulation()
{
	int selectedIndex = ui.listSimulations->currentRow();
	if (selectedIndex >= 0 && selectedIndex < visualisation.GetSimulations().size())
	{
		Simulation *selectedSimulation = visualisation.GetSimulations()[selectedIndex];
		visualisation.RemoveSimulation(selectedSimulation);
		
		ui.listSimulations->clear();
		std::vector<Simulation*> simulations = visualisation.GetSimulations();
		for (int i = 0; i < simulations.size(); i++)
			ui.listSimulations->addItem(new QListWidgetItem(QString::fromStdString(simulations[i]->GetName())));
		if (simulations.size() > 0)
		{
			if (selectedIndex < simulations.size())
				ui.listSimulations->setCurrentRow(selectedIndex);
			else
				ui.listSimulations->setCurrentRow(selectedIndex - 1);
		}
	}
}

void ParkingSimulator::infoSimulation()
{
	int selectedIndex = ui.listSimulations->currentRow();
	if (selectedIndex >= 0 && selectedIndex < visualisation.GetSimulations().size())
	{
		Simulation *selectedSimulation = visualisation.GetSimulations()[selectedIndex];
		SimulationInfo simulationInfoWindow(selectedSimulation);
		simulationInfoWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		simulationInfoWindow.exec();
	}
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
			simulation->SetSimulationTime(ui.simulationLength->value());
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
	ui.glVisualisation->show();
	ui.glVisualisation3D->hide();
}

void ParkingSimulator::enableVisualisation3D()
{
	visualisation.SetVisualisation2D(false);
	visualisation.SetVisualisation3D(true);
	ui.btnVisualisation2D->setStyleSheet("");
	ui.btnVisualisation3D->setStyleSheet("border: 3px solid #d86a39;");
	ui.glVisualisation->hide();
	ui.glVisualisation3D->show();
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

void ParkingSimulator::simulationSelectionChange()
{
	int selectedIndex = ui.listSimulations->currentRow();
	if (selectedIndex >= 0 && selectedIndex < visualisation.GetSimulations().size())
		visualisation.SetCurrentSimulation(visualisation.GetSimulations()[selectedIndex]);
	else
		visualisation.SetCurrentSimulation(nullptr);
}

void ParkingSimulator::initializeLanguages()
{
	path p(".");
	std::vector<directory_entry> v;

	if (is_directory(p))
	{
		copy(directory_iterator(p), directory_iterator(), back_inserter(v));
		for (std::vector<directory_entry>::const_iterator it = v.begin(); it != v.end(); ++it)
		{
			std::string path = (*it).path().string();
			if (path.find("lang") != std::string::npos)
			{
				int start = path.find("lang.") + 5;
				int end = path.find(".ini");
				ui.languagesList->addItem(QString::fromStdString(path.substr(start, end-start)));
			}
		}
	}
}

#pragma endregion