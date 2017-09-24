#include "PathPlannerGLHost.h"

PathPlannerGLHost::PathPlannerGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void PathPlannerGLHost::enterEvent(QEvent * event)
{
	OpenGLHost::enterEvent(event);

	setFocus();
}

void PathPlannerGLHost::leaveEvent(QEvent * event)
{
	OpenGLHost::leaveEvent(event);
}

void PathPlannerGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	if (pathPlanner->GetSetStartPosition())
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				pathPlanner->SetStartParkingSpace(dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)));
				pathPlanner->SetStartPoint(nullptr);
				pathPlanner->SetSetStartPosition(false);
				pathPlanner->SetStartPositionChanged(true);
			}
		}
		else
		{
			glm::vec2 *startPosition = new glm::vec2(positionOnMap.x, positionOnMap.y);
			pathPlanner->SetStartParkingSpace(nullptr);
			pathPlanner->SetStartPoint(startPosition);
			pathPlanner->SetSetStartPosition(false);
			pathPlanner->SetStartPositionChanged(true);
		}
	}

	if (pathPlanner->GetSetEndPosition())
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				pathPlanner->SetEndParkingSpace(dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)));
				pathPlanner->SetEndPoint(nullptr);
				pathPlanner->SetSetEndPosition(false);
				pathPlanner->SetEndPositionChanged(true);
			}
		}
		else
		{
			glm::vec2 *endPosition = new glm::vec2(positionOnMap.x, positionOnMap.y);
			pathPlanner->SetEndParkingSpace(nullptr);
			pathPlanner->SetEndPoint(endPosition);
			pathPlanner->SetSetEndPosition(false);
			pathPlanner->SetEndPositionChanged(true);
		}
	}
}

void PathPlannerGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	OpenGLHost::mouseReleaseEvent(event);
}

void PathPlannerGLHost::mouseMoveEvent(QMouseEvent * event)
{
	OpenGLHost::mouseMoveEvent(event);

	positionOnMap = (glm::vec2(mouseLastX, mouseLastY) - drawAreaPosition - widgetOffset) / magnificationRatio;

	if (pathPlanner->GetSetStartPosition())
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				drawStartFlag = true;
			}
			else
			{
				drawStartFlag = false;
			}
		}
		else
		{
			drawStartFlag = true;
		}
	}

	if (pathPlanner->GetSetEndPosition())
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				drawEndFlag = true;
			}
			else
			{
				drawEndFlag = false;
			}
		}
		else
		{
			drawEndFlag = true;
		}
	}
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

void PathPlannerGLHost::nvgRenderFrame()
{
	nvgHelper->DrawMap(pathPlanner->GetMap());

	if (drawStartFlag)
	{
		if (pathPlanner->GetSetStartPosition())
		{
			if (mouseInsideGLHost)
				nvgHelper->DrawStartFlag(positionOnMap);
		}
		else
		{
			if (pathPlanner->GetStartPoint() != NULL)
			{
				glm::vec2 position = *pathPlanner->GetStartPoint();
				nvgHelper->DrawStartFlag(position);
			}
			else if (pathPlanner->GetStartParkingSpace() != NULL)
			{
				glm::vec2 position = pathPlanner->GetStartParkingSpace()->GetPosition();
				nvgHelper->DrawStartFlag(position);
			}
		}
	}

	if (drawEndFlag)
	{
		if (pathPlanner->GetSetEndPosition())
		{
			if (mouseInsideGLHost)
				nvgHelper->DrawEndFlag(positionOnMap);
		}
		else
		{
			if (pathPlanner->GetEndPoint() != NULL)
			{
				glm::vec2 position = *pathPlanner->GetEndPoint();
				nvgHelper->DrawEndFlag(position);
			}
			else if (pathPlanner->GetEndParkingSpace() != NULL)
			{
				glm::vec2 position = pathPlanner->GetEndParkingSpace()->GetPosition();
				nvgHelper->DrawEndFlag(position);
			}
		}
	}

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

void PathPlannerGLHost::renderPathPolyline()
{
	nvgSave(vg);

	nvgLineCap(vg, NVG_ROUND);
	nvgLineJoin(vg, NVG_MITER);

	nvgStrokeWidth(vg, 1.0f);
	nvgStrokeColor(vg, nvgRGBA(255, 0, 0, 255));
	nvgBeginPath(vg);

	/*if (pathPlanner1.UserPoints().size() < 2) return;
	for (int i = 1; i < pathPlanner1.UserPoints().size(); i++)
	{
		nvgMoveTo(vg, pathPlanner1.UserPoints()[i - 1].x,  pathPlanner1.UserPoints()[i - 1].y);
		nvgLineTo(vg, pathPlanner1.UserPoints()[i].x, pathPlanner1.UserPoints()[i].y);
	}*/

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