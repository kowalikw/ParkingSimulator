#include "ParkingSimulator.h"

ParkingSimulator::ParkingSimulator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: #2a2a2a;");

	renderTimer = new QTimer();
	renderTimer->setInterval(20);
	renderTimer->start();

	connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderTimerCall()));

	ui.treeMapElements->setColumnCount(1);
	QList<QTreeWidgetItem *> items;

	auto buildings = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Buildings")));
	auto decorations = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Decorations")));
	auto roads = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Roads")));
	auto parkPlaces = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Park places")));

	items.append(buildings);
	items.append(decorations);
	items.append(roads);
	items.append(parkPlaces);

	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 10; ++i)
		items.append(new QTreeWidgetItem(buildings, QStringList(QString("Building: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 5; ++i)
		items.append(new QTreeWidgetItem(decorations, QStringList(QString("Decoration: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 5; ++i)
		items.append(new QTreeWidgetItem(roads, QStringList(QString("Road: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);

	items.clear();
	for (int i = 0; i < 5; ++i)
		items.append(new QTreeWidgetItem(parkPlaces, QStringList(QString("Park place: %1").arg(i))));
	ui.treeMapElements->insertTopLevelItems(0, items);
}

void ParkingSimulator::renderTimerCall()
{
	ui.glPathPlanner->repaint();
}
