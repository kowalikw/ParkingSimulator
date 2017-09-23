#ifndef PATH_PLANNER_GL_HOST_H
#define PATH_PLANNER_GL_HOST_H

#include "OpenGLHost.h"
#include "PathPlanner.h"

class PathPlannerGLHost : public OpenGLHost
{
	Q_OBJECT

public:
	PathPlannerGLHost(QWidget *parent);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent * event);
	void SetPathPlanner(PathPlanner *pathPlanner);
	PathPlanner * GetPathPlanner();
protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
private:
	PathPlanner *pathPlanner;

	PathPlanner pathPlanner1, pathPlanner2;
	Path pathPolyline;
	Path *pathAdmissible;
	//BSpline bspline;

	void nvgRenderFrame();
	void renderMap();
	void renderVehicle();
	void renderVoronoiGraph();
	void renderUserPoints();
	void renderPathPolyline();
	void renderPathAdmissible();
	void drawBSpline();
};

#endif