#pragma once

#include <QDialog>
#include "ui_AddSimulation.h"
#include "PathPlanner.h"
#include <QFileDialog>
#include "WarningErrorMsg.h"

using namespace Ui;

class AddSimulation : public QDialog
{
	Q_OBJECT

public:
	AddSimulation(PathPlanner *pathPlanner, QDialog *parent = Q_NULLPTR);
	~AddSimulation();

	Simulation * GetSimulation();

private:
	AddSimulationClass ui;

	Simulation *simulation;
	PathPlanner *pathPlanner;

public slots:
	void loadSimulationFromPathPlanner();
	void loadSimulationFromFile();
};
