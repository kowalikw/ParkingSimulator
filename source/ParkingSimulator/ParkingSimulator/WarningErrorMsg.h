#pragma once

#include <QDialog>
#include <string>
#include "ui_WarningErrorMsg.h"

using namespace Ui;

enum MessageType
{
	Error,
	Warning
};

class WarningErrorMsg : public QDialog
{
	Q_OBJECT

public:
	WarningErrorMsg(std::string title, std::string content, MessageType type, QWidget *parent = Q_NULLPTR);
	~WarningErrorMsg();

private:
	WarningErrorMsgClass ui;
};
