#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "glew.h"
#include "ui_ParkingSimulator.h"
#include "CreateMap.h"
#include "MapEditor.h"
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
	MapEditor mapEditor;
public slots:
	void renderTimerCall();

	void createMap();
	void saveMap();
	void loadMap();
	void addBuilding();
	void addDecoration();
	void addRoad();
	void addParkPlace();
	void clearAddButtons();
	void clearAddButtonsStyle();
};
