#ifndef PATH_PLANNER_GL_HOST_H
#define PATH_PLANNER_GL_HOST_H

#include "OpenGLHost.h"

class PathPlannerGLHost : public OpenGLHost
{
	Q_OBJECT

public:
	PathPlannerGLHost(QWidget *parent);
protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
private:
	NVGcontext* vg = NULL;
};

#endif