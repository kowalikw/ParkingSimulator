#include "WarningErrorMsg.h"

WarningErrorMsg::WarningErrorMsg(std::string title, std::string content, MessageType type, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnAccept, SIGNAL(released()), this, SLOT(reject()));

	switch (type)
	{
	case Warning:
		ui.messageIcon->setText(QString("<html><head/><body><img src='Resources/warningIcon.png'/></body></html>"));
		break;
	case Error:
		ui.messageIcon->setText(QString("<html><head/><body><img src='Resources/errorIcon.png'/></body></html>"));
		break;
	}

	ui.messageTitle->setText(QString::fromStdString(title));
	ui.messageContent->setText(QString::fromStdString(content));
	ui.btnAccept->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_CloseWindow"]));
}

WarningErrorMsg::~WarningErrorMsg()
{
}
