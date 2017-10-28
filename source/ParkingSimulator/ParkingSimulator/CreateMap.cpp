#include "CreateMap.h"

CreateMap::CreateMap(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnAccept, SIGNAL(released()), this, SLOT(createMap()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));

	ui.createNewMapLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["MapEditor_CreateNewMap"]));
	ui.widthLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Width"]));
	ui.heightLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Height"]));
	ui.btnAccept->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Create"]));
	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Cancel"]));
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
	this->mapWidth = CommonHelper::ConverMetersToPixeks(ui.mapWidth->value());
	this->mapHeight = CommonHelper::ConverMetersToPixeks(ui.mapHeight->value());

	accept();
}