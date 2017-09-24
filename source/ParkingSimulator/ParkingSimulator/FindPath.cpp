#include "FindPath.h"

FindPath::FindPath(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnFindPath, SIGNAL(released()), this, SLOT(findPath()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));
}

FindPath::~FindPath()
{
}

void FindPath::findPath()
{
	accept();
}