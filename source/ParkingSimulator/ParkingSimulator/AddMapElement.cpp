#include "AddMapElement.h"

AddMapElement::AddMapElement(AddMapElementType type, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnAccept, SIGNAL(released()), this, SLOT(addElement()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));

	switch (type)
	{
	case B:
		initBuildings();
		ui.addMapElementDescription->setText(QString("Select building type:"));
		break;
	case D:
		initDecorations();
		ui.addMapElementDescription->setText(QString("Select decoration type:"));
		break;
	case P:
		initParkingSpaces();
		ui.addMapElementDescription->setText(QString("Select parking place type:"));
		break;
	}

	
}

AddMapElement::~AddMapElement()
{
}

void AddMapElement::initBuildings()
{
	for (int i = 0; i < 5; i++)
	{
		QIcon icon;
		icon.addFile(QStringLiteral("Resources/MapEditorIcons/AddBuildingIcons/default.png"), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(clicked()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::initDecorations()
{
	auto decorations = Settings::getInstance()->GetDecorations();
	for (int i = 0; i < decorations.size(); i++)
	{
		QIcon icon;
		icon.addFile(QStringLiteral("Resources/MapEditorIcons/AddBuildingIcons/default.png"), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(pressed()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::initParkingSpaces()
{
	for (int i = 0; i < 5; i++)
	{
		QIcon icon;
		icon.addFile(QStringLiteral("Resources/MapEditorIcons/AddBuildingIcons/default.png"), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(clicked()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::clickedSlot()
{
	auto btn = qobject_cast<QPushButton*>(sender());
	for (int i = 1; i < ui.mapElementsContainer->count(); i++)
	{
		auto el = ui.mapElementsContainer->itemAt(i)->widget();
		QPushButton *element = dynamic_cast<QPushButton*>(el);
		if(element != NULL)
			element->setStyleSheet("");
	}
	btn->setStyleSheet("border: 3px solid #d86a39;");
}

void AddMapElement::addElement()
{

}