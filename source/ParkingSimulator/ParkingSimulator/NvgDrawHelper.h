#ifndef NVG_DRAW_HELPER_H
#define NVG_DRAW_HELPER_H

#include "Map.h"
#include "MapElement.h"
#include "Vehicle.h"
#include "Path.h"
#include "Graph.h"
#include "Road.h"
#include "Line.h"
#include "Arc.h"
#include "Simulation.h"
#include "BezierCurve.h"
#include "BSpline.h"
#include <glm.hpp>

#define GLEW_STATIC
#include "glew.h"
#include <QTime>
#include <QEvent>
#include <qdesktopwidget.h>
#include <QMouseEvent> 
#include <QOpenGLWidget>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

class NvgDrawHelper
{
public:
	NvgDrawHelper(NVGcontext *vg, glm::vec2 *widgetSize, glm::vec2 *offset, glm::vec2 *maxOffset, glm::vec2 *drawAreaSize, glm::vec2 *drawAreaPosition, float *magnificationRatio);

	void DrawMap(Map *map);
	void DrawVehicle(Vehicle *vehicle);
	void DrawPath(Path *path);
	void DrawGraph(Graph * g);
	void DrawSimulationFrame(Simulation *simulation);
	void DrawPolygon(std::vector<glm::vec2> polygon, bool fill = false);
	void DrawActiveElement(MapElement *mapElement, bool admissible);
	void DrawHoverElement(MapElement *mapElement);
	void DrawSelectedElement(MapElement *mapElement);
	void DrawStartFlag(glm::vec2 position);
	void DrawEndFlag(glm::vec2 position);
	void DrawArrow(glm::vec2 point, glm::vec2 direction);
	void DrawBSpline(BSpline *bSpline, bool drawPolyline = false);
	void DrawPathElement(PathElement *pathElement);

	const int SELECTED_MARKER_SIZE = 5;
	const NVGcolor SELECTED_MARKER_COLOR = nvgRGBA(255, 0, 0, 255);

	const NVGcolor START_FLAG_COLOR = nvgRGBA(0, 255, 0, 255);
	const NVGcolor END_FLAG_COLOR = nvgRGBA(255, 0, 0, 255);

	const int ACTIVE_BORDER_WIDTH = 3;
	const NVGcolor ACTIVE_GOOD_COLOR = nvgRGBA(135, 255, 145, 255);
	const NVGcolor ACTIVE_GOOD_BORDER_COLOR = nvgRGBA(0, 145, 0, 255);
	const NVGcolor ACTIVE_BAD_COLOR = nvgRGBA(255, 125, 125, 255);
	const NVGcolor ACTIVE_BAD_BORDER_COLOR = nvgRGBA(175, 0, 0, 255);

	const int EXTRA_OFFSET = 30;
	const float MIN_MAGNIFICATION_RATIO = 0.2;
	const float MAX_MAGNIFICATION_RATIO = 5.0;

	const int MAP_BORDER_WIDTH = 3;
	const NVGcolor MAP_COLOR = nvgRGBA(58, 124, 37, 255);
	const NVGcolor MAP_BORDER_COLOR = nvgRGBA(255, 0, 0, 255);

	const int VEHICLE_BORDER_WIDTH = 3;
	const NVGcolor VEHICLE_COLOR = nvgRGBA(8, 73, 171, 255);
	const NVGcolor VEHICLE_BORDER_COLOR = nvgRGBA(255, 0, 0, 255);

	const int MAP_ELEMENT_HOVER_BORDER_WIDTH = 3;
	const NVGcolor MAP_ELEMENT_HOVER_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor MAP_ELEMENT_HOVER_BORDER_COLOR = nvgRGBA(255, 255, 0, 255);

	const int MAP_ELEMENT_SELECTED_BORDER_WIDTH = 3;
	const NVGcolor MAP_ELEMENT_SELECTED_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor MAP_ELEMENT_SELECTED_BORDER_COLOR = nvgRGBA(255, 255, 0, 255);

	const int BUILDING_BORDER_WIDTH = 3;
	const NVGcolor BUILDING_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor BUILDING_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);

	const int DECORATION_BORDER_WIDTH = 3;
	const NVGcolor DECORATION_COLOR = nvgRGBA(51, 153, 255, 255);
	const NVGcolor DECORATION_BORDER_COLOR = nvgRGBA(153, 153, 255, 255);

	const int PARKING_SPACE_BORDER_WIDTH = 3;
	const NVGcolor PARKING_SPACE_COLOR = nvgRGBA(153, 51, 102, 255);
	const NVGcolor PARKING_SPACE_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);

	const int ROAD_BORDER_WIDTH = 3;
	const NVGcolor ROAD_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor ROAD_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);

	const int GRAPH_EDGE_WIDTH = 2;
	const NVGcolor GRAPH_EDGE_COLOR = nvgRGBA(255, 255, 140, 255);

	const int GRAPH_VERTEX_RADIUS = 3;
	const NVGcolor GRAPH_VERTEX_COLOR = nvgRGBA(140, 140, 140, 255);

	const int PATH_LINE_WIDTH = 2;
	const NVGcolor PATH_LINE_COLOR = nvgRGBA(0, 0, 255, 255);

	const int PATH_CIRCLE_WIDTH = 2;
	const NVGcolor PATH_CIRCLE_COLOR = nvgRGBA(255, 0, 255, 255);

	const int POLYGON_BORDER_WIDTH = 3;
	const NVGcolor POLYGON_COLOR = nvgRGBA(255, 255, 0, 255);
	const NVGcolor POLYGON_BORDER_COLOR = nvgRGBA(255, 255, 0, 255);

	const int TRANSFORM_SHAPE_MOVE_RADIUS = 4;
	const int TRANSFORM_SHAPE_ROTATE_RADIUS = 20;
	const int TRANSFORM_SHAPE_ROTATE_WIDTH = 3;
	const int TRANSFORM_SHAPE_RESIZE_RADIUS = 4;
	const NVGcolor TRANSFORM_SHAPE_MOVE_COLOR = nvgRGBA(255, 0, 0, 255);
	const NVGcolor TRANSFORM_SHAPE_ROTATE_COLOR = nvgRGBA(255, 0, 0, 255);
	const NVGcolor TRANSFORM_SHAPE_RESIZE_COLOR = nvgRGBA(255, 0, 0, 255);
	const NVGcolor TRANSFORM_SHAPE_HOVER_COLOR = nvgRGBA(255, 255, 0, 255);

private:
	NVGcontext *vg;
	glm::vec2 *widgetSize;
	glm::vec2 *offset;
	glm::vec2 *maxOffset;
	glm::vec2 *drawAreaSize; // draw area is a map
	glm::vec2 *drawAreaPosition;
	float *magnificationRatio;

	void updateDrawAreaProperties();
	void drawMapElements(std::vector<MapElement*> mapElements);
	void drawObstacle(Obstacle *obstacle);
	void drawParkingSpace(ParkingSpace *parkingSpace);
	void drawRoad(Road *road);
	void drawLine(Line *line);
	void drawCircle(Circle *circle);
	void drawResizeShape(MapElement *mapElement);
	void drawMoveShape(MapElement *mapElement);
	void drawRotateShape(MapElement *mapElement);
	void drawTransformShapes(MapElement *mapElement);
};

#endif