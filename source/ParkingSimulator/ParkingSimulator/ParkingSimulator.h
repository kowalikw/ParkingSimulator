#pragma once

#include <QTimer>
#include <map>
#include <QtWidgets/QMainWindow>
#include <QColorDialog>
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include <boost/filesystem.hpp>
#include <QThread>
#include <qmovie.h>
#include "glew.h"
#include "ui_ParkingSimulator.h"
#include "CreateMap.h"
#include "WarningErrorMsg.h"
#include "AddMapElement.h"
#include "MapEditor.h"
#include "Visualisation.h"
#include "MapEditorGLHost.h"
#include "Settings.h"
#include "SelectMap.h"
#include "SelectVehicle.h"
#include "FindPath.h"
#include "AddSimulation.h"
#include "SimulationInfo.h"
#include "Language.h"
#include "CommonHelper.h"

using namespace Ui;
using namespace boost::filesystem;

class FindPathThread : public QThread
{

public:
	FindPathThread() {}
	void SetPathPlanner(PathPlanner *pathPlanner)
	{
		this->planner = pathPlanner;
	}

	PathPlanner * GetPathPlanner()
	{
		return this->planner;
	}

protected:
	void run()
	{
		int error;
		planner->FindPath(&error);
		quit();
	}
private:
	PathPlanner *planner;
};

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
	VehicleEditor vehicleEditor;
	PathPlanner pathPlanner;
	Visualisation visualisation;
	FindPathThread findPathThread;

	bool updateSimulationProgressBarEnable = false;

public slots:
	void renderTimerCall();
	void render3DTimerCall();
	void updateTimerCall();

	// start
	void loadSettings();
	void saveSettings();
	void goToMapEditor();
	void goToVehicleEditor();
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

	//vehicle editor
	void newVehicle();
	void openVehicle();
	void saveVehicle();
	void loadVehicleModel();
	void loadVehicleLeftFrontWheelModel();
	void loadVehicleRightFrontWheelModel();
	void loadVehicleLeftRearWheelModel();
	void loadVehicleRightRearWheelModel();
	void enableVehicleFrontLeftWheelProperties();
	void enableVehicleFrontRightWheelProperties();
	void enableVehicleRearLeftWheelProperties();
	void enableVehicleRearRightWheelProperties();
	void updateVehicleProperties();
	void updateVehicleFrontLeftWheelProperties();
	void updateVehicleFrontRightWheelProperties();
	void updateVehicleRearLeftWheelProperties();
	void updateVehicleRearRightWheelProperties();
	void applyVehicleProperties();
	void vehicleFrontLeftWheelPropertiesChanged();
	void vehicleFrontRightWheelPropertiesChanged();
	void vehicleRearLeftWheelPropertiesChanged();
	void vehicleRearRightWheelPropertiesChanged();

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
	void enablePathPlannerButtons();
	void disablePathPlannerButtons();
	void cancelPathCalculation();
	void pathCalculationFinished();
	void setMapProperties();
	void setVehicleProperties();
	void setPathProperties();

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
	void simulationSelectionChange();

	// settings
	void setLanguage();
	void initializeLanguages();
	void changeColor(ColorContainer & pos);
};
