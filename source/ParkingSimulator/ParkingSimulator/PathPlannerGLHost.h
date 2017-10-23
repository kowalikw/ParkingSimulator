#ifndef PATH_PLANNER_GL_HOST_H
#define PATH_PLANNER_GL_HOST_H

#include "OpenGLHost.h"
#include "PathPlanner.h"
#include "NvgDrawHelper.h"
#include "BezierCurve.h"

class PathPlannerGLHost : public OpenGLHost
{
	Q_OBJECT

public:
	PathPlannerGLHost(QWidget *parent);
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
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
	glm::vec2 positionOnMap;

	bool drawStartFlag = false;
	bool drawEndFlag = false;
	bool startFlagAdmissible = false;
	bool endFlagAdmissible = false;

	Path pathPolyline;
	Path *pathAdmissible;

	void nvgRenderFrame();
};

#endif