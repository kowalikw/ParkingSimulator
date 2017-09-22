#pragma once

#include <QDialog>
#include <vector>
#include "ui_AddMapElement.h"
#include "Settings.h"

using namespace Ui;

enum AddMapElementType
{
	B,
	D,
	P
};

class AddMapElement : public QDialog
{
	Q_OBJECT

public:
	AddMapElement(AddMapElementType type, QWidget *parent = Q_NULLPTR);
	~AddMapElement();

private:
	AddMapElementClass ui;

	void initBuildings();
	void initDecorations();
	void initParkingSpaces();
public slots:
	void addElement();
	void clickedSlot();
};
