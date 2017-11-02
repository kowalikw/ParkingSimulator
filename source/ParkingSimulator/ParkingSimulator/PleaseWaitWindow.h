#pragma once

#include <QDialog>
#include "ui_PleaseWaitWindow.h"
#include <QMovie>
#include "Language.h"

class PleaseWaitWindow : public QDialog
{
	Q_OBJECT

public:
	PleaseWaitWindow(QDialog *QDialog = Q_NULLPTR);
	~PleaseWaitWindow();

private:
	Ui::PleaseWaitWindowClass ui;
};
