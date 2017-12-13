#ifndef COLOR_CONTAINER_H
#define COLOR_CONTAINER_H

#include <qlabel.h>
#include <qcolordialog.h>

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

#endif