#include "ParkingSimulator.h"

ParkingSimulator::ParkingSimulator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	renderTimer = new QTimer();
	renderTimer->setInterval(20);
	renderTimer->start();

	connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderTimerCall()));
}

void ParkingSimulator::renderTimerCall()
{
	ui.glPathPlanner->repaint();
}
