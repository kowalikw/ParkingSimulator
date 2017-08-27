#include "MapEditorGLHost.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

#include "PathPlanner.h"

Graph g;

PathPlanner pathPlanner;

Map *mapa = new Map(1000, 550);
Vehicle *vehicle = new Vehicle(200, 100);

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

	drawMap(&mapEditor->GetMap());
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

	mapa->AddMapElement(parkingSpace);
	vehicle->UpdateState(glm::vec2(500, 500), -M_PI / 4.0);

	auto pS = dynamic_cast<ParkingSpace*>(parkingSpace);

	//Path p = pathPlanner.createParkingPath(*vehicle, *pS);
	//simulation->SetPath(&p);

	int lala2 = 0;
}

void MapEditorGLHost::resizeGL(int w, int h)
{
	OpenGLHost::resizeGL(w, h);

	drawMap(&mapEditor->GetMap());
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
	drawMap(simulation->GetMap());

	std::vector<MapElement*> mapElements = simulation->GetMap()->GetMapElements();
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
		else if (dynamic_cast<ParkingSpace*>(mapElements[i]) != NULL)
		{
			auto parkingSpace = dynamic_cast<ParkingSpace*>(mapElements[i]);
			drawBuilding(parkingSpace, true);
		}
	}

	nvgBeginPath(vg);
	nvgEllipse(vg, 500, 500, 8, 8);
	nvgFillColor(vg, nvgRGBA(255, 255, 0, 255));
	nvgFill(vg);

	drawVehicle(simulation->GetVehicle(), magnificationRatio);

	drawPath(simulation->GetPath());

	drawActiveElement();

	drawGraph(g);
}

void MapEditorGLHost::drawMap(Map *map)
{
	mapWidth = map->GetWidth() * magnificationRatio;
	mapHeight = map->GetHeight() * magnificationRatio;
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

void MapEditorGLHost::drawVehicle(Vehicle * vehicle, float magnificationRatio)
{
	auto position = vehicle->GetPosition();
	auto wheelbase = vehicle->GetWheelbase() * magnificationRatio;
	auto track = vehicle->GetTrack() * magnificationRatio;
	auto dirWheelbase = vehicle->GetDirWheelbase();
	auto dirTrack = vehicle->GetDirTrack();
	auto tireRadius = 25.0f * magnificationRatio;

	auto p0 = glm::vec2(mapPositionX + offsetX + position.x * magnificationRatio, mapPositionY + offsetY + position.y * magnificationRatio);
	auto p1 = p0 + (float)(wheelbase / 2.0) * dirWheelbase;
	auto p2 = p1 + (float)(track / 2.0) * dirTrack;
	auto p3 = p1 - (float)(track / 2.0) * dirTrack;
	auto p4 = p0 - (float)(wheelbase / 2.0) * dirWheelbase;
	auto p5 = p4 + (float)(track / 2.0) * dirTrack;
	auto p6 = p4 - (float)(track / 2.0) * dirTrack;

	auto p2a = p2 - (tireRadius * dirWheelbase);
	auto p2b = p2 + (tireRadius * dirWheelbase);
	auto p3a = p3 - (tireRadius * dirWheelbase);
	auto p3b = p3 + (tireRadius * dirWheelbase);
	auto p5a = p5 - (tireRadius * dirWheelbase);
	auto p5b = p5 + (tireRadius * dirWheelbase);
	auto p6a = p6 - (tireRadius * dirWheelbase);
	auto p6b = p6 + (tireRadius * dirWheelbase);

	nvgBeginPath(vg);
	
	nvgMoveTo(vg, p1.x, p1.y);
	nvgLineTo(vg, p4.x, p4.y);
	
	nvgMoveTo(vg, p2.x, p2.y);
	nvgLineTo(vg, p3.x, p3.y);

	nvgMoveTo(vg, p5.x, p5.y);
	nvgLineTo(vg, p6.x, p6.y);

	nvgStrokeColor(vg, MAP_BORDER_COLOR);
	nvgStroke(vg);

	//draw tires
	nvgBeginPath(vg);

	nvgMoveTo(vg, p2a.x, p2a.y);
	nvgLineTo(vg, p2b.x, p2b.y);

	nvgMoveTo(vg, p3a.x, p3a.y);
	nvgLineTo(vg, p3b.x, p3b.y);

	nvgMoveTo(vg, p5a.x, p5a.y);
	nvgLineTo(vg, p5b.x, p5b.y);

	nvgMoveTo(vg, p6a.x, p6a.y);
	nvgLineTo(vg, p6b.x, p6b.y);

	nvgStrokeWidth(vg, 5);
	nvgStrokeColor(vg, MAP_BORDER_COLOR);
	nvgStroke(vg);

	nvgBeginPath(vg);
	nvgEllipse(vg, position.x, position.y, SELECTED_MARKER_SIZE, SELECTED_MARKER_SIZE);
	nvgFillColor(vg, SELECTED_MARKER_COLOR);
	nvgFill(vg);
}

void MapEditorGLHost::drawPath(Path * path)
{
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