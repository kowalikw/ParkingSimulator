#pragma once

#include <QWidget>
#include "ui_AddBuilding.h"

class AddBuilding : public QWidget
{
	Q_OBJECT

public:
	AddBuilding(QWidget *parent = Q_NULLPTR);
	~AddBuilding();

private:
	Ui::AddBuilding ui;
};
