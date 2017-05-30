#include "PathPlannerGLHost.h"

PathPlannerGLHost::PathPlannerGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void PathPlannerGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	pathPlanner.AddUserPoint(glm::vec2(mouseClickX, mouseClickY));
}

void PathPlannerGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	OpenGLHost::mouseReleaseEvent(event);
}

void PathPlannerGLHost::mouseMoveEvent(QMouseEvent * event)
{
	OpenGLHost::mouseMoveEvent(event);
}

void PathPlannerGLHost::keyPressEvent(QKeyEvent * event)
{
	OpenGLHost::keyPressEvent(event);
}

void PathPlannerGLHost::keyReleaseEvent(QKeyEvent * event)
{
	OpenGLHost::keyReleaseEvent(event);
}

#pragma endregion

#pragma region OpenGL methods.

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

	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, widgetWidth, widgetHeight, pixelRatio);

	nvgRenderFrame();

	nvgEndFrame(vg);
}

#pragma endregion

#pragma region Private methods.

void PathPlannerGLHost::nvgRenderFrame()
{
	nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 2.0f);
	nvgStrokeColor(vg, nvgRGBA(255, 255, 0, 255));
	nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));

	nvgBeginPath(vg);

	for (glm::vec2 point : pathPlanner.UserPoints())
	{
		nvgCircle(vg, point.x, point.y, 5);
	}
	nvgFill(vg);

	nvgStroke(vg);
}

#pragma endregion