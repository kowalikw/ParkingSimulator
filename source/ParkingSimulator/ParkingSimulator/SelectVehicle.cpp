#include "SelectVehicle.h"

SelectVehicle::SelectVehicle(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnAccept, SIGNAL(released()), this, SLOT(selectVehicle()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));
}

SelectVehicle::~SelectVehicle()
{
}

Vehicle * SelectVehicle::GetVehicle()
{
	return this->vehicle;
}

void SelectVehicle::selectVehicle()
{
	this->vehicle = new Vehicle(100, 50);
	this->vehicle->SetName("Renault Scenic");

	accept();
}