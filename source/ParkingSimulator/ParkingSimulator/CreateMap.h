#pragma once

#include <QDialog>
#include "ui_CreateMap.h"
#include "Language.h"

using namespace Ui;

class CreateMap : public QDialog
{
	Q_OBJECT

public:
	CreateMap(QWidget *parent = Q_NULLPTR);
	int GetMapWidth();
	int GetMapHeight();
private:
	CreateMapClass ui;
	int mapWidth;
	int mapHeight;
public slots:
	void createMap();
};
