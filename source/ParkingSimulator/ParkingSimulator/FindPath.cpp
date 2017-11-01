#include "FindPath.h"

FindPath::FindPath(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnFindPath, SIGNAL(released()), this, SLOT(findPath()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));
	connect(ui.comboBoxAlgorithm, SIGNAL(currentIndexChanged(int)), this, SLOT(algorithmChanged()));
	connect(ui.sliderCollisionDetectionQuality, SIGNAL(valueChanged(int)), this, SLOT(collisionDetectionQualityChanged()));

	ui.comboBoxAlgorithm->addItem(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_Algorithm_BSpline"]));
	ui.comboBoxAlgorithm->addItem(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_Algorithm_Arcs"]));
	ui.checkBoxAdmissibleArcsOnly->setEnabled(false);

	ui.findPathTitle->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_FindPathTitle"]));
	ui.expandObstaclesPercentLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_ExpandObstaclesPercent"]));
	ui.pathPlanningAlgorithmLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_Algorithm"]));
	ui.checkBoxAdmissibleArcsOnly->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_UseAdmissibleArcsOnly"]));
	ui.collisionDetectionQualityLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_CollisionDetectionQuality"]));
	ui.ExtraVerticesAlongLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_GraphExtraVerticesAlong"]));
	ui.extraVerticesAcrossLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_GraphExtraVerticesAcross"]));
	ui.btnFindPath->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_FindPath"]));
	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Cancel"]));

	collisionDetectionQualityChanged();
}

FindPath::~FindPath()
{
}

double FindPath::GetExpandSizePercent()
{
	return this->expandSizePercent;
}

double FindPath::GetCollisionDetectionQuality()
{
	return this->collisionDetectionQuality;
}

PathPlanningAlgorithm FindPath::GetPathPlanningAlgorithm()
{
	return this->pathPlanningAlgorithm;
}

bool FindPath::UseOnlyAdmissibleArcs()
{
	return this->useAdmissibleArcsOnly;
}

int FindPath::GetGraphExtraVerticesAlong()
{
	return this->graphExtraVerticesAlong;
}

int FindPath::GetGraphExtraVerticesAcross()
{
	return this->graphExtraVerticesAcross;
}

void FindPath::algorithmChanged()
{
	if (ui.comboBoxAlgorithm->currentIndex() == 1)
		ui.checkBoxAdmissibleArcsOnly->setEnabled(true);
	else
		ui.checkBoxAdmissibleArcsOnly->setEnabled(false);
}

void FindPath::collisionDetectionQualityChanged()
{
	double collisionDetectionQuality = ui.sliderCollisionDetectionQuality->value();
	if (collisionDetectionQuality == 0)
		ui.labelCollisionDetectionQuality->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Disabled"]));
	else if (collisionDetectionQuality > 0 && collisionDetectionQuality <= 10)
		ui.labelCollisionDetectionQuality->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_VeryLow"]));
	else if (collisionDetectionQuality > 10 && collisionDetectionQuality <= 25)
		ui.labelCollisionDetectionQuality->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Low"]));
	else if (collisionDetectionQuality > 25 && collisionDetectionQuality <= 50)
		ui.labelCollisionDetectionQuality->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Medium"]));
	else if (collisionDetectionQuality > 50 && collisionDetectionQuality <= 75)
		ui.labelCollisionDetectionQuality->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_Good"]));
	else if (collisionDetectionQuality > 75 && collisionDetectionQuality < 100)
		ui.labelCollisionDetectionQuality->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Quality_VeryGood"]));
}

void FindPath::findPath()
{
	this->expandSizePercent = ui.expandSizePercent->value();
	this->collisionDetectionQuality = ui.sliderCollisionDetectionQuality->value();
	switch (ui.comboBoxAlgorithm->currentIndex())
	{
	case 0:
		this->pathPlanningAlgorithm = PathPlanningAlgorithm::Spline;
		break;
	case 1:
		this->pathPlanningAlgorithm = PathPlanningAlgorithm::Arcs;
		break;
	}
	this->useAdmissibleArcsOnly = ui.checkBoxAdmissibleArcsOnly->isChecked();
	this->graphExtraVerticesAlong = ui.graphExtraVerticesAlong->value();
	this->graphExtraVerticesAcross = ui.graphExtraVerticesAcross->value();

	accept();
}