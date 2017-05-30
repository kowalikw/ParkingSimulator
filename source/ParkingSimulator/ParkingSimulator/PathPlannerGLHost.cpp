#include "PathPlannerGLHost.h"

PathPlannerGLHost::PathPlannerGLHost(QWidget *parent) : OpenGLHost(parent) { }

void PathPlannerGLHost::initializeGL()
{
	OpenGLHost::initializeGL();

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
}

void PathPlannerGLHost::resizeGL(int w, int h)
{
	OpenGLHost::resizeGL(w, h);
}

void PathPlannerGLHost::paintGL()
{
	OpenGLHost::paintGL();

	// Update and render
	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, widgetWidth, widgetHeight, pixelRatio);

	nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 2.0f);
	nvgStrokeColor(vg, nvgRGBA(0, 192, 255, 255));
	nvgBeginPath(vg);

	for (double a = 0; a < 60; a += 0.01)
	{
		double x = 400 + (100 - 10 * a) * cos(a);
		double y = 400 + (100 - 10 * a) * sin(a);

		if (a == 0)
			nvgMoveTo(vg, x, y);
		nvgLineTo(vg, x, y);
	}

	nvgStroke(vg);

	nvgEndFrame(vg);
}
