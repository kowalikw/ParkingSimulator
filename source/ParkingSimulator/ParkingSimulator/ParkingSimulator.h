#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "glew.h"
#include "ui_ParkingSimulator.h"
#include "CreateMap.h"
#include "MapEditor.h"
#include "Visualisation.h"
#include "MapEditorGLHost.h"

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
	Visualisation visualisation;
	QTreeWidgetItem *buildings, *decorations, *roads, *parkPlaces;

	bool updateSimulationProgressBarEnable = false;
public slots:
	void renderTimerCall();
	void updateTimerCall();

	void createMap();
	void saveMap();
	void loadMap();
	void addBuilding();
	void addDecoration();
	void addRoad();
	void addParkPlace();

	void updateMapElementsTree();
	void clearAddButtons();
	void clearAddButtonsStyle();

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
