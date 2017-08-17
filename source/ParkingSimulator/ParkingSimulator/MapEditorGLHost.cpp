#include "MapEditorGLHost.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

#include "Graph.h"

Graph g;

MapEditorGLHost::MapEditorGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void MapEditorGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	if (mapEditor->GetAddBuilding())
	{
		mapEditor->AddObstacleConfirm();
		g.CreateVoronoiGraph(&mapEditor->GetMap());
	}
}

void MapEditorGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	OpenGLHost::mouseReleaseEvent(event);
}

void MapEditorGLHost::mouseMoveEvent(QMouseEvent * event)
{
	OpenGLHost::mouseMoveEvent(event);

	if (mouseMiddlePressed)
	{
		offsetX += mouseOffsetX / 2.0;
		offsetY += mouseOffsetY / 2.0;

		adjustMaxOffset();
	}

	if (mapEditor->GetAddBuilding() && mapEditor->GetNewElement() != nullptr)
	{
		int positionOnMapX = mouseLastX - mapPositionX - offsetX;
		if(positionOnMapX > 0) positionOnMapX /= magnificationRatio;

		int positionOnMapY = mouseLastY - mapPositionY - offsetY;
		if (positionOnMapY > 0) positionOnMapY /= magnificationRatio;

		positionOnMap = glm::vec2(positionOnMapX, positionOnMapY);

		mapEditor->GetNewElement()->SetPosition(positionOnMap);

		/*std::ostringstream ss;
		ss << "X: " << positionOnMap.x << endl;
		ss << "Y: " << positionOnMap.y << endl;
		ss << endl;
		std::string s(ss.str());

		OutputDebugStringA(s.c_str());*/
	}
}

void MapEditorGLHost::wheelEvent(QWheelEvent * event)
{
	OpenGLHost::wheelEvent(event);

	magnificationRatio += mouseWheelSteps / 20.0f;

	if (magnificationRatio < 0.05f)
		magnificationRatio = 0.05f;

	drawMap();
	adjustMaxOffset();
}

void MapEditorGLHost::keyPressEvent(QKeyEvent * event)
{
	OpenGLHost::keyPressEvent(event);
}

void MapEditorGLHost::keyReleaseEvent(QKeyEvent * event)
{
	OpenGLHost::keyReleaseEvent(event);
}

void MapEditorGLHost::SetMapEditor(MapEditor * mapEditor)
{
	this->mapEditor = mapEditor;
}

MapEditor * MapEditorGLHost::GetMapEditor()
{
	return this->mapEditor;
}

#pragma endregion

#pragma region OpenGL methods.

void MapEditorGLHost::initializeGL()
{
	OpenGLHost::initializeGL();

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
}

void MapEditorGLHost::resizeGL(int w, int h)
{
	OpenGLHost::resizeGL(w, h);

	drawMap();
	adjustMaxOffset();
}

void MapEditorGLHost::paintGL()
{
	OpenGLHost::paintGL();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, widgetWidth, widgetHeight, pixelRatio);

	nvgRenderFrame();

	nvgEndFrame(vg);
}

#pragma endregion

#pragma region Private methods.

void MapEditorGLHost::adjustMaxOffset()
{
	if (offsetX > maxOffsetX + EXTRA_OFFSET)
		offsetX = maxOffsetX + EXTRA_OFFSET;
	if (offsetX < -maxOffsetX - EXTRA_OFFSET)
		offsetX = -maxOffsetX - EXTRA_OFFSET;
	if (offsetY > maxOffsetY + EXTRA_OFFSET)
		offsetY = maxOffsetY + EXTRA_OFFSET;
	if (offsetY < -maxOffsetY - EXTRA_OFFSET)
		offsetY = -maxOffsetY - EXTRA_OFFSET;
}

void MapEditorGLHost::nvgRenderFrame()
{
	drawMap();

	std::vector<MapElement*> mapElements = mapEditor->GetMap().GetMapElements();
	for (int i = 0; i < mapElements.size(); i++)
	{
		if (dynamic_cast<Obstacle*>(mapElements[i]) != NULL)
		{
			auto obstacle = dynamic_cast<Obstacle*>(mapElements[i]);
			switch (obstacle->GetType())
			{
			case ObstacleType::Building:
				drawBuilding(obstacle, true);
				break;
			case ObstacleType::Decoration:

				break;
			}
		}
	}

	drawActiveElement();

	drawGraph(g);
}

void MapEditorGLHost::drawMap()
{
	mapWidth = mapEditor->GetMap().GetWidth() * magnificationRatio;
	mapHeight = mapEditor->GetMap().GetHeight() * magnificationRatio;
	mapPositionX = (widgetWidth - mapWidth - 2 * MAP_BORDER_WIDTH) / 2;
	mapPositionY = (widgetHeight - mapHeight - 2 * MAP_BORDER_WIDTH) / 2;

	if (mapWidth == 0 || mapHeight == 0) return;

	maxOffsetX = mapPositionX < 0 ? abs(mapPositionX) : 0;
	maxOffsetY = mapPositionY < 0 ? abs(mapPositionY) : 0;

	nvgBeginPath(vg);
	nvgRect(vg, mapPositionX + offsetX, mapPositionY + offsetY, mapWidth + 2 * MAP_BORDER_WIDTH, mapHeight + 2 * MAP_BORDER_WIDTH);
	nvgFillColor(vg, MAP_BORDER_COLOR);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, mapPositionX + offsetX + MAP_BORDER_WIDTH, mapPositionY + offsetY + MAP_BORDER_WIDTH, mapWidth, mapHeight);
	nvgFillColor(vg, MAP_COLOR);
	nvgFill(vg);
}

void MapEditorGLHost::drawBuilding(MapElement *building, bool selected)
{
	if (building == NULL) return;

	std::vector<glm::vec2> points = building->GetPoints();
	int buildingPositionX = mapPositionX + building->GetPosition().x * magnificationRatio + offsetX;
	int buildingPositionY = mapPositionY + building->GetPosition().y * magnificationRatio + offsetY;

	nvgBeginPath(vg);
	nvgMoveTo(vg, mapPositionX + points[0].x * magnificationRatio + offsetX, mapPositionY + points[0].y * magnificationRatio + offsetY);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, mapPositionX + points[i % points.size()].x * magnificationRatio + offsetX, mapPositionY + points[i % points.size()].y * magnificationRatio + offsetY);
	nvgStrokeColor(vg, BUILDING_BORDER_COLOR);
	nvgStrokeWidth(vg, BUILDING_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, BUILDING_COLOR);
	nvgFill(vg);

	if (selected)
	{
		nvgBeginPath(vg);
		nvgEllipse(vg, buildingPositionX, buildingPositionY, SELECTED_MARKER_SIZE, SELECTED_MARKER_SIZE);
		nvgFillColor(vg, SELECTED_MARKER_COLOR);
		nvgFill(vg);
	}
}

void MapEditorGLHost::drawRoad()
{
}

void MapEditorGLHost::drawDecoration(MapElement *decoration, bool selected)
{
}

void MapEditorGLHost::drawActiveElement()
{
	if (mapEditor->GetNewElement() == nullptr)
		return;

	std::vector<glm::vec2> points = mapEditor->GetNewElement()->GetPoints();
	bool admissible = mapEditor->IsMapElementAdmissible(mapEditor->GetNewElement());
	int buildingPositionX = mapPositionX + mapEditor->GetNewElement()->GetPosition().x * magnificationRatio + offsetX;
	int buildingPositionY = mapPositionY + mapEditor->GetNewElement()->GetPosition().y * magnificationRatio + offsetY;

	nvgBeginPath(vg);
	nvgMoveTo(vg, mapPositionX + points[0].x * magnificationRatio + offsetX, mapPositionY + points[0].y * magnificationRatio + offsetY);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, mapPositionX + points[i % points.size()].x * magnificationRatio + offsetX, mapPositionY + points[i % points.size()].y * magnificationRatio + offsetY);
	nvgStrokeColor(vg, admissible ? ACTIVE_GOOD_BORDER_COLOR : ACTIVE_BAD_BORDER_COLOR);
	nvgStrokeWidth(vg, ACTIVE_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, admissible ? ACTIVE_GOOD_COLOR : ACTIVE_BAD_COLOR);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgEllipse(vg, buildingPositionX, buildingPositionY, SELECTED_MARKER_SIZE, SELECTED_MARKER_SIZE);
	nvgFillColor(vg, SELECTED_MARKER_COLOR);
	nvgFill(vg);
}

void MapEditorGLHost::drawGraph(Graph g)
{
	for (int i = 0; i < g.VerticesCount(); i++)
	{
		for (int j = 0; j < g.VerticesCount(); j++)
		{
			if (g.GetEdge(i, j) == NULL) continue;

			nvgBeginPath(vg);
			nvgMoveTo(vg, g.GetEdge(i, j)->v1->x, g.GetEdge(i, j)->v1->y);
			nvgLineTo(vg, g.GetEdge(i, j)->v2->x, g.GetEdge(i, j)->v2->y);
			nvgStrokeColor(vg, nvgRGBA(255, 255, 0, 255));
			nvgStrokeWidth(vg, 3);
			nvgStroke(vg);
		}
	}

	for (int i = 0; i < g.VerticesCount(); i++)
	{
		nvgBeginPath(vg);
		nvgEllipse(vg, g.GetVertex(i)->x, g.GetVertex(i)->y, 3, 3);
		nvgFillColor(vg, nvgRGBA(0, 0, 255, 255));
		nvgFill(vg);
	}
}

#pragma endregion