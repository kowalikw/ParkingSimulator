#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ParkingSimulator.h"

class ParkingSimulator : public QMainWindow
{
	Q_OBJECT

public:
	ParkingSimulator(QWidget *parent = Q_NULLPTR);

private:
	Ui::ParkingSimulatorClass ui;
};
