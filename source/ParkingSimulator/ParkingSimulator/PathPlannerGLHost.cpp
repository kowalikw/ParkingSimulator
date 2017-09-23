#include "PathPlannerGLHost.h"

PathPlannerGLHost::PathPlannerGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void PathPlannerGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	pathPlanner1.AddUserPoint(glm::vec2(mouseClickX, mouseClickY));
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

void PathPlannerGLHost::SetPathPlanner(PathPlanner * pathPlanner)
{
	this->pathPlanner = pathPlanner;
}

PathPlanner * PathPlannerGLHost::GetPathPlanner()
{
	return this->pathPlanner;
}

#pragma endregion

#pragma region OpenGL methods.

void PathPlannerGLHost::initializeGL()
{
	OpenGLHost::initializeGL();

	pathPlanner1 = PathPlanner(Map(), Vehicle(100, 50));

	// normal path
	pathPlanner->AddUserPoint(glm::vec2(200, 300));
	pathPlanner->AddUserPoint(glm::vec2(200, 500));
	pathPlanner->AddUserPoint(glm::vec2(500, 500));
	pathPlanner->AddUserPoint(glm::vec2(500, 300));

	pathPlanner->AddUserPoint(glm::vec2(700, 300));
	pathPlanner->AddUserPoint(glm::vec2(300, 100));

	pathPlanner->AddUserPoint(glm::vec2(100, 100));

	pathPlanner->AddUserPoint(glm::vec2(100, 300));
	pathPlanner->AddUserPoint(glm::vec2(310, 300));

	pathPlanner->AddUserPoint(glm::vec2(305, 171));

	pathPlanner->AddUserPoint(glm::vec2(678, 186));
	pathPlanner->AddUserPoint(glm::vec2(697, 488));
	pathPlanner->AddUserPoint(glm::vec2(988, 517));
	pathPlanner->AddUserPoint(glm::vec2(1000, 100));
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
	//pathAdmissible = pathPlanner->CreateAdmissiblePath(pathPlanner->UserPoints());
	
	//bspline = BSpline(pathPlanner.UserPoints());


	/*nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 1.0f);
	nvgStrokeColor(vg, nvgRGBA(255, 255, 0, 255));
	nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));

	nvgBeginPath(vg);

	for (glm::vec2 point : pathPlanner1.UserPoints())
	{
		//nvgCircle(vg, point.x, point.y, 5);
	}
	nvgFill(vg);

	nvgStroke(vg);

	renderPathPolyline();
	renderPathAdmissible();*/
	//drawBSpline();
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

	if (pathPlanner1.UserPoints().size() < 2) return;
	for (int i = 1; i < pathPlanner1.UserPoints().size(); i++)
	{
		nvgMoveTo(vg, pathPlanner1.UserPoints()[i - 1].x,  pathPlanner1.UserPoints()[i - 1].y);
		nvgLineTo(vg, pathPlanner1.UserPoints()[i].x, pathPlanner1.UserPoints()[i].y);
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

	//for (glm::vec2 point : pathPlanner.UserPoints())
	//{
		for (PathElement *pathElement : pathAdmissible->GetElements())
		{
			if (dynamic_cast<Line*>(pathElement) != NULL)
			{
				Line *line = dynamic_cast<Line*>(pathElement);
				nvgMoveTo(vg, line->GetFrom().x, line->GetFrom().y);
				nvgLineTo(vg, line->GetTo().x, line->GetTo().y);
			}
			else if (dynamic_cast<Circle*>(pathElement) != NULL)
			{
				Circle *circle = dynamic_cast<Circle*>(pathElement);
				for (double angle = circle->angleFrom; angle < circle->angleTo; angle += 0.01)
				{
					if (angle == circle->angleFrom)
					{
						auto p = circle->GetPointForAngle(angle);
						nvgMoveTo(vg, p.x, p.y);
					}
					auto p = circle->GetPointForAngle(angle);
					nvgLineTo(vg, p.x, p.y);
				}
			}
		}
	//}

	nvgStroke(vg);
}

#pragma endregion