#include "FindPath.h"

FindPath::FindPath(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnFindPath, SIGNAL(released()), this, SLOT(findPath()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));

	ui.comboBoxAlgorithm->addItem(QString("BSpline"));
	ui.comboBoxAlgorithm->addItem(QString("Arcs"));
}

FindPath::~FindPath()
{
}

double FindPath::GetExpandSizePercent()
{
	return this->expandSizePercent;
}

PathPlanningAlgorithm FindPath::GetAlgorithm()
{
	return this->algorithm;
}

void FindPath::findPath()
{
	this->expandSizePercent = ui.expandSizePercent->value();
	switch (ui.comboBoxAlgorithm->currentIndex())
	{
	case 0:
		this->algorithm = PathPlanningAlgorithm::Spline;
		break;
	case 1:
		this->algorithm = PathPlanningAlgorithm::Arcs;
		break;
	}

	accept();
}