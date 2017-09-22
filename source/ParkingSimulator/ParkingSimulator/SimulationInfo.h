#pragma once

#include <QDialog>
#include "ui_SimulationInfo.h"
#include "Simulation.h"

using namespace Ui;

class SimulationInfo : public QDialog
{
	Q_OBJECT

public:
	SimulationInfo(Simulation *simulation, QWidget *parent = Q_NULLPTR);
	~SimulationInfo();

private:
	SimulationInfoClass ui;
};
