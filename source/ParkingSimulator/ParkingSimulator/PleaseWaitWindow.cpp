#include "PleaseWaitWindow.h"

PleaseWaitWindow::PleaseWaitWindow(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QMovie *movie = new QMovie("Resources/loadingBig.gif");
	ui.loadingModelsInProgressMovie->setMovie(movie);
	movie->start();

	ui.loadingModelsInProgressLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PleaseWait_LoadingModels"]));
}

PleaseWaitWindow::~PleaseWaitWindow()
{
}
