#pragma once

#include <QDialog>
#include <QFileDialog>
#include "ui_SelectMap.h"
#include "Map.h"
#include "MapEditor.h"
#include "WarningErrorMsg.h"

using namespace Ui;

class SelectMap : public QDialog
{
	Q_OBJECT

public:
	SelectMap(MapEditor *mapEditor, QWidget *parent = Q_NULLPTR);
	~SelectMap();

	Map * GetMap();

private:
	SelectMapClass ui;

	Map *map;
	MapEditor *mapEditor;

public slots:
	void loadMapFromMapEditor();
	void loadMapFromFile();
};
