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
#include "SimulationInfo.h"
#include "Language.h"
#include <map>

using namespace Ui;

class ParkingSimulator : public QMainWindow
{
	Q_OBJECT

public:
	ParkingSimulator(QWidget *parent = Q_NULLPTR);

private:
	ParkingSimulatorClass ui;
	QTimer *renderTimer;
	QTimer *render3DTimer;
	QTimer *updateTimer;
	MapEditor mapEditor;
	PathPlanner pathPlanner;
	Visualisation visualisation;

	bool updateSimulationProgressBarEnable = false;

	void setMapProperties();
	void setVehicleProperties();
	void setPathProperties();

	void setLanguage();
public slots:
	void renderTimerCall();
	void render3DTimerCall();
	void updateTimerCall();

	// start
	void loadSettings();
	void saveSettings();
	void goToMapEditor();
	void goToPathPlanner();
	void goToVisualisation();
	void goToSettings();

	// map editor
	void createMap();
	void saveMap();
	void loadMap();
	void clearMap();
	void addBuilding();
	void addDecoration();
	void addCar();
	void addParkPlace();
	void addTerrain();
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
	void showParkingPath();
	void showFinalPath();
	void showExpandedObstacles();
	void findPath();
	void clearPathPlannerButtons();
	void clearPathPlannerButtonsStyle();
	void pathElementSelectionChange();

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
	void simulationSelectionChange();

	// settings
	void initializeLanguages();
};
