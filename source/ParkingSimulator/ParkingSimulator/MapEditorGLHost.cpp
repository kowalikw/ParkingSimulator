#include "MapEditorGLHost.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

MapEditorGLHost::MapEditorGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void MapEditorGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);
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

	if (mapEditor->GetAddBuilding())
	{
		int positionOnMapX = mouseLastX - mapPositionX - offsetX;
		if (positionOnMapX > mapWidth) positionOnMapX = -1;
		if(positionOnMapX > 0) positionOnMapX /= magnificationRatio;

		int positionOnMapY = mouseLastY - mapPositionY - offsetY;
		if (positionOnMapY > mapHeight) positionOnMapY = -1;
		if (positionOnMapY > 0) positionOnMapY /= magnificationRatio;

		if (positionOnMapX < 0 || positionOnMapY < 0)
			positionOnMap = glm::vec2(-1, -1);
		else
			positionOnMap = glm::vec2(positionOnMapX, positionOnMapY);

		std::ostringstream ss;
		ss << "X: " << positionOnMap.x << endl;
		ss << "Y: " << positionOnMap.y << endl;
		ss << endl;
		std::string s(ss.str());

		OutputDebugStringA(s.c_str());
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

	// to test only
	currentElement = new Obstacle(glm::vec2(5, 5), glm::vec2(500, 500), ObstacleType::Building, "Budynek");
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

	drawBuilding(currentElement);
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

void MapEditorGLHost::drawBuilding(MapElement *building)
{
	int buildingWidth = building->GetSize().x * magnificationRatio;
	int buildingHeight = building->GetSize().y * magnificationRatio;
	int buildingPositionX = mapPositionX + building->GetPosition().x * magnificationRatio + offsetX;
	int buildingPositionY = mapPositionY + building->GetPosition().y * magnificationRatio + offsetY;

	nvgBeginPath(vg);
	nvgRect(vg, buildingPositionX, buildingPositionY, buildingWidth + 2 * BUILDING_BORDER_WIDTH, buildingHeight + 2 * BUILDING_BORDER_WIDTH);
	nvgFillColor(vg, BUILDING_BORDER_COLOR);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, buildingPositionX + BUILDING_BORDER_WIDTH, buildingPositionY + BUILDING_BORDER_WIDTH, buildingWidth, buildingHeight);
	nvgFillColor(vg, BUILDING_COLOR);
	nvgFill(vg);
}

void MapEditorGLHost::drawRoad()
{
}

void MapEditorGLHost::drawDecoration(MapElement *decoration)
{
}

#pragma endregion