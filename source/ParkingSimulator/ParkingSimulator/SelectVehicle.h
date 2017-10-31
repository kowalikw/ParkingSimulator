#pragma once

#include <QDialog>
#include "ui_SelectVehicle.h"
#include "Vehicle.h"
#include "Language.h"
#include "VehicleEditor.h"
#include <QFileDialog>
#include "WarningErrorMsg.h"

using namespace Ui;

class SelectVehicle : public QDialog
{
	Q_OBJECT

public:
	SelectVehicle(VehicleEditor *vehicleEditor, QWidget *parent = Q_NULLPTR);
	~SelectVehicle();
	Vehicle * GetVehicle();

private:
	SelectVehicleClass ui;

	Vehicle *vehicle;
	VehicleEditor *vehicleEditor;

public slots:
	void loadVehicleFromVehicleEditor();
	void loadVehicleFromFile();
};
