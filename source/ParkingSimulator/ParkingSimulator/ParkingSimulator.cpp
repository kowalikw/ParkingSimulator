#include "ParkingSimulator.h"

#include <fstream>

#include <iostream>

#include <QFileDialog>

ParkingSimulator::ParkingSimulator(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: #2a2a2a;");

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

	connect(ui.btnLoadSettings, SIGNAL(released()), this, SLOT(loadSettings()));
	connect(ui.btnSaveSettings, SIGNAL(released()), this, SLOT(saveSettings()));

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
	connect(ui.btnAddRoad, SIGNAL(released()), this, SLOT(addRoad()));	
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
			else if (dynamic_cast<Road*>(mapEditor.GetSelectedElement()) != NULL)
			{

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
	Settings::getInstance()->SaveSettings();
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

	if (mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace() || mapEditor.GetAddRoad())
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

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddParkPlace() || mapEditor.GetAddRoad())
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

void ParkingSimulator::addRoad()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace())
		clearMapEditorButtons();

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

		WarningErrorMsg warningWindow("An error occured!", "Map element after transformation is not admissible!", MessageType::Error);
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
	else if (parentItemTitle == "Roads")
	{
		//mapEditor.SetSelectedElement(mapEditor.GetMap()->GetMapElements()[index - 5]);
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
	mapEditor.SetAddRoad(false);

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
	if (!mapEditor.GetAddRoad())
		ui.btnAddRoad->setStyleSheet("");
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
		WarningErrorMsg warningWindow("An error occured!", "It was impossible to find a path, because not all required parameters were set.", MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
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
			WarningErrorMsg warningWindow("Path not found!", "A path between start and end position was not found!\nTry again with another parameters.", MessageType::Warning);
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

#pragma endregion