#ifndef VISUALISATION_GL_HOST_H
#define VISUALISATION_GL_HOST_H

#include "OpenGLHost.h"
#include "Visualisation.h"
#include "Vehicle.h"

class VisualisationGLHost : public OpenGLHost
{
	Q_OBJECT

public:
	VisualisationGLHost(QWidget *parent);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent * event);
	void SetVisualisation(Visualisation *visualisation);
	Visualisation *GetVisualisation();
protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
private:
	Visualisation* visualisation;
	NVGcontext* vg = NULL;

	void nvgRenderFrame();
	void drawVehicle(Vehicle vehicle);
	void drawMap(Map map);
	void drawPath(Path path);
	void drawMapElement(MapElement *mapElement);
};

#endif