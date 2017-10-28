#include "SimulationInfo.h"

SimulationInfo::SimulationInfo(Simulation *simulation, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnClose, SIGNAL(released()), this, SLOT(reject()));

	QString collisionDetectionQuality;
	double collisionDetectionDensity = simulation->GetCollisionDetectionDensity();
	if (collisionDetectionDensity == 0)
		collisionDetectionQuality = QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Disabled"]);
	else if (collisionDetectionDensity > 0 && collisionDetectionDensity <= 10)
		collisionDetectionQuality = QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_VeryLow"]);
	else if (collisionDetectionDensity > 10 && collisionDetectionDensity <= 25)
		collisionDetectionQuality = QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Low"]);
	else if (collisionDetectionDensity > 25 && collisionDetectionDensity <= 50)
		collisionDetectionQuality = QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Medium"]);
	else if (collisionDetectionDensity > 50 && collisionDetectionDensity <= 75)
		collisionDetectionQuality = QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Goog"]);
	else if (collisionDetectionDensity > 75 && collisionDetectionDensity < 100)
		collisionDetectionQuality = QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_VeryGood"]);

	QString pathPlanningAlgorithm;
	if (simulation->GetPathPlanningAlgorithm() == PathPlanningAlgorithm::Spline)
		pathPlanningAlgorithm = QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_Algorithm_BSpline"]);
	else if (simulation->GetPathPlanningAlgorithm() == PathPlanningAlgorithm::Arcs)
		pathPlanningAlgorithm = QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_Algorithm_Arcs"]);

	ui.simulationNameLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Name"]));
	ui.vehicleNameLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["SimulationInfo_VehicleName"]));
	ui.pathElementsLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["SimulationInfo_PathElements"]));
	ui.pathLengthLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["SimulationInfo_PathLength"]));
	ui.expandObstaclesSizePercentLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["SimulationInfo_ObstaclesExpandSizePercent"]));
	ui.pathPlanningAlgorithmLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["SimulationInfo_PathPlanningAlgorithm"]));
	ui.useAdmissibleArcsOnlyLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["SimulationInfo_UseAdmissibleArcsOnly"]));
	ui.collisionDetectionQualityLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["SimulationInfo_CollisionDetectionQuality"]));
	ui.btnClose->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Close"]));

	ui.simulationName->setText(QString::fromStdString(simulation->GetName()));
	ui.simulationVehicleName->setText(QString::fromStdString(simulation->GetVehicle()->GetName()));
	ui.simulationPathElements->setText(QString("%1").arg(simulation->GetFinalPath()->GetElements().size()));
	ui.simulationPathLength->setText(QString("%1").arg(CommonHelper::ConvertPixelsToMeters(simulation->GetFinalPath()->GetLength())));
	ui.simulationExpandSizePercent->setText(QString("%1").arg(simulation->GetExpandSize()));
	ui.simulationPathPlanningAlgorithm->setText(pathPlanningAlgorithm);
	ui.simulationUseAdmissibleArcsOnly->setText(simulation->GetUseAdmissibleArcsOnly() ? "True" : "False");
	ui.simulationCollisionDetectionQuality->setText(collisionDetectionQuality);
}

SimulationInfo::~SimulationInfo()
{
}