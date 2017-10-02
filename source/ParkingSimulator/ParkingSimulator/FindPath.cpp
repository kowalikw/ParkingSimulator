#include "FindPath.h"

FindPath::FindPath(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnFindPath, SIGNAL(released()), this, SLOT(findPath()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));
	//connect(ui.comboBoxAlgorithm, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](const QString &text) { algorithmChanged(); });
	connect(ui.comboBoxAlgorithm, SIGNAL(currentIndexChanged(int)), this, SLOT(algorithmChanged()));

	ui.comboBoxAlgorithm->addItem(QString("BSpline"));
	ui.comboBoxAlgorithm->addItem(QString("Arcs"));
	ui.checkBoxAdmissibleArcsOnly->hide();
}

FindPath::~FindPath()
{
}

double FindPath::GetExpandSizePercent()
{
	return this->expandSizePercent;
}

double FindPath::GetCollisionDetectionDensity()
{
	return this->collisionDetectionDensity;
}

PathPlanningAlgorithm FindPath::GetAlgorithm()
{
	return this->algorithm;
}

bool FindPath::UseOnlyAdmissibleArcs()
{
	return this->useAdmissibleArcsOnly;
}

void FindPath::algorithmChanged()
{
	if (ui.comboBoxAlgorithm->currentIndex() == 1)
		ui.checkBoxAdmissibleArcsOnly->show();
	else
		ui.checkBoxAdmissibleArcsOnly->hide();
}

void FindPath::findPath()
{
	this->expandSizePercent = ui.expandSizePercent->value();
	this->collisionDetectionDensity = ui.sliderCollisionDetectionDensity->value();
	switch (ui.comboBoxAlgorithm->currentIndex())
	{
	case 0:
		this->algorithm = PathPlanningAlgorithm::Spline;
		break;
	case 1:
		this->algorithm = PathPlanningAlgorithm::Arcs;
		break;
	}
	this->useAdmissibleArcsOnly = ui.checkBoxAdmissibleArcsOnly->isChecked();

	accept();
}