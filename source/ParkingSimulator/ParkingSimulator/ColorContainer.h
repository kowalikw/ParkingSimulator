#pragma once

#include <qlabel.h>

class ColorContainer : public QLabel
{
	Q_OBJECT
public:
	explicit ColorContainer(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ColorContainer();
protected:

	void mousePressEvent(QMouseEvent * e);

signals:

	void clicked(ColorContainer & pos);
};

