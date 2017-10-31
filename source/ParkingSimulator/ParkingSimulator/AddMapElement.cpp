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
		ui.addMapElementDescription->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["AddMapElement_SelectBuilding"]));
		ui.newElementName->setText(QString("Building - box"));
		break;
	case D:
		initDecorations();
		ui.addMapElementDescription->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["AddMapElement_SelectDecoration"]));
		ui.newElementName->setText(QString("Decoration - box"));
		break;
	case P:
		initParkingSpaces();
		ui.addMapElementDescription->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["AddMapElement_SelectParkingPlace"]));
		ui.newElementName->setText(QString("Parking place - plane"));
		break;
	case C:
		initVehicles();
		ui.addMapElementDescription->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["AddMapElement_SelectVehicle"]));
		ui.newElementName->setText(QString("Kia Rio"));
		break;
	case T:
		initTerrains();
		ui.addMapElementDescription->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["AddMapElement_SelectTerrain"]));
		ui.newElementName->setText(QString("Grass"));
		ui.newElementName->setEnabled(false);
		ui.newElementWidth->hide();
		ui.newElementHeight->hide();
		ui.newElementRotation->hide();
		ui.newElementWidthLabel->hide();
		ui.newElementHeightLabel->hide();
		ui.newElementRotationLabel->hide();
		break;
	}

	ui.windowTitle->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["AddMapElement_Title"]));
	ui.newElementNameLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Name"]));
	ui.newElementWidthLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Width"]));
	ui.newElementHeightLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Height"]));
	ui.newElementRotationLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Rotation"]));
	ui.btnAccept->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Add"]));
	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Cancel"]));

	this->type = type;

	createElement(0);

	ui.newElementName->setText(QString::fromStdString(model->name));
	ui.newElementWidth->setValue(model->defaultWidth);
	ui.newElementHeight->setValue(model->defaultHeight);
}

AddMapElement::~AddMapElement()
{
}

MapElement * AddMapElement::GetNewMapElement()
{
	return this->newMapElement;
}

Terrain * AddMapElement::GetNewTerrain()
{
	return this->newTerrain;
}

void AddMapElement::initBuildings()
{
	buildings = Settings::getInstance()->GetBuildings();
	for (int i = 0; i < buildings.size(); i++)
	{
		QIcon icon;
		icon.addFile(QString::fromStdString(buildings[i].thumbnail), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		mapElement->setToolTip(QString::fromStdString(buildings[i].name));
		if (i == 0)
			mapElement->setStyleSheet("border: 3px solid #d86a39;");
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(clicked()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::initDecorations()
{
	decorations = Settings::getInstance()->GetDecorations();
	for (int i = 0; i < decorations.size(); i++)
	{
		QIcon icon;
		icon.addFile(QString::fromStdString(decorations[i].thumbnail), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		mapElement->setToolTip(QString::fromStdString(decorations[i].name));
		if (i == 0)
			mapElement->setStyleSheet("border: 3px solid #d86a39;");
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(pressed()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::initParkingSpaces()
{
	parkingPlaces = Settings::getInstance()->GetParkingPlaces();
	for (int i = 0; i < parkingPlaces.size(); i++)
	{
		QIcon icon;
		icon.addFile(QString::fromStdString(parkingPlaces[i].thumbnail), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		mapElement->setToolTip(QString::fromStdString(parkingPlaces[i].name));
		if (i == 0)
			mapElement->setStyleSheet("border: 3px solid #d86a39;");
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(clicked()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::initVehicles()
{
	vehicles = Settings::getInstance()->GetVehicles();
	for (int i = 0; i < vehicles.size(); i++)
	{
		QIcon icon;
		icon.addFile(QString::fromStdString(vehicles[i].thumbnail), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		mapElement->setToolTip(QString::fromStdString(vehicles[i].name));
		if (i == 0)
			mapElement->setStyleSheet("border: 3px solid #d86a39;");
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(clicked()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::initTerrains()
{
	terrains = Settings::getInstance()->GetTerrains();
	for (int i = 0; i < terrains.size(); i++)
	{
		QIcon icon;
		icon.addFile(QString::fromStdString(terrains[i].thumbnail), QSize(), QIcon::Normal, QIcon::Off);

		QPushButton *mapElement;
		mapElement = new QPushButton(this);
		mapElement->setCursor(QCursor(Qt::PointingHandCursor));
		mapElement->setStyleSheet(QString("margin-left: 0px;"));
		mapElement->setIcon(icon);
		mapElement->setIconSize(QSize(64, 64));
		mapElement->setToolTip(QString::fromStdString(terrains[i].name));
		if (i == 0)
			mapElement->setStyleSheet("border: 3px solid #d86a39;");
		ui.mapElementsContainer->addWidget(mapElement);

		QObject::connect(mapElement, SIGNAL(clicked()), this, SLOT(clickedSlot()));
	}
}

void AddMapElement::createElement(int index)
{
	switch (type)
	{
	case B:
		model = &buildings[index];
		newMapElement = new Obstacle(glm::vec2(), ObstacleType::Building);
		break;
	case D:
		model = &decorations[index];
		newMapElement = new Obstacle(glm::vec2(), ObstacleType::Decoration);
		break;
	case P:
		model = &parkingPlaces[index];
		newMapElement = new ParkingSpace(glm::vec2(), index == 2 ? ParkingSpaceType::Perpendicular : ParkingSpaceType::Paralell);
		break;
	case C:
		model = &vehicles[index];
		newMapElement = new Vehicle();
		break;
	case T:
		model = &terrains[index];
		newTerrain = new Terrain(model->model, glm::vec3(model->r, model->g, model->b), glm::vec2());
		break;
	}
}

void AddMapElement::clickedSlot()
{
	int selectedIndex = -1;
	auto btn = qobject_cast<QPushButton*>(sender());
	for (int i = 1; i < ui.mapElementsContainer->count(); i++)
	{
		auto el = ui.mapElementsContainer->itemAt(i)->widget();
		QPushButton *element = dynamic_cast<QPushButton*>(el);
		if (element != NULL)
		{
			element->setStyleSheet("");
			if (btn == element)
				selectedIndex = i - 1;
		}
	}
	btn->setStyleSheet("border: 3px solid #d86a39;");
	if (selectedIndex < 0)
		selectedIndex = 0;

	createElement(selectedIndex);

	ui.newElementName->setText(QString::fromStdString(model->name));
	ui.newElementWidth->setValue(model->defaultWidth);
	ui.newElementHeight->setValue(model->defaultHeight);
}

void AddMapElement::addElement()
{
	if(newMapElement == NULL && newTerrain == NULL)
		createElement(0);
	else
	{
		if (newMapElement != NULL)
		{
			newMapElement->SetSize(glm::vec2(ui.newElementWidth->value(), ui.newElementHeight->value()));
			newMapElement->SetName(ui.newElementName->text().toStdString());
			newMapElement->SetRotation(glm::radians(ui.newElementRotation->value()));
			newMapElement->SetThumbnailPath(model->thumbnail);
			newMapElement->SetModelPath(model->model);
		}
	}

	accept();
}