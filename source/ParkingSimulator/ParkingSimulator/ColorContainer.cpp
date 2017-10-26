#include "ColorContainer.h"
#include <qcolordialog.h>

ColorContainer::ColorContainer(QWidget* parent, Qt::WindowFlags f)
	: QLabel(parent) {
	this->setStyleSheet("background-color: green;");
}

ColorContainer::~ColorContainer()
{
}

void ColorContainer::mousePressEvent(QMouseEvent * e)
{
	QLabel::mousePressEvent(e);

	emit clicked(*this);
}
