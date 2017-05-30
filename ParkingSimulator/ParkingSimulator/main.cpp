#include "ParkingSimulator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ParkingSimulator w;
	w.show();
	return a.exec();
}
