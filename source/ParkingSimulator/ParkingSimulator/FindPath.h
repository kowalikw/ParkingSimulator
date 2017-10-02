#pragma once

#include <QDialog>
#include "ui_FindPath.h"
#include "PathPlanner.h";

using namespace Ui;

class FindPath : public QDialog
{
	Q_OBJECT

public:
	FindPath(QWidget *parent = Q_NULLPTR);
	~FindPath();

	double GetExpandSizePercent();
	PathPlanningAlgorithm GetAlgorithm();
	bool UseOnlyAdmissibleArcs();

private:
	FindPathClass ui;

	double expandSizePercent;
	PathPlanningAlgorithm algorithm;
public slots:
	void findPath();
	void algorithmChanged();
};
