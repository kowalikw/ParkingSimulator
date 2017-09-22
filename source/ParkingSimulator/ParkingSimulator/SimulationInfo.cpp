#include "SimulationInfo.h"

SimulationInfo::SimulationInfo(Simulation *simulation, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnClose, SIGNAL(released()), this, SLOT(reject()));

	// TODO:
}

SimulationInfo::~SimulationInfo()
{
}
