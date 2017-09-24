#pragma once

#include <QDialog>
#include "ui_SelectVehicle.h"
#include "Vehicle.h"

using namespace Ui;

class SelectVehicle : public QDialog
{
	Q_OBJECT

public:
	SelectVehicle(QWidget *parent = Q_NULLPTR);
	~SelectVehicle();
	Vehicle * GetVehicle();

private:
	SelectVehicleClass ui;

	Vehicle *vehicle;

public slots:
	void selectVehicle();
};
