#include "MapEditorGLHost.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

#include "PathPlanner.h"

Graph g;



Map *mapa = new Map(1000, 550);
Vehicle *vehicle = new Vehicle(100, 50);

PathPlanner pathPlanner(*mapa, *vehicle);

Path *path = new Path();
MapElement *parkingSpace = new ParkingSpace(glm::vec2(200, 200), glm::vec2(300, 120), ParkingSpaceType::Paralell);
Simulation *simulation = new Simulation(mapa, vehicle, path);

MapEditorGLHost::MapEditorGLHost(QWidget *parent) : OpenGLHost(parent) 
{ 
	
}

#pragma region User input events.

void MapEditorGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	if (mapEditor->GetAddBuilding())
	{
		mapEditor->AddObstacleConfirm();
	}
	else if (mapEditor->GetAddDecoration())
	{

	}
	else if (mapEditor->GetAddParkPlace())
	{

	}
	else if (mapEditor->GetAddRoad())
	{

	}
}

void MapEditorGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	OpenGLHost::mouseReleaseEvent(event);
}

void MapEditorGLHost::mouseMoveEvent(QMouseEvent * event)
{
	OpenGLHost::mouseMoveEvent(event);

	if (mapEditor->GetAddBuilding() && mapEditor->GetNewElement() != nullptr)
	{
		positionOnMap = (glm::vec2(mouseLastX, mouseLastY) - drawAreaPosition - widgetOffset) / magnificationRatio;

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

	//drawMap(&mapEditor->GetMap());
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
}

void MapEditorGLHost::resizeGL(int w, int h)
{
	OpenGLHost::resizeGL(w, h);

	//drawMap(&mapEditor->GetMap());
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

/*void MapEditorGLHost::adjustMaxOffset()
{
	if (offsetX > maxOffsetX + EXTRA_OFFSET)
		offsetX = maxOffsetX + EXTRA_OFFSET;
	if (offsetX < -maxOffsetX - EXTRA_OFFSET)
		offsetX = -maxOffsetX - EXTRA_OFFSET;
	if (offsetY > maxOffsetY + EXTRA_OFFSET)
		offsetY = maxOffsetY + EXTRA_OFFSET;
	if (offsetY < -maxOffsetY - EXTRA_OFFSET)
		offsetY = -maxOffsetY - EXTRA_OFFSET;
}*/

void MapEditorGLHost::nvgRenderFrame()
{
	nvgHelper->DrawMap(mapEditor->GetMap());

	//drawActiveElement();

	if (mapEditor->GetNewElement() != nullptr)
	{
		bool admissible = mapEditor->IsMapElementAdmissible(mapEditor->GetNewElement());
		nvgHelper->DrawActiveElement(mapEditor->GetNewElement(), admissible);
	}
}

//void MapEditorGLHost::drawActiveElement()
//{
//	if (mapEditor->GetNewElement() == nullptr)
//		return;
//
//	std::vector<glm::vec2> points = mapEditor->GetNewElement()->GetPoints();
//	
//	glm::vec2 elementPosition = drawAreaPosition + mapEditor->GetNewElement()->GetPosition() * magnificationRatio + widgetOffset;
//
//	nvgBeginPath(vg);
//	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + widgetOffset.x, drawAreaPosition.y + points[0].y * magnificationRatio + widgetOffset.y);
//	for (int i = 0; i <= points.size(); i++)
//		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + widgetOffset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + widgetOffset.y);
//	nvgStrokeColor(vg, admissible ? ACTIVE_GOOD_BORDER_COLOR : ACTIVE_BAD_BORDER_COLOR);
//	nvgStrokeWidth(vg, ACTIVE_BORDER_WIDTH);
//	nvgStroke(vg);
//	nvgFillColor(vg, admissible ? ACTIVE_GOOD_COLOR : ACTIVE_BAD_COLOR);
//	nvgFill(vg);
//
//	nvgBeginPath(vg);
//	nvgEllipse(vg, elementPosition.x, elementPosition.y, SELECTED_MARKER_SIZE, SELECTED_MARKER_SIZE);
//	nvgFillColor(vg, SELECTED_MARKER_COLOR);
//	nvgFill(vg);
//}

#pragma endregion