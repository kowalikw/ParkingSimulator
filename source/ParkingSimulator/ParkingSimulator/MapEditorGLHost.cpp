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

	g.AddVertex(100, 100);
	g.AddVertex(200, 300);
	g.AddVertex(300, 500);
	g.AddVertex(400, 100);
	g.AddVertex(150, 200);
	g.AddVertex(805, 200);
	g.AddVertex(650, 400);
	g.AddVertex(130, 100);

	for (int i = 0; i < g.VerticesCount(); i++)
		for (int j = 0; j < g.VerticesCount(); j++)
			if (i == j) continue;
			else
			{
				g.AddEdge(i, j);
			}

	g.RemoveEdge(0, 5);
	g.RemoveEdge(0, 3);

	double **estimated = new double*[g.VerticesCount()];
	for (int i = 0; i < g.VerticesCount(); i++)
		estimated[i] = new double[g.VerticesCount()];

	for (int i = 0; i < g.VerticesCount(); i++)
		for (int j = 0; j < g.VerticesCount(); j++)
			estimated[i][j] = 999;

	auto path = g.FindPath(0, 5, estimated);

	mapa->AddMapElement(parkingSpace);
	vehicle->UpdateState(glm::vec2(500, 500), 0);

	auto pS = dynamic_cast<ParkingSpace*>(parkingSpace);

	Path *p = pathPlanner.createParkingPath(*vehicle, *pS);

	pathPlanner.AddUserPoint(glm::vec2(200, 300));
	pathPlanner.AddUserPoint(glm::vec2(200, 500));
	pathPlanner.AddUserPoint(glm::vec2(500, 500));
	pathPlanner.AddUserPoint(glm::vec2(500, 300));

	pathPlanner.AddUserPoint(glm::vec2(700, 300));
	pathPlanner.AddUserPoint(glm::vec2(300, 100));

	pathPlanner.AddUserPoint(glm::vec2(100, 100));

	pathPlanner.AddUserPoint(glm::vec2(100, 300));
	pathPlanner.AddUserPoint(glm::vec2(310, 300));

	pathPlanner.AddUserPoint(glm::vec2(305, 171));

	pathPlanner.AddUserPoint(glm::vec2(678, 186));
	pathPlanner.AddUserPoint(glm::vec2(697, 488));
	pathPlanner.AddUserPoint(glm::vec2(988, 517));
	pathPlanner.AddUserPoint(glm::vec2(1000, 100));

	Path *p2 = pathPlanner.CreateAdmissiblePath(pathPlanner.UserPoints());

	simulation->SetPath(p2);
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
	nvgHelper->DrawMap(simulation->GetMap());

	nvgHelper->DrawVehicle(simulation->GetVehicle());

	//nvgHelper->DrawPath(simulation->GetPath());

	nvgHelper->DrawGraph(&g);

	drawActiveElement();
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

#pragma endregion