#include "SelectMap.h"

SelectMap::SelectMap(MapEditor *mapEditor, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnLoadFromMapEditor, SIGNAL(released()), this, SLOT(loadMapFromMapEditor()));
	connect(ui.btnLoadFromFile, SIGNAL(released()), this, SLOT(loadMapFromFile()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));

	ui.btnLoadFromMapEditor->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_SetMap_FromMapEditor"]));
	ui.btnLoadFromFile->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_SetMap_FromFile"]));
	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Cancel"]));
	
	this->mapEditor = mapEditor;
}

SelectMap::~SelectMap()
{
}

Map * SelectMap::GetMap()
{
	return this->map;
}

void SelectMap::loadMapFromMapEditor()
{
	this->map = mapEditor->GetMap();

	if (map != NULL && map->GetWidth() > 0 && map->GetHeight() > 0)
		accept();
	else
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_NoMapInMapEditor_Title"], Language::getInstance()->GetDictionary()["WarningError_NoMapInMapEditor_Content"], MessageType::Warning);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
	}
}

void SelectMap::loadMapFromFile()
{
	QString filter = tr("Map files (*.map)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open map", QString(), tr("All files (*.*);;Map files (*.map)"), &filter);

	if (filePath != "")
	{
		map = new Map(0, 0);

		std::ifstream ifs(filePath.toStdString());

		boost::archive::text_iarchive ia(ifs);

		ia >> this->map;

		accept();
	}
}
