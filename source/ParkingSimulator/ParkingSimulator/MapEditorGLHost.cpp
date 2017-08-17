#include "MapEditorGLHost.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 


#include <boost/polygon/voronoi.hpp>
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;



namespace boost
{
	namespace polygon 
	{
		
	}
}

struct Point2 {
	int a;
	int b;
	Point2(int x, int y) : a(x), b(y) {}
};

struct Segment {
	Point2 p0;
	Point2 p1;
	Segment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
};

template <>
struct boost::polygon::geometry_concept<Point2> {
	typedef point_concept type;
};

template <>
struct boost::polygon::point_traits<Point2> {
	typedef int coordinate_type;

	static inline coordinate_type get(
		const Point2& point, boost::polygon::orientation_2d orient) {
		return (orient == boost::polygon::HORIZONTAL) ? point.a : point.b;
	}
};

template <>
struct boost::polygon::geometry_concept<Segment> {
	typedef segment_concept type;
};

template <>
struct boost::polygon::segment_traits<Segment> {
	typedef int coordinate_type;
	typedef Point2 point_type;

	static inline point_type get(const Segment& segment, boost::polygon::direction_1d dir) {
		return dir.to_int() ? segment.p1 : segment.p0;
	}
};

std::vector<Point2> points;
std::vector<Segment> segments;
voronoi_diagram<double> vd;

MapEditorGLHost::MapEditorGLHost(QWidget *parent) : OpenGLHost(parent) { }

#pragma region User input events.

void MapEditorGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	if (mapEditor->GetAddBuilding())
	{
		mapEditor->AddObstacleConfirm();

		segments.clear();

		auto mapElements = mapEditor->GetMap().GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
		{
			auto elementPoints = mapElements[i]->GetPoints();
			for (int j = 0; j < elementPoints.size(); j++)
			{
				segments.push_back(Segment(elementPoints[j].x, elementPoints[j].y, elementPoints[(j + 1) % elementPoints.size()].x, elementPoints[(j + 1) % elementPoints.size()].y));
			}
		}


		vd.clear();
		construct_voronoi(points.begin(), points.end(), segments.begin(), segments.end(), &vd);
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

	// voronoi test
	// Preparing Input Geometries.
	

	segments.push_back(Segment(0, 0, 0, 500));
	segments.push_back(Segment(0, 500, 600, 500));
	segments.push_back(Segment(600, 500, 600, 0));
	segments.push_back(Segment(600, 0, 0, 0));
	
	/*segments.push_back(Segment(100, 100, 200, 100));
	segments.push_back(Segment(200, 100, 200, 200));
	segments.push_back(Segment(200, 200, 100, 200));
	segments.push_back(Segment(100, 200, 100, 100));

	segments.push_back(Segment(300, 300, 300, 400));
	segments.push_back(Segment(300, 400, 400, 400));
	segments.push_back(Segment(400, 400, 400, 300));
	segments.push_back(Segment(400, 300, 300, 300));*/

	// Construction of the Voronoi Diagram.
	
	

	int lala = 0;
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

	/*for (int i = 0; i < segments.size(); i++)
	{
		nvgBeginPath(vg);
		nvgMoveTo(vg, segments[i].p0.a, segments[i].p0.b);
		nvgLineTo(vg, segments[i].p1.a, segments[i].p1.b);
		nvgStrokeColor(vg, MAP_BORDER_COLOR);
		nvgStrokeWidth(vg, 3);
		nvgStroke(vg);
	}*/

	
	{
		for (voronoi_diagram<double>::const_cell_iterator it = vd.cells().begin();
			it != vd.cells().end(); ++it) {
			const voronoi_diagram<double>::cell_type &cell = *it;
			const voronoi_diagram<double>::edge_type *edge = cell.incident_edge();
			// This is convenient way to iterate edges around Voronoi cell.
			do {
				//if (edge->is_primary())
				//	++result;

				if (edge != NULL && edge->vertex0() != NULL && edge->vertex1() != NULL)
				{
					nvgBeginPath(vg);
					nvgMoveTo(vg, edge->vertex0()->x(), edge->vertex0()->y());
					nvgLineTo(vg, edge->vertex1()->x(), edge->vertex1()->y());
					nvgStrokeColor(vg, MAP_BORDER_COLOR);
					nvgStrokeWidth(vg, 3);
					nvgStroke(vg);
				}

				edge = edge->next();
			} while (edge != cell.incident_edge());
		}
	}

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

#pragma endregion