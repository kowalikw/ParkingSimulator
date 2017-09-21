#include "CreateMap.h"

CreateMap::CreateMap(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnAccept, SIGNAL(released()), this, SLOT(createMap()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));
}

int CreateMap::GetMapWidth()
{
	return this->mapWidth;
}

int CreateMap::GetMapHeight()
{
	return this->mapHeight;
}

void CreateMap::createMap()
{
	this->mapWidth = ui.mapWidth->value();
	this->mapHeight = ui.mapHeight->value();

	accept();
}