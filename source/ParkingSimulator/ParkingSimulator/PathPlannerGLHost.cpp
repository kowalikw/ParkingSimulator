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
	pathPlanner->AddUserPoint(glm::vec2(100, 100));
	pathPlanner->AddUserPoint(glm::vec2(100, 100));
	pathPlanner->AddUserPoint(glm::vec2(100, 100));
	pathPlanner->AddUserPoint(glm::vec2(300, 100));
	pathPlanner->AddUserPoint(glm::vec2(350, 400));
	pathPlanner->AddUserPoint(glm::vec2(500, 400));
	pathPlanner->AddUserPoint(glm::vec2(500, 400));
	pathPlanner->AddUserPoint(glm::vec2(500, 400));

	/*pathPlanner->AddUserPoint(glm::vec2(700, 300));
	pathPlanner->AddUserPoint(glm::vec2(300, 100));

	pathPlanner->AddUserPoint(glm::vec2(100, 100));

	pathPlanner->AddUserPoint(glm::vec2(100, 300));
	pathPlanner->AddUserPoint(glm::vec2(310, 300));

	pathPlanner->AddUserPoint(glm::vec2(305, 171));

	pathPlanner->AddUserPoint(glm::vec2(678, 186));
	pathPlanner->AddUserPoint(glm::vec2(697, 488));
	pathPlanner->AddUserPoint(glm::vec2(988, 517));
	pathPlanner->AddUserPoint(glm::vec2(1000, 100));*/
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
				nvgHelper->DrawStartFlag(*pathPlanner->GetStartPoint());
				nvgHelper->DrawArrow(*pathPlanner->GetStartPoint(), direction);
			}
			else if (mouseInsideGLHost)
			{
				nvgHelper->DrawStartFlag(positionOnMap);
			}
		}
		else
		{
			if (pathPlanner->GetStartPoint() != NULL)
			{
				nvgHelper->DrawStartFlag(*pathPlanner->GetStartPoint());
				nvgHelper->DrawArrow(*pathPlanner->GetStartPoint(), *pathPlanner->GetStartDirection());
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
			if (pathPlanner->GetSetEndDirection())
			{
				glm::vec2 direction = GeometryHelper::GetLineDirection(*pathPlanner->GetEndPoint(), positionOnMap);
				nvgHelper->DrawEndFlag(*pathPlanner->GetEndPoint());
				nvgHelper->DrawArrow(*pathPlanner->GetEndPoint(), direction);
			}
			else if (mouseInsideGLHost)
			{
				nvgHelper->DrawEndFlag(positionOnMap);
			}
		}
		else
		{
			if (pathPlanner->GetEndPoint() != NULL)
			{
				nvgHelper->DrawEndFlag(*pathPlanner->GetEndPoint());
				nvgHelper->DrawArrow(*pathPlanner->GetEndPoint(), *pathPlanner->GetEndDirection());
			}
			else if (pathPlanner->GetEndParkingSpace() != NULL)
			{
				glm::vec2 position = pathPlanner->GetEndParkingSpace()->GetPosition();
				nvgHelper->DrawEndFlag(position);
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
}

#pragma endregion