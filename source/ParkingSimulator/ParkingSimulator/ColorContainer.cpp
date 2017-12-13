#include "ColorContainer.h"

ColorContainer::ColorContainer(QWidget* parent, Qt::WindowFlags f) : QLabel(parent) 
{	
}

ColorContainer::~ColorContainer()
{
}

void ColorContainer::mousePressEvent(QMouseEvent * e)
{
	QLabel::mousePressEvent(e);

	emit clicked(*this);
}
