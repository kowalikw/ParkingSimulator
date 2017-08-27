#ifndef NVG_DRAW_HELPER_H
#define NVG_DRAW_HELPER_H

#include "Map.h"
#include "MapElement.h"
#include "Vehicle.h"
#include "Path.h"
#include <glm.hpp>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

class NvgDrawHelper
{
public:
	void DrawMap(NVGcontext* vg, Map *map, glm::vec2 mapPosition, glm::vec2 offset, float magnificationRatio);
	void DrawVehicle(NVGcontext* vg, Vehicle *vehicle, glm::vec2 mapPosition, glm::vec2 offset, float magnificationRatio);
	void DrawPath(NVGcontext* vg, Path *path, glm::vec2 mapPosition, glm::vec2 offset, float magnificationRatio);
private:
	void drawMapElements(NVGcontext* vg, std::vector<MapElement*> *mapElements, glm::vec2 mapPosition, glm::vec2 offset, float magnificationRatio);
	void drawObstacle(NVGcontext* vg, Obstacle *obstacle, glm::vec2 mapPosition, glm::vec2 offset, float magnificationRatio);
	void drawParkingSpace(NVGcontext* vg, ParkingSpace *parkingSpace, glm::vec2 mapPosition, glm::vec2 offset, float magnificationRatio);
};

#endif