#ifndef NVG_DRAW_HELPER_H
#define NVG_DRAW_HELPER_H

#include "Map.h"
#include "MapElement.h"
#include "Vehicle.h"
#include "Path.h"
#include "Graph.h"
#include "Line.h"
#include "Arc.h"
#include "Simulation.h"
#include "BezierCurve.h"
#include "BSpline.h"
#include <glm.hpp>
#include "Settings.h"

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

enum VehicleType
{
	VehicleStandard,
	VehicleStart,
	VehicleEnd,
	VehicleError
};

class NvgDrawHelper
{
public:
	NvgDrawHelper(NVGcontext *vg, glm::vec2 *widgetSize, glm::vec2 *offset, glm::vec2 *maxOffset, glm::vec2 *drawAreaSize, glm::vec2 *drawAreaPosition, float *magnificationRatio);

	void DrawMap(Map *map);
	void DrawVehicle(Vehicle *vehicle, VehicleType type = VehicleType::VehicleStandard);
	void DrawPath(Path *path);
	void DrawGraph(Graph * g);
	void DrawSimulationFrame(Simulation *simulation);
	void DrawPolygon(std::vector<glm::vec2> polygon, bool fill = false);
	void DrawActiveElement(MapElement *mapElement, bool admissible);
	void DrawHoverElement(MapElement *mapElement);
	void DrawSelectedElement(MapElement *mapElement);
	void DrawStartFlag(Vehicle *vehicle, bool isAdmissible = false);
	void DrawEndFlag(Vehicle *vehicle, bool isAdmissible = false);
	void DrawArrow(glm::vec2 point, glm::vec2 direction);
	void DrawPathElement(PathElement *pathElement, bool isSelected = false);
	void DrawMeshOnMap(Map *map, int width = 3);
	void DrawTerrain(Terrain *terrain, bool isHover = false);

	const int SELECTED_MARKER_SIZE = 5;
	const NVGcolor SELECTED_MARKER_COLOR = nvgRGBA(255, 0, 0, 255);

	const NVGcolor START_FLAG_COLOR = nvgRGBA(0, 255, 0, 255);
	const NVGcolor END_FLAG_COLOR = nvgRGBA(255, 0, 0, 255);

	int ACTIVE_BORDER_WIDTH = Settings::getInstance()->GetInt("ACTIVE_BORDER_WIDTH");
	NVGcolor ACTIVE_GOOD_COLOR = Settings::getInstance()->GetColor("ACTIVE_GOOD_COLOR");
	NVGcolor ACTIVE_GOOD_BORDER_COLOR = Settings::getInstance()->GetColor("ACTIVE_GOOD_BORDER_COLOR");
	NVGcolor ACTIVE_BAD_COLOR = Settings::getInstance()->GetColor("ACTIVE_BAD_COLOR");
	NVGcolor ACTIVE_BAD_BORDER_COLOR = Settings::getInstance()->GetColor("ACTIVE_BAD_BORDER_COLOR");

	int EXTRA_OFFSET = 30;
	float MIN_MAGNIFICATION_RATIO = 0.2;
	float MAX_MAGNIFICATION_RATIO = 5.0;

	int MAP_BORDER_WIDTH = Settings::getInstance()->GetInt("MAP_BORDER_WIDTH");
	NVGcolor MAP_COLOR = Settings::getInstance()->GetColor("MAP_COLOR");
	NVGcolor MAP_BORDER_COLOR = Settings::getInstance()->GetColor("MAP_BORDER_COLOR");

	 int MAP_ELEMENT_HOVER_BORDER_WIDTH = Settings::getInstance()->GetInt("MAP_ELEMENT_HOVER_BORDER_WIDTH");
	 NVGcolor MAP_ELEMENT_HOVER_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_HOVER_COLOR");
	 NVGcolor MAP_ELEMENT_HOVER_BORDER_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_HOVER_BORDER_COLOR");

	 int MAP_ELEMENT_SELECTED_BORDER_WIDTH = Settings::getInstance()->GetInt("MAP_ELEMENT_SELECTED_BORDER_WIDTH");
	 NVGcolor MAP_ELEMENT_SELECTED_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_SELECTED_COLOR");
	 NVGcolor MAP_ELEMENT_SELECTED_BORDER_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_SELECTED_BORDER_COLOR");

	 int BUILDING_BORDER_WIDTH = Settings::getInstance()->GetInt("BUILDING_BORDER_WIDTH");
	 NVGcolor BUILDING_COLOR = Settings::getInstance()->GetColor("BUILDING_COLOR");
	 NVGcolor BUILDING_BORDER_COLOR = Settings::getInstance()->GetColor("BUILDING_BORDER_COLOR");

	 int DECORATION_BORDER_WIDTH = Settings::getInstance()->GetInt("DECORATION_BORDER_WIDTH");
	 NVGcolor DECORATION_COLOR = Settings::getInstance()->GetColor("DECORATION_COLOR");
	 NVGcolor DECORATION_BORDER_COLOR = Settings::getInstance()->GetColor("DECORATION_BORDER_COLOR");

	 int PARKING_SPACE_BORDER_WIDTH = Settings::getInstance()->GetInt("PARKING_SPACE_BORDER_WIDTH");
	 NVGcolor PARKING_SPACE_COLOR = Settings::getInstance()->GetColor("PARKING_SPACE_COLOR");
	 NVGcolor PARKING_SPACE_BORDER_COLOR = Settings::getInstance()->GetColor("PARKING_SPACE_BORDER_COLOR");

	 int VEHICLE_BORDER_WIDTH = Settings::getInstance()->GetInt("VEHICLE_BORDER_WIDTH");
	 NVGcolor VEHICLE_COLOR = Settings::getInstance()->GetColor("VEHICLE_COLOR");
	 NVGcolor VEHICLE_BORDER_COLOR = Settings::getInstance()->GetColor("VEHICLE_BORDER_COLOR");

	 int GRAPH_EDGE_WIDTH = Settings::getInstance()->GetInt("GRAPH_EDGE_WIDTH");
	 NVGcolor GRAPH_EDGE_COLOR = Settings::getInstance()->GetColor("GRAPH_EDGE_COLOR");

	int GRAPH_VERTEX_RADIUS = Settings::getInstance()->GetInt("GRAPH_VERTEX_RADIUS");
	NVGcolor GRAPH_VERTEX_COLOR = Settings::getInstance()->GetColor("GRAPH_VERTEX_COLOR");

	int PATH_LINE_WIDTH = Settings::getInstance()->GetInt("PATH_LINE_WIDTH");
	NVGcolor PATH_LINE_COLOR = Settings::getInstance()->GetColor("PATH_LINE_COLOR");

	int PATH_CIRCLE_WIDTH = Settings::getInstance()->GetInt("PATH_CIRCLE_WIDTH");
	NVGcolor PATH_CIRCLE_COLOR = Settings::getInstance()->GetColor("PATH_CIRCLE_COLOR");

	int PATH_BSPLINE_WIDTH = Settings::getInstance()->GetInt("PATH_BSPLINE_WIDTH");
	NVGcolor PATH_BSPLINE_COLOR = Settings::getInstance()->GetColor("PATH_BSPLINE_COLOR");

	int PATH_ELEMENT_SELECTED_WIDTH = Settings::getInstance()->GetInt("PATH_ELEMENT_SELECTED_WIDTH");
	NVGcolor PATH_ELEMENT_SELECTED_COLOR = Settings::getInstance()->GetColor("PATH_ELEMENT_SELECTED_COLOR");

	int POLYGON_BORDER_WIDTH = Settings::getInstance()->GetInt("POLYGON_BORDER_WIDTH");
	NVGcolor POLYGON_COLOR = Settings::getInstance()->GetColor("POLYGON_COLOR");
	NVGcolor POLYGON_BORDER_COLOR = Settings::getInstance()->GetColor("POLYGON_BORDER_COLOR");

	int TRANSFORM_SHAPE_MOVE_RADIUS = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_MOVE_RADIUS");
	int TRANSFORM_SHAPE_ROTATE_RADIUS = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_ROTATE_RADIUS");
	int TRANSFORM_SHAPE_ROTATE_WIDTH = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_ROTATE_WIDTH");
	int TRANSFORM_SHAPE_RESIZE_RADIUS = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_RESIZE_RADIUS");
	NVGcolor TRANSFORM_SHAPE_MOVE_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_MOVE_COLOR");
	NVGcolor TRANSFORM_SHAPE_ROTATE_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_ROTATE_COLOR");
	NVGcolor TRANSFORM_SHAPE_RESIZE_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_RESIZE_COLOR");
	NVGcolor TRANSFORM_SHAPE_HOVER_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_HOVER_COLOR");

	NVGcolor TERRAIN_HOVER_COLOR = Settings::getInstance()->GetColor("TERRAIN_HOVER_COLOR");

	int meshOnMapSize = Settings::getInstance()->GetInt("TERRAIN_SLICE_SIZE");

private:
	NVGcontext *vg;
	glm::vec2 *widgetSize;
	glm::vec2 *offset;
	glm::vec2 *maxOffset;
	glm::vec2 *drawAreaSize; // draw area is a map
	glm::vec2 *drawAreaPosition;
	float *magnificationRatio;
	int vehicleImage;
	int vehicleStartImage;
	int vehicleEndImage;
	int VehicleErrorImage;

	void updateDrawAreaProperties();
	void drawMapElements(std::vector<MapElement*> mapElements);
	void drawObstacle(Obstacle *obstacle);
	void drawVehicle(Vehicle *vehicle);
	void drawParkingSpace(ParkingSpace *parkingSpace);
	void drawLine(Line *line, bool isSelected = false);
	void drawCircle(Circle *circle, bool isSelected = false);
	void drawBSpline(BSpline *bSpline, bool isSelected = false, bool drawPolyline = false);
	void drawResizeShape(MapElement *mapElement);
	void drawMoveShape(MapElement *mapElement);
	void drawRotateShape(MapElement *mapElement);
	void drawTransformShapes(MapElement *mapElement);
};

#endif