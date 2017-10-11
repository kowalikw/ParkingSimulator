#include "SimulationInfo.h"

SimulationInfo::SimulationInfo(Simulation *simulation, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnClose, SIGNAL(released()), this, SLOT(reject()));

	QString collisionDetectionQuality;
	double collisionDetectionDensity = simulation->GetCollisionDetectionDensity();
	if (collisionDetectionDensity == 0)
		collisionDetectionQuality = QString("Disabled");
	else if (collisionDetectionDensity > 0 && collisionDetectionDensity <= 10)
		collisionDetectionQuality = QString("Very Low");
	else if (collisionDetectionDensity > 10 && collisionDetectionDensity <= 25)
		collisionDetectionQuality = QString("Low");
	else if (collisionDetectionDensity > 25 && collisionDetectionDensity <= 50)
		collisionDetectionQuality = QString("Medium");
	else if (collisionDetectionDensity > 50 && collisionDetectionDensity <= 75)
		collisionDetectionQuality = QString("Good");
	else if (collisionDetectionDensity > 75 && collisionDetectionDensity < 100)
		collisionDetectionQuality = QString("Very good");

	QString pathPlanningAlgorithm;
	if (simulation->GetPathPlanningAlgorithm() == PathPlanningAlgorithm::Spline)
		pathPlanningAlgorithm = QString("Spline");
	else if (simulation->GetPathPlanningAlgorithm() == PathPlanningAlgorithm::Arcs)
		pathPlanningAlgorithm = QString("Arcs");

	ui.simulationName->setText(QString::fromStdString(simulation->GetName()));
	ui.simulationVehicleName->setText(QString::fromStdString(simulation->GetVehicle()->GetName()));
	ui.simulationPathElements->setText(QString("%1").arg(simulation->GetFinalPath()->GetElements().size()));
	ui.simulationPathLength->setText(QString("%1").arg(simulation->GetFinalPath()->GetLength()));
	ui.simulationExpandSizePercent->setText(QString("%1").arg(simulation->GetExpandSize()));
	ui.simulationPathPlanningAlgorithm->setText(pathPlanningAlgorithm);
	ui.simulationUseAdmissibleArcsOnly->setText(simulation->GetUseAdmissibleArcsOnly() ? "True" : "False");
	ui.simulationCollisionDetectionQuality->setText(collisionDetectionQuality);
}

SimulationInfo::~SimulationInfo()
{
}