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
	NvgDrawHelper(NVGcontext *vg, glm::vec2 *widgetSize, glm::vec2 *offset, glm::vec2 *maxOffset, float *magnificationRatio);

	void DrawMap(Map *map);
	void DrawVehicle(Vehicle *vehicle);
	void DrawPath(Path *path);
	void DrawGraph(Graph * g);

	const int SELECTED_MARKER_SIZE = 5;
	const NVGcolor SELECTED_MARKER_COLOR = nvgRGBA(255, 0, 0, 255);

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

	const int BUILDING_BORDER_WIDTH = 3;
	const NVGcolor BUILDING_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor BUILDING_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);

	const int DECORATION_BORDER_WIDTH = 3;
	const NVGcolor DECORATION_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor DECORATION_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);

	const int PARKING_SPACE_BORDER_WIDTH = 3;
	const NVGcolor PARKING_SPACE_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor PARKING_SPACE_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);

	const int ROAD_BORDER_WIDTH = 3;
	const NVGcolor ROAD_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor ROAD_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);

	const int GRAPH_EDGE_WIDTH = 3;
	const NVGcolor GRAPH_EDGE_COLOR = nvgRGBA(255, 255, 140, 255);

	const int GRAPH_VERTEX_RADIUS = 3;
	const NVGcolor GRAPH_VERTEX_COLOR = nvgRGBA(140, 140, 140, 255);

	const int PATH_LINE_WIDTH = 3;
	const NVGcolor PATH_LINE_COLOR = nvgRGBA(140, 0, 140, 255);

	const int PATH_CIRCLE_WIDTH = 3;
	const NVGcolor PATH_CIRCLE_COLOR = nvgRGBA(255, 255, 0, 255);
private:
	NVGcontext *vg;
	glm::vec2 *widgetSize;
	glm::vec2 *offset;
	glm::vec2 *maxOffset;
	float *magnificationRatio;

	glm::vec2 drawAreaSize; // draw area is a map
	glm::vec2 drawAreaPosition;

	void updateDrawAreaProperties();
	void drawMapElements(std::vector<MapElement*> mapElements);
	void drawObstacle(Obstacle *obstacle);
	void drawParkingSpace(ParkingSpace *parkingSpace);
	void drawRoad(Road *road);
	void drawLine(Line *line);
	void drawCircle(Circle *circle);
};

#endif