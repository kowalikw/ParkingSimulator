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

	pathPlanner = PathPlanner(Map(), Vehicle(100, 50));

	pathPlanner.AddUserPoint(glm::vec2(200, 300));
	pathPlanner.AddUserPoint(glm::vec2(200, 700));
	pathPlanner.AddUserPoint(glm::vec2(900, 700));
	pathPlanner.AddUserPoint(glm::vec2(900, 900));


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

void PathPlannerGLHost::renderMap()
{
}

void PathPlannerGLHost::renderVehicle()
{
}

void PathPlannerGLHost::nvgRenderFrame()
{
	pathAdmissible = pathPlanner.CreateAdmissiblePath(pathPlanner.UserPoints());
	bspline = BSpline(pathPlanner.UserPoints());


	nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 1.0f);
	nvgStrokeColor(vg, nvgRGBA(255, 255, 0, 255));
	nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));

	nvgBeginPath(vg);

	for (glm::vec2 point : pathPlanner.UserPoints())
	{
		//nvgCircle(vg, point.x, point.y, 5);
	}
	nvgFill(vg);

	nvgStroke(vg);

	renderPathPolyline();
	//renderPathAdmissible();
	drawBSpline();
}

void PathPlannerGLHost::renderVoronoiGraph()
{
}

void PathPlannerGLHost::renderUserPoints()
{
}

void PathPlannerGLHost::renderPathPolyline()
{
	nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 1.0f);
	nvgStrokeColor(vg, nvgRGBA(255, 0, 0, 255));
	nvgBeginPath(vg);

	if (pathPlanner.UserPoints().size() < 2) return;
	for (int i = 1; i < pathPlanner.UserPoints().size(); i++)
	{
		nvgMoveTo(vg, pathPlanner.UserPoints()[i - 1].x,  pathPlanner.UserPoints()[i - 1].y);
		nvgLineTo(vg, pathPlanner.UserPoints()[i].x, pathPlanner.UserPoints()[i].y);
	}

	nvgStroke(vg);
}

void PathPlannerGLHost::renderPathAdmissible()
{
	nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 1.0f);
	nvgStrokeColor(vg, nvgRGBA(255, 255, 0, 255));

	nvgBeginPath(vg);

	for (glm::vec2 point : pathPlanner.UserPoints())
	{
		for (PathElement pathElement : pathAdmissible.GetElements())
		{
			if (pathElement.type == Line)
			{
				nvgMoveTo(vg, pathElement.GetFrom().x, pathElement.GetFrom().y);
				nvgLineTo(vg, pathElement.GetTo().x, pathElement.GetTo().y);
			}
			else if (pathElement.type == Circle)
			{
				//nvgArc(vg, 100, 100, 40, 0, -M_PI / 2, NVG_CCW);
				for (double angle = pathElement.angleFrom; angle < pathElement.angleTo; angle += 0.01)
				{
					if (angle == pathElement.angleFrom)
					{
						auto p = pathElement.GetCirclePoint(angle);
						nvgMoveTo(vg, p.x, p.y);
					}
					auto p = pathElement.GetCirclePoint(angle);
					nvgLineTo(vg, p.x, p.y);
				}
			}
		}
	}

	nvgStroke(vg);
}

void PathPlannerGLHost::drawBSpline()
{
	nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 1.0f);
	nvgStrokeColor(vg, nvgRGBA(255, 255, 0, 255));

	nvgBeginPath(vg);

	auto p = bspline.CalculatePoint(0);
	nvgMoveTo(vg, p.x, p.y);
	for (double t = 0; t <= 1.0; t += 0.01)
	{
		auto p = bspline.CalculatePoint(t);

		nvgLineTo(vg, p.x, p.y);
	}

	nvgStroke(vg);
}

#pragma endregion