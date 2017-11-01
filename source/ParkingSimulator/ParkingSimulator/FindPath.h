#pragma once

#include <QDialog>
#include "ui_FindPath.h"
#include "PathPlanner.h";
#include "Language.h"

using namespace Ui;

class FindPath : public QDialog
{
	Q_OBJECT

public:
	FindPath(QWidget *parent = Q_NULLPTR);
	~FindPath();

	double GetExpandSizePercent();
	double GetCollisionDetectionQuality();
	PathPlanningAlgorithm GetPathPlanningAlgorithm();
	bool UseOnlyAdmissibleArcs();
	int GetGraphExtraVerticesAlong();
	int GetGraphExtraVerticesAcross();

private:
	FindPathClass ui;

	double expandSizePercent;
	double collisionDetectionQuality;
	PathPlanningAlgorithm pathPlanningAlgorithm;
	bool useAdmissibleArcsOnly;
	int graphExtraVerticesAlong;
	int graphExtraVerticesAcross;
public slots:
	void findPath();
	void algorithmChanged();
	void collisionDetectionQualityChanged();
};
