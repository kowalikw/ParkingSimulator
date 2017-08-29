#include "VisualisationGLHost.h"

VisualisationGLHost::VisualisationGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void VisualisationGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);
}

void VisualisationGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	OpenGLHost::mouseReleaseEvent(event);
}

void VisualisationGLHost::mouseMoveEvent(QMouseEvent * event)
{
	OpenGLHost::mouseMoveEvent(event);
}

void VisualisationGLHost::keyPressEvent(QKeyEvent * event)
{
	OpenGLHost::keyPressEvent(event);
}

void VisualisationGLHost::keyReleaseEvent(QKeyEvent * event)
{
	OpenGLHost::keyReleaseEvent(event);
}

void VisualisationGLHost::SetVisualisation(Visualisation * visualisation)
{
	this->visualisation = visualisation;
}

Visualisation * VisualisationGLHost::GetVisualisation()
{
	return this->visualisation;
}

#pragma endregion

#pragma region OpenGL methods.

void VisualisationGLHost::initializeGL()
{
	OpenGLHost::initializeGL();
}

void VisualisationGLHost::resizeGL(int w, int h)
{
	OpenGLHost::resizeGL(w, h);
}

void VisualisationGLHost::paintGL()
{
	OpenGLHost::paintGL();

	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, widgetWidth, widgetHeight, pixelRatio);

	//nvgRenderFrame();
	

	nvgEndFrame(vg);
}

void VisualisationGLHost::nvgRenderFrame()
{
}

void VisualisationGLHost::drawVehicle(Vehicle vehicle)
{
	// pytanie co traktujemy jako pozycjê pojazdu... zak³adam póki co ¿e œrodek tylnej osi

	auto position = vehicle.GetPosition();

	nvgBeginPath(vg);
	
	nvgMoveTo(vg, position.x, position.y);
	nvgLineTo(vg, position.x + vehicle.dirTrack.x * (vehicle.track / 2.0), vehicle.dirTrack.y * position.y);
	nvgLineTo(vg, position.x - vehicle.dirTrack.x * (vehicle.track / 2.0), vehicle.dirTrack.y * position.y);
	nvgLineTo(vg, position.x, position.y);
	nvgLineTo(vg, position.x, position.y);
	nvgLineTo(vg, position.x, position.y);

	nvgStrokeColor(vg, nvgRGBA(135, 255, 145, 255));
	nvgStroke(vg);
	nvgFill(vg);


}

void VisualisationGLHost::drawPath(Path path)
{
}

void VisualisationGLHost::drawMap(Map map)
{
	/*

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
	nvgFill(vg);*/
}

void VisualisationGLHost::drawMapElement(MapElement *building)
{
	/*if (building == NULL) return;

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
	}*/
}

#pragma endregion