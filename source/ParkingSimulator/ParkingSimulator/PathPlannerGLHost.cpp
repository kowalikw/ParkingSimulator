#include "PathPlannerGLHost.h"

PathPlannerGLHost::PathPlannerGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void PathPlannerGLHost::enterEvent(QEvent * event)
{
	OpenGLHost::enterEvent(event);
}

void PathPlannerGLHost::leaveEvent(QEvent * event)
{
	OpenGLHost::leaveEvent(event);
}

void PathPlannerGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	if (pathPlanner->GetSetStartPosition() && startFlagAdmissible)
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				pathPlanner->SetStartParkingSpace(dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)));
				pathPlanner->SetStartPoint(nullptr);
				pathPlanner->SetSetStartPosition(false);
				pathPlanner->SetStartPositionChanged(true);
				pathPlanner->GetVehicleStart()->SetRotation(-pathPlanner->GetHoverElement(positionOnMap)->GetRotation());
			}
		}
		else
		{
			if (pathPlanner->GetSetStartDirection())
			{
				glm::vec2 startDirection = glm::normalize(positionOnMap - *pathPlanner->GetStartPoint());
				pathPlanner->SetStartDirection(new glm::vec2(startDirection.x, startDirection.y));

				pathPlanner->SetSetStartPosition(false);
				pathPlanner->SetSetStartDirection(false);
				pathPlanner->SetStartPositionChanged(true);
			}
			else
			{
				glm::vec2 *startPosition = new glm::vec2(positionOnMap.x, positionOnMap.y);
				pathPlanner->SetStartParkingSpace(nullptr);
				pathPlanner->SetStartPoint(startPosition);
				pathPlanner->SetSetStartDirection(true);
			}
		}
	}

	if (pathPlanner->GetSetEndPosition() && endFlagAdmissible)
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				pathPlanner->SetEndParkingSpace(dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)));
				pathPlanner->SetEndPoint(nullptr);
				pathPlanner->SetSetEndPosition(false);
				pathPlanner->SetEndPositionChanged(true);
				pathPlanner->GetVehicleEnd()->SetRotation(-pathPlanner->GetHoverElement(positionOnMap)->GetRotation());
			}
		}
		else
		{
			if (pathPlanner->GetSetEndDirection())
			{
				glm::vec2 endDirection = glm::normalize(positionOnMap - *pathPlanner->GetEndPoint());
				pathPlanner->SetEndDrection(new glm::vec2(endDirection.x, endDirection.y));

				pathPlanner->SetSetEndPosition(false);
				pathPlanner->SetSetEndDirection(false);
				pathPlanner->SetEndPositionChanged(true);
			}
			else
			{
				glm::vec2 *endPosition = new glm::vec2(positionOnMap.x, positionOnMap.y);
				pathPlanner->SetEndParkingSpace(nullptr);
				pathPlanner->SetEndPoint(endPosition);
				pathPlanner->SetSetEndDirection(true);
			}
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

	if (pathPlanner->GetSetStartPosition() && !pathPlanner->GetSetStartDirection())
		pathPlanner->GetVehicleStart()->UpdateState(positionOnMap);

	if (pathPlanner->GetSetStartDirection())
	{
		glm::vec2 direction = GeometryHelper::GetLineDirection(*pathPlanner->GetStartPoint(), positionOnMap);
		pathPlanner->GetVehicleStart()->UpdateState(-atan2(direction.y, direction.x));
	}

	if (pathPlanner->GetSetEndPosition() && !pathPlanner->GetSetEndDirection())
		pathPlanner->GetVehicleEnd()->UpdateState(positionOnMap);

	if (pathPlanner->GetSetEndDirection())
	{
		glm::vec2 direction = GeometryHelper::GetLineDirection(*pathPlanner->GetEndPoint(), positionOnMap);
		pathPlanner->GetVehicleEnd()->UpdateState(-atan2(direction.y, direction.x));
	}

	if (pathPlanner->GetSetStartPosition())
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				drawStartFlag = true;
				startFlagAdmissible = true;
			}
			else
			{
				drawStartFlag = true;
				startFlagAdmissible = false;
			}
		}
		else if (!pathPlanner->GetMap()->IsVehicleAdmissible(pathPlanner->GetVehicleStart()))
		{
			startFlagAdmissible = false;
		}
		else
		{
			drawStartFlag = true;
			startFlagAdmissible = true;
		}
	}

	if (pathPlanner->GetSetEndPosition())
	{
		if (pathPlanner->GetHoverElement(positionOnMap) != NULL)
		{
			if (dynamic_cast<ParkingSpace*>(pathPlanner->GetHoverElement(positionOnMap)) != NULL)
			{
				drawEndFlag = true;
				endFlagAdmissible = true;
			}
			else
			{
				drawEndFlag = false;
				endFlagAdmissible = false;
			}
		}
		else if (!pathPlanner->GetMap()->IsVehicleAdmissible(pathPlanner->GetVehicleEnd()))
		{
			endFlagAdmissible = false;
		}
		else
		{
			drawEndFlag = true;
			endFlagAdmissible = true;
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
	if (pathPlanner->GetShowExpandedObstacles())
		nvgHelper->DrawMap(pathPlanner->GetExpandedMap());

	nvgHelper->DrawMap(pathPlanner->GetMap());

	if (pathPlanner->GetFinalPath() != NULL)
		nvgHelper->DrawPath(pathPlanner->GetFinalPath());

	if (drawStartFlag)
	{
		if (pathPlanner->GetSetStartPosition())
		{
			if (pathPlanner->GetSetStartDirection())
			{
				glm::vec2 direction = GeometryHelper::GetLineDirection(*pathPlanner->GetStartPoint(), positionOnMap);
				pathPlanner->GetVehicleStart()->UpdateState(*pathPlanner->GetStartPoint());
				nvgHelper->DrawStartFlag(pathPlanner->GetVehicleStart(), startFlagAdmissible);
			}
			else if (mouseInsideGLHost)
			{
				nvgHelper->DrawStartFlag(pathPlanner->GetVehicleStart(), startFlagAdmissible);
			}
		}
		else
		{
			if (pathPlanner->GetStartPoint() != NULL)
			{
				pathPlanner->GetVehicleStart()->UpdateState(*pathPlanner->GetStartPoint());
				nvgHelper->DrawStartFlag(pathPlanner->GetVehicleStart(), startFlagAdmissible);
			}
			else if (pathPlanner->GetStartParkingSpace() != NULL)
			{
				glm::vec2 position = pathPlanner->GetStartParkingSpace()->GetPosition();
				pathPlanner->GetVehicleStart()->UpdateState(position);
				nvgHelper->DrawStartFlag(pathPlanner->GetVehicleStart(), startFlagAdmissible);
			}
		}
	}

	if (drawEndFlag)
	{
		if (pathPlanner->GetSetEndPosition())
		{
			if (pathPlanner->GetSetEndDirection())
			{
				glm::vec2 direction = GeometryHelper::GetLineDirection(*pathPlanner->GetEndPoint(), positionOnMap);
				pathPlanner->GetVehicleEnd()->UpdateState(*pathPlanner->GetEndPoint());
				nvgHelper->DrawEndFlag(pathPlanner->GetVehicleEnd(), endFlagAdmissible);
			}
			else if (mouseInsideGLHost)
			{
				nvgHelper->DrawEndFlag(pathPlanner->GetVehicleEnd(), endFlagAdmissible);
			}
		}
		else
		{
			if (pathPlanner->GetEndPoint() != NULL)
			{
				pathPlanner->GetVehicleEnd()->UpdateState(*pathPlanner->GetEndPoint());
				nvgHelper->DrawEndFlag(pathPlanner->GetVehicleEnd(), endFlagAdmissible);
			}
			else if (pathPlanner->GetEndParkingSpace() != NULL)
			{
				glm::vec2 position = pathPlanner->GetEndParkingSpace()->GetPosition();
				pathPlanner->GetVehicleEnd()->UpdateState(position);
				nvgHelper->DrawEndFlag(pathPlanner->GetVehicleEnd(), endFlagAdmissible);
			}
		}
	}

	if (pathPlanner->GetShowVoronoiGraph() && pathPlanner->GetVoronoiGraph() != NULL)
	{
		nvgHelper->DrawGraph(pathPlanner->GetVoronoiGraph());
	}

	if (pathPlanner->GetShowFullVoronoiVisibilityGraph() && pathPlanner->GetFullVoronoiVisibilityGraph() != NULL)
	{
		nvgHelper->DrawGraph(pathPlanner->GetFullVoronoiVisibilityGraph());
	}

	if (pathPlanner->GetShowPolylinePath())
	{
		nvgHelper->DrawPath(pathPlanner->GetPolylinePath());
	}

	if (pathPlanner->GetShowParkingPath())
	{
		nvgHelper->DrawPath(pathPlanner->GetParkingPathStart());
		nvgHelper->DrawPath(pathPlanner->GetParkingPathEnd());
	}

	if (pathPlanner->GetShowFinalPath())
	{
		nvgHelper->DrawPath(pathPlanner->GetFinalPath());
	}

	if (pathPlanner->GetSelectedPathElement() != NULL)
	{
		nvgHelper->DrawPathElement(pathPlanner->GetSelectedPathElement(), true);
	}
}

#pragma endregion