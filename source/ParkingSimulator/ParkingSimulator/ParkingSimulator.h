#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "glew.h"
#include "ui_ParkingSimulator.h"
#include "CreateMap.h"
#include "WarningErrorMsg.h"
#include "AddMapElement.h"
#include "MapEditor.h"
#include "Visualisation.h"
#include "MapEditorGLHost.h"
#include "Settings.h";
#include "SelectMap.h"
#include "SelectVehicle.h"
#include "FindPath.h"
#include "AddSimulation.h"

using namespace Ui;

class ParkingSimulator : public QMainWindow
{
	Q_OBJECT

public:
	ParkingSimulator(QWidget *parent = Q_NULLPTR);

private:
	ParkingSimulatorClass ui;
	QTimer *renderTimer;
	QTimer *updateTimer;
	MapEditor mapEditor;
	PathPlanner pathPlanner;
	Visualisation visualisation;

	bool updateSimulationProgressBarEnable = false;
public slots:
	void renderTimerCall();
	void updateTimerCall();

	void loadSettings();
	void saveSettings();

	void createMap();
	void saveMap();
	void loadMap();
	void clearMap();
	void addBuilding();
	void addDecoration();
	void addRoad();
	void addParkPlace();
	void mapElementRemove();
	void mapElementSaveProperties();

	void updateMapElementsTree();
	void treeMapElementsSelectionChanged();
	void clearMapEditorButtons();
	void clearMapEditorButtonsStyle();

	//path planner
	void newSimulation();
	void openSimulation();
	void saveSimulation();
	void setMap();
	void setVehicle();
	void setStart();
	void setEnd();
	void showVoronoiGraph();
	void showFullVoronoiVisibilityGraph();
	void showPolylinePath();
	void showFinalPath();
	void showExpandedObstacles();
	void findPath();
	void clearPathPlannerButtons();
	void clearPathPlannerButtonsStyle();

	// visualisation
	void addSimulation();
	void removeSimulation();
	void infoSimulation();
	void playPauseSimulation();
	void stopSimulation();
	void enableVisualisation2D();
	void enableVisualisation3D();
	void showSimulationPath(int checked);
	void simulationProgressBarChange(int time);
	void simulationProgressBarPressed();
	void simulationProgressBarReleased();
};
