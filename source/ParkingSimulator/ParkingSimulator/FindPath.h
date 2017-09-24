#pragma once

#include <QDialog>
#include "ui_FindPath.h"

using namespace Ui;

class FindPath : public QDialog
{
	Q_OBJECT

public:
	FindPath(QWidget *parent = Q_NULLPTR);
	~FindPath();

private:
	FindPathClass ui;
public slots:
	void findPath();
};
