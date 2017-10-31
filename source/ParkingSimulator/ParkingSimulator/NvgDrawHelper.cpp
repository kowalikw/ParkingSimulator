#include "NvgDrawHelper.h"

#pragma region Public methods.

NvgDrawHelper::NvgDrawHelper(NVGcontext * vg, glm::vec2 * widgetSize, glm::vec2 * offset, glm::vec2 * maxOffset, glm::vec2 * drawAreaSize, glm::vec2 * drawAreaPosition, float * magnificationRatio)
{
	this->vg = vg;
	this->widgetSize = widgetSize;
	this->offset = offset;
	this->maxOffset = maxOffset;
	this->drawAreaSize = drawAreaSize;
	this->drawAreaPosition = drawAreaPosition;
	this->magnificationRatio = magnificationRatio;
	this->vehicleImage = nvgCreateImage(vg, "Resources/carIcon.png", 0);
	this->vehicleStartImage = nvgCreateImage(vg, "Resources/vehicleIconStart.png", 0);
	this->vehicleEndImage = nvgCreateImage(vg, "Resources/vehicleIconEnd.png", 0);
	this->VehicleErrorImage = nvgCreateImage(vg, "Resources/vehicleErrorIcon.png", 0);

	UpdateSettings();
}

void NvgDrawHelper::DrawMap(Map * map)
{
	if (map == NULL) return;

	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	float magnificationRatio = *this->magnificationRatio;

	drawAreaSize->x = map->GetWidth() * magnificationRatio;
	drawAreaSize->y = map->GetHeight() * magnificationRatio;

	drawAreaPosition->x = (widgetSize.x - drawAreaSize->x - 2 * MAP_BORDER_WIDTH) / 2;
	drawAreaPosition->y = (widgetSize.y - drawAreaSize->y - 2 * MAP_BORDER_WIDTH) / 2;

	maxOffset->x = drawAreaPosition->x < 0 ? abs(drawAreaPosition->x) : 0;
	maxOffset->y = drawAreaPosition->y < 0 ? abs(drawAreaPosition->y) : 0;

	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;

	nvgBeginPath(vg);
	nvgRect(vg, drawAreaPosition->x + offset.x, drawAreaPosition->y + offset.y, drawAreaSize->x, drawAreaSize->y);
	nvgStrokeWidth(vg, 5);
	nvgStrokeColor(vg, MAP_BORDER_COLOR);
	nvgStroke(vg);
	nvgFillColor(vg, MAP_BORDER_COLOR);
	nvgFill(vg);

	for (int i = 0; i < map->GetTerrainWidthCount(); i++)
	{
		for (int j = 0; j < map->GetTerrainHeightCount(); j++)
		{
			DrawTerrain(map->GetTerrainSlice(i, j));
		}
	}

	DrawMeshOnMap(map, 1);

	drawMapElements(map->GetMapElements());
}

void NvgDrawHelper::DrawVehicle(Vehicle * vehicle, VehicleType type)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	float magnificationRatio = (*this->magnificationRatio);

	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;

	auto position = vehicle->GetPosition();
	auto size = vehicle->GetSize() * magnificationRatio;
	auto dirWheelbase = vehicle->GetDirWheelbase();
	auto dirTrack = vehicle->GetDirTrack();
	auto tireRadius = 25.0f * magnificationRatio;

	auto p0 = glm::vec2(drawAreaPosition->x + offset.x + position.x * magnificationRatio, drawAreaPosition->y + offset.y + position.y * magnificationRatio);
	auto p1 = p0 + (float)(size.x / 2.0) * dirWheelbase;
	auto p2 = p1 + (float)(size.y / 2.0) * dirTrack;
	auto p3 = p1 - (float)(size.y / 2.0) * dirTrack;
	auto p4 = p0 - (float)(size.x / 2.0) * dirWheelbase;
	auto p5 = p4 + (float)(size.y / 2.0) * dirTrack;
	auto p6 = p4 - (float)(size.y / 2.0) * dirTrack;

	auto p2a = p2 - (tireRadius * dirWheelbase);
	auto p2b = p2 + (tireRadius * dirWheelbase);
	auto p3a = p3 - (tireRadius * dirWheelbase);
	auto p3b = p3 + (tireRadius * dirWheelbase);
	auto p5a = p5 - (tireRadius * dirWheelbase);
	auto p5b = p5 + (tireRadius * dirWheelbase);
	auto p6a = p6 - (tireRadius * dirWheelbase);
	auto p6b = p6 + (tireRadius * dirWheelbase);

	int imgHandler;
	switch (type)
	{
	case VehicleType::VehicleStart:
		imgHandler = vehicleStartImage;
		break;
	case VehicleType::VehicleEnd:
		imgHandler = vehicleEndImage;
		break;
	case VehicleType::VehicleError:
		imgHandler = VehicleErrorImage;
		break;
	default:
		imgHandler = vehicleImage;
		break;
	}

	nvgBeginPath(vg);
	nvgMoveTo(vg, p2.x, p2.y);
	nvgLineTo(vg, p3.x, p3.y);
	nvgLineTo(vg, p6.x, p6.y);
	nvgLineTo(vg, p5.x, p5.y);
	nvgLineTo(vg, p2.x, p2.y);
	nvgFillPaint(vg, nvgImagePattern(vg, p6.x, p6.y, size.x, size.y, -(float)vehicle->GetAngle(), imgHandler, 1));
	nvgFill(vg);
}

void NvgDrawHelper::DrawPath(Path * path)
{
	if (path == NULL) return;

	std::vector<PathElement*> pathElements = path->GetElements();
	for (int i = 0; i < pathElements.size(); i++)
	{
		DrawPathElement(pathElements[i]);
	}
}

void NvgDrawHelper::DrawGraph(Graph *g)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	for (int i = 0; i < g->VerticesCount(); i++)
	{
		for (int j = 0; j < g->VerticesCount(); j++)
		{
			if (g->GetEdge(i, j) == NULL) continue;

			glm::vec2 from = drawAreaPosition + glm::vec2(g->GetEdge(i, j)->v1->x, g->GetEdge(i, j)->v1->y) * magnificationRatio + offset;;
			glm::vec2 to = drawAreaPosition + glm::vec2(g->GetEdge(i, j)->v2->x, g->GetEdge(i, j)->v2->y) * magnificationRatio + offset;;

			nvgBeginPath(vg);
			nvgMoveTo(vg, from.x, from.y);
			nvgLineTo(vg, to.x, to.y);
			nvgStrokeColor(vg, GRAPH_EDGE_COLOR);
			nvgStrokeWidth(vg, GRAPH_EDGE_WIDTH);
			nvgStroke(vg);
		}
	}

	for (int i = 0; i < g->VerticesCount(); i++)
	{
		glm::vec2 v = drawAreaPosition + glm::vec2(g->GetVertex(i)->x, g->GetVertex(i)->y) * magnificationRatio + offset;;

		nvgBeginPath(vg);
		nvgEllipse(vg, v.x, v.y, GRAPH_VERTEX_RADIUS, GRAPH_VERTEX_RADIUS);
		nvgFillColor(vg, GRAPH_VERTEX_COLOR);
		nvgFill(vg);
	}
}

void NvgDrawHelper::DrawSimulationFrame(Simulation * simulation)
{
	DrawMap(simulation->GetMap());

	DrawVehicle(simulation->GetVehicle());
}

void NvgDrawHelper::DrawPolygon(std::vector<glm::vec2> polygon, bool fill)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	for (int i = 0; i < polygon.size(); i++)
	{
		glm::vec2 from = drawAreaPosition + polygon[i] * magnificationRatio + offset;;
		glm::vec2 to = drawAreaPosition + polygon[(i + 1) % polygon.size()] * magnificationRatio + offset;;

		nvgBeginPath(vg);
		nvgMoveTo(vg, from.x, from.y);
		nvgLineTo(vg, to.x, to.y);
		nvgStrokeColor(vg, POLYGON_BORDER_COLOR);
		nvgStrokeWidth(vg, POLYGON_BORDER_WIDTH);
		nvgStroke(vg);
		if (fill)
		{
			nvgFillColor(vg, POLYGON_COLOR);
			nvgFill(vg);
		}
	}
}

void NvgDrawHelper::DrawActiveElement(MapElement * mapElement, bool admissible)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;
	if (mapElement == NULL) return;

	std::vector<glm::vec2> points = mapElement->GetPoints();
	glm::vec2 elementPosition = drawAreaPosition + mapElement->GetPosition() * magnificationRatio + offset;

	nvgBeginPath(vg);
	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + offset.x, drawAreaPosition.y + points[0].y * magnificationRatio + offset.y);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + offset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + offset.y);
	nvgStrokeColor(vg, admissible ? ACTIVE_GOOD_BORDER_COLOR : ACTIVE_BAD_BORDER_COLOR);
	nvgStrokeWidth(vg, ACTIVE_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, admissible ? ACTIVE_GOOD_COLOR : ACTIVE_BAD_COLOR);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgEllipse(vg, elementPosition.x, elementPosition.y, SELECTED_MARKER_SIZE, SELECTED_MARKER_SIZE);
	nvgFillColor(vg, SELECTED_MARKER_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::DrawHoverElement(MapElement * mapElement)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;
	if (mapElement == NULL) return;

	std::vector<glm::vec2> points = mapElement->GetPoints();

	nvgBeginPath(vg);
	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + offset.x, drawAreaPosition.y + points[0].y * magnificationRatio + offset.y);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + offset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + offset.y);
	nvgStrokeColor(vg, MAP_ELEMENT_HOVER_BORDER_COLOR);
	nvgStrokeWidth(vg, MAP_ELEMENT_HOVER_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, MAP_ELEMENT_HOVER_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::DrawSelectedElement(MapElement * mapElement)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;
	if (mapElement == NULL) return;

	std::vector<glm::vec2> points = mapElement->GetPoints();

	nvgBeginPath(vg);
	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + offset.x, drawAreaPosition.y + points[0].y * magnificationRatio + offset.y);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + offset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + offset.y);
	if (!mapElement->IsMoveActive() && !mapElement->IsRotationActive() && !mapElement->IsResizeActive())
	{
		nvgStrokeColor(vg, MAP_ELEMENT_SELECTED_BORDER_COLOR);
		nvgStrokeWidth(vg, MAP_ELEMENT_SELECTED_BORDER_WIDTH);
		nvgFillColor(vg, MAP_ELEMENT_SELECTED_COLOR);
	}
	else
	{
		nvgStrokeColor(vg, mapElement->IsAdmissible() ? ACTIVE_GOOD_BORDER_COLOR : ACTIVE_BAD_BORDER_COLOR);
		nvgStrokeWidth(vg, ACTIVE_BORDER_WIDTH);
		nvgFillColor(vg, mapElement->IsAdmissible() ? ACTIVE_GOOD_COLOR : ACTIVE_BAD_COLOR);
	}
	nvgStroke(vg);
	nvgFill(vg);

	drawTransformShapes(mapElement);
}

void NvgDrawHelper::DrawStartFlag(Vehicle *vehicle, bool isAdmissible)
{
	if (isAdmissible)
		DrawVehicle(vehicle, VehicleType::VehicleStart);
	else
		DrawVehicle(vehicle, VehicleType::VehicleError);
}

void NvgDrawHelper::DrawEndFlag(Vehicle *vehicle, bool isAdmissible)
{
	if (isAdmissible)
		DrawVehicle(vehicle, VehicleType::VehicleEnd);
	else
		DrawVehicle(vehicle, VehicleType::VehicleError);
}

void NvgDrawHelper::DrawArrow(glm::vec2 point, glm::vec2 direction)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	auto from = drawAreaPosition + point * magnificationRatio + offset;
	auto to = from + 50.0f * direction;

	nvgBeginPath(vg);
	nvgMoveTo(vg, from.x, from.y);
	nvgLineTo(vg, to.x, to.y);
	nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));
	nvgFill(vg);
}

void NvgDrawHelper::DrawPathElement(PathElement * pathElement, bool isSelected)
{
	if (dynamic_cast<Line*>(pathElement) != NULL)
	{
		Line *line = dynamic_cast<Line*>(pathElement);

		drawLine(line, isSelected);
	}
	else if (dynamic_cast<Circle*>(pathElement) != NULL)
	{
		Circle *circle = dynamic_cast<Circle*>(pathElement);

		drawCircle(circle, isSelected);
	}
	else if (dynamic_cast<BSpline*>(pathElement) != NULL)
	{
		BSpline *bSpline = dynamic_cast<BSpline*>(pathElement);

		drawBSpline(bSpline, isSelected);
	}
}

void NvgDrawHelper::DrawMeshOnMap(Map * map, int width)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	nvgBeginPath(vg);	
	for (int i = meshOnMapSize; i < map->GetWidth(); i += meshOnMapSize)
	{
		auto from = drawAreaPosition + glm::vec2(i, 0) * magnificationRatio + offset;
		auto to = drawAreaPosition + glm::vec2(i, map->GetHeight()) * magnificationRatio + offset;

		nvgMoveTo(vg, from.x, from.y);
		nvgLineTo(vg, to.x, to.y);
	}
	for (int i = meshOnMapSize; i < map->GetHeight(); i += meshOnMapSize)
	{
		auto from = drawAreaPosition + glm::vec2(0, i) * magnificationRatio + offset;
		auto to = drawAreaPosition + glm::vec2(map->GetWidth(), i) * magnificationRatio + offset;

		nvgMoveTo(vg, from.x, from.y);
		nvgLineTo(vg, to.x, to.y);
	}
	nvgStrokeWidth(vg, width);
	nvgStrokeColor(vg, MAP_BORDER_COLOR);
	nvgStroke(vg);
}

void NvgDrawHelper::DrawTerrain(Terrain * terrain, bool isHover)
{
	if (terrain == NULL) return;

	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	nvgBeginPath(vg);
	nvgRect(vg, drawAreaPosition.x + terrain->GetPosition().x * magnificationRatio + offset.x, drawAreaPosition.y + terrain->GetPosition().y * magnificationRatio + offset.y, TERRAIN_SLICE_SIZE * magnificationRatio, TERRAIN_SLICE_SIZE * magnificationRatio);
	nvgFillColor(vg, isHover ? TERRAIN_HOVER_COLOR : nvgRGBA(terrain->GetColor().r, terrain->GetColor().g, terrain->GetColor().b, 255));
	nvgFill(vg);
}

#pragma endregion

#pragma region Private methods.

void NvgDrawHelper::updateDrawAreaProperties()
{

}

void NvgDrawHelper::drawMapElements(std::vector<MapElement*> mapElements)
{
	for (int i = 0; i < mapElements.size(); i++)
	{
		if (dynamic_cast<Obstacle*>(mapElements[i]) != NULL)
		{
			auto obstacle = dynamic_cast<Obstacle*>(mapElements[i]);
			drawObstacle(obstacle);
		}
		else if (dynamic_cast<Vehicle*>(mapElements[i]) != NULL)
		{
			auto vehicle = dynamic_cast<Vehicle*>(mapElements[i]);
			drawVehicle(vehicle);
		}
		else if (dynamic_cast<ParkingSpace*>(mapElements[i]) != NULL)
		{
			auto parkingSpace = dynamic_cast<ParkingSpace*>(mapElements[i]);
			drawParkingSpace(parkingSpace);
		}
	}
}

void NvgDrawHelper::drawObstacle(Obstacle * obstacle)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);
	
	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;
	if (obstacle == NULL) return;

	std::vector<glm::vec2> points = obstacle->GetPoints();
	glm::vec2 obstaclePosition = drawAreaPosition + obstacle->GetPosition() * magnificationRatio + offset;
	
	nvgBeginPath(vg);
	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + offset.x, drawAreaPosition.y + points[0].y * magnificationRatio + offset.y);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + offset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + offset.y);

	//nvgFontSize(vg, 18.0f);
	//nvgFontFace(vg, "sans-bold");
	//nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

	//nvgFontBlur(vg, 2);
	//nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
	//nvgText(vg, x + w / 2, y + 16 + 1, "Nazwa przeszkody", NULL);

	nvgStrokeColor(vg, obstacle->GetType() == Building ? BUILDING_BORDER_COLOR : DECORATION_BORDER_COLOR);
	nvgStrokeWidth(vg, obstacle->GetType() == Building ? BUILDING_BORDER_WIDTH : DECORATION_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, obstacle->GetType() == Building ? BUILDING_COLOR : DECORATION_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::drawVehicle(Vehicle * vehicle)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;
	if (vehicle == NULL) return;

	std::vector<glm::vec2> points = vehicle->GetPoints();
	glm::vec2 parkingSpacePosition = drawAreaPosition + vehicle->GetPosition() * magnificationRatio + offset;

	nvgBeginPath(vg);
	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + offset.x, drawAreaPosition.y + points[0].y * magnificationRatio + offset.y);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + offset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + offset.y);
	nvgStrokeColor(vg, VEHICLE_BORDER_COLOR);
	nvgStrokeWidth(vg, VEHICLE_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, VEHICLE_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::drawParkingSpace(ParkingSpace * parkingSpace)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);
	
	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;
	if (parkingSpace == NULL) return;

	std::vector<glm::vec2> points = parkingSpace->GetPoints();
	glm::vec2 parkingSpacePosition = drawAreaPosition + parkingSpace->GetPosition() * magnificationRatio + offset;
	
	nvgBeginPath(vg);
	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + offset.x, drawAreaPosition.y + points[0].y * magnificationRatio + offset.y);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + offset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + offset.y);
	nvgStrokeColor(vg, PARKING_SPACE_BORDER_COLOR);
	nvgStrokeWidth(vg, PARKING_SPACE_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, PARKING_SPACE_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::drawLine(Line *line, bool isSelected)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	glm::vec2 from = drawAreaPosition + line->GetFrom() * magnificationRatio + offset;
	glm::vec2 to = drawAreaPosition + line->GetTo() * magnificationRatio + offset;

	nvgBeginPath(vg);
	nvgMoveTo(vg, from.x, from.y);
	nvgLineTo(vg, to.x, to.y);
	nvgStrokeWidth(vg, isSelected ? PATH_ELEMENT_SELECTED_WIDTH : PATH_LINE_WIDTH);
	nvgStrokeColor(vg, isSelected ? PATH_ELEMENT_SELECTED_COLOR : PATH_LINE_COLOR);
	nvgStroke(vg);
}

void NvgDrawHelper::drawCircle(Circle *circle, bool isSelected)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	if (circle->GetAngleFrom() < circle->GetAngleTo())
	{
		nvgBeginPath(vg);
		for (double angle = circle->angleFrom; angle < circle->angleTo; angle += 0.01)
		{
			if (angle == circle->angleFrom)
			{
				auto p = drawAreaPosition + circle->GetPointForAngle(angle) * magnificationRatio + offset;
				nvgMoveTo(vg, p.x, p.y);
			}

			auto p = drawAreaPosition + circle->GetPointForAngle(angle) * magnificationRatio + offset;
			nvgLineTo(vg, p.x, p.y);
		}
		nvgStrokeWidth(vg, isSelected ? PATH_ELEMENT_SELECTED_WIDTH : PATH_CIRCLE_WIDTH);
		nvgStrokeColor(vg, isSelected ? PATH_ELEMENT_SELECTED_COLOR : PATH_CIRCLE_COLOR);
		nvgStroke(vg);
	}
	else
	{
		nvgBeginPath(vg);
		for (double angle = circle->angleFrom; angle > circle->angleTo; angle -= 0.01)
		{
			if (angle == circle->angleFrom)
			{
				auto p = drawAreaPosition + circle->GetPointForAngle(angle) * magnificationRatio + offset;
				nvgMoveTo(vg, p.x, p.y);
			}

			auto p = drawAreaPosition + circle->GetPointForAngle(angle) * magnificationRatio + offset;
			nvgLineTo(vg, p.x, p.y);
		}
		nvgStrokeWidth(vg, isSelected ? PATH_ELEMENT_SELECTED_WIDTH : PATH_CIRCLE_WIDTH);
		nvgStrokeColor(vg, isSelected ? PATH_ELEMENT_SELECTED_COLOR : PATH_CIRCLE_COLOR);
		nvgStroke(vg);
	}
}

void NvgDrawHelper::drawBSpline(BSpline *bSpline, bool isSelected, bool drawPolyline)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	std::vector<glm::vec2> controlPoints = bSpline->GetControlPoints();

	if (drawPolyline)
	{
		nvgBeginPath(vg);
		nvgMoveTo(vg, controlPoints[0].x, controlPoints[0].y);
		for (int i = 0; i < controlPoints.size(); i++)
			nvgLineTo(vg, controlPoints[1].x, controlPoints[1].y);
		nvgStrokeWidth(vg, 3);
		nvgStrokeColor(vg, nvgRGBA(255, 255, 0, 255));
		nvgStroke(vg);
	}

	nvgBeginPath(vg);
	auto startPoint = drawAreaPosition + controlPoints[0] * magnificationRatio + offset;
	nvgMoveTo(vg, startPoint.x, startPoint.y);
	for (double t = 0; t < 1; t += 0.01)
	{
		if (t < bSpline->knots[bSpline->n] || t > bSpline->knots[bSpline->m - bSpline->n]) continue;

		/*auto x = bSpline->GetPointX(t);
		auto y = bSpline->GetPointY(t);*/

		auto p = drawAreaPosition + bSpline->GetPoint(t) * magnificationRatio + offset;

		nvgLineTo(vg, p.x, p.y);
	}
	nvgStrokeWidth(vg, isSelected ? PATH_ELEMENT_SELECTED_WIDTH : PATH_BSPLINE_WIDTH);
	nvgStrokeColor(vg, isSelected ? PATH_ELEMENT_SELECTED_COLOR : PATH_BSPLINE_COLOR);
	nvgStroke(vg);
}

void NvgDrawHelper::drawResizeShape(MapElement * mapElement)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = *this->magnificationRatio;

	glm::vec2 mapElementSize = mapElement->GetSize() * magnificationRatio;
	glm::vec2 mapElementDirX = mapElement->GetDirX();
	glm::vec2 mapElementDirY = mapElement->GetDirY();
	glm::vec2 mapElementPosition = drawAreaPosition + mapElement->GetPosition() * magnificationRatio + offset;
	float resizeShapeSize = TRANSFORM_SHAPE_RESIZE_RADIUS * magnificationRatio;
	bool isHover = mapElement->IsResizeHover();
	int hoverCorner = mapElement->GetResizeHoverCorner();

	auto p0 = mapElementPosition + mapElementDirX * mapElementSize.x / 2.0f - mapElementDirY * mapElementSize.y / 2.0f;
	auto p1 = mapElementPosition + mapElementDirX * mapElementSize.x / 2.0f + mapElementDirY * mapElementSize.y / 2.0f;
	auto p2 = mapElementPosition - mapElementDirX * mapElementSize.x / 2.0f + mapElementDirY * mapElementSize.y / 2.0f;
	auto p3 = mapElementPosition - mapElementDirX * mapElementSize.x / 2.0f - mapElementDirY * mapElementSize.y / 2.0f;

	nvgBeginPath(vg);
	nvgEllipse(vg, p0.x, p0.y, resizeShapeSize, resizeShapeSize);
	nvgEllipse(vg, p1.x, p1.y, resizeShapeSize, resizeShapeSize);
	nvgEllipse(vg, p2.x, p2.y, resizeShapeSize, resizeShapeSize);
	nvgEllipse(vg, p3.x, p3.y, resizeShapeSize, resizeShapeSize);
	nvgFillColor(vg, TRANSFORM_SHAPE_RESIZE_COLOR);
	nvgFill(vg);

	if (isHover)
	{
		nvgBeginPath(vg);
		switch (hoverCorner)
		{
		case 0:
			nvgEllipse(vg, p0.x, p0.y, resizeShapeSize, resizeShapeSize);
			break;
		case 1:
			nvgEllipse(vg, p1.x, p1.y, resizeShapeSize, resizeShapeSize);
			break;
		case 2:
			nvgEllipse(vg, p2.x, p2.y, resizeShapeSize, resizeShapeSize);
			break;
		case 3:
			nvgEllipse(vg, p3.x, p3.y, resizeShapeSize, resizeShapeSize);
			break;
		}
		nvgFillColor(vg, TRANSFORM_SHAPE_HOVER_COLOR);
		nvgFill(vg);
	}
}

void NvgDrawHelper::drawMoveShape(MapElement * mapElement)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = *this->magnificationRatio;

	glm::vec2 mapElementPosition = drawAreaPosition + mapElement->GetPosition() * magnificationRatio + offset;
	float moveShapeSize = TRANSFORM_SHAPE_MOVE_RADIUS * magnificationRatio;
	bool isHover = mapElement->IsMoveHover();

	nvgBeginPath(vg);
	nvgEllipse(vg, mapElementPosition.x, mapElementPosition.y, moveShapeSize, moveShapeSize);
	nvgFillColor(vg, isHover ? TRANSFORM_SHAPE_HOVER_COLOR : TRANSFORM_SHAPE_MOVE_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::drawRotateShape(MapElement * mapElement)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = *this->magnificationRatio;

	glm::vec2 mapElementPosition = drawAreaPosition + mapElement->GetPosition() * magnificationRatio + offset;
	float rotateShapeRadius = TRANSFORM_SHAPE_ROTATE_RADIUS * magnificationRatio;
	float rotateShapeAngleStart = -0.5f;
	float rotateShapeAngleEnd = 0.5f;
	bool isHover = mapElement->IsRotationHover();

	nvgBeginPath(vg);
	nvgArc(vg, mapElementPosition.x, mapElementPosition.y, rotateShapeRadius, rotateShapeAngleStart, rotateShapeAngleEnd, NVG_CW);
	nvgStrokeColor(vg, isHover ? TRANSFORM_SHAPE_HOVER_COLOR : TRANSFORM_SHAPE_ROTATE_COLOR);
	nvgStroke(vg);
	
	nvgBeginPath(vg);
	nvgArc(vg, mapElementPosition.x, mapElementPosition.y, rotateShapeRadius, rotateShapeAngleStart + M_PI / 2.0f, rotateShapeAngleEnd + M_PI / 2.0f, NVG_CW);
	nvgStrokeColor(vg, isHover ? TRANSFORM_SHAPE_HOVER_COLOR : TRANSFORM_SHAPE_ROTATE_COLOR);
	nvgStroke(vg);

	nvgBeginPath(vg);
	nvgArc(vg, mapElementPosition.x, mapElementPosition.y, rotateShapeRadius, rotateShapeAngleStart + M_PI, rotateShapeAngleEnd + M_PI, NVG_CW);
	nvgStrokeColor(vg, isHover ? TRANSFORM_SHAPE_HOVER_COLOR : TRANSFORM_SHAPE_ROTATE_COLOR);
	nvgStroke(vg);
	
	nvgBeginPath(vg);
	nvgArc(vg, mapElementPosition.x, mapElementPosition.y, rotateShapeRadius, rotateShapeAngleStart + 3 * M_PI / 2.0f, rotateShapeAngleEnd + 3 * M_PI / 2.0f, NVG_CW);
	nvgStrokeColor(vg, isHover ? TRANSFORM_SHAPE_HOVER_COLOR : TRANSFORM_SHAPE_ROTATE_COLOR);
	nvgStroke(vg);
}

void NvgDrawHelper::drawTransformShapes(MapElement * mapElement)
{
	if (mapElement == NULL) return;

	drawMoveShape(mapElement);
	drawResizeShape(mapElement);
	drawRotateShape(mapElement);
}

#pragma endregion

void NvgDrawHelper::UpdateSettings()
{
	SELECTED_MARKER_SIZE = Settings::getInstance()->GetInt("SELECTED_MARKER_WIDTH");
	SELECTED_MARKER_COLOR = Settings::getInstance()->GetColor("SELECTED_MARKER_COLOR");

	ACTIVE_BORDER_WIDTH = Settings::getInstance()->GetInt("ACTIVE_BORDER_WIDTH");
	ACTIVE_GOOD_COLOR = Settings::getInstance()->GetColor("ACTIVE_GOOD_COLOR");
	ACTIVE_GOOD_BORDER_COLOR = Settings::getInstance()->GetColor("ACTIVE_GOOD_BORDER_COLOR");
	ACTIVE_BAD_COLOR = Settings::getInstance()->GetColor("ACTIVE_BAD_COLOR");
	ACTIVE_BAD_BORDER_COLOR = Settings::getInstance()->GetColor("ACTIVE_BAD_BORDER_COLOR");

	MAP_BORDER_WIDTH = Settings::getInstance()->GetInt("MAP_BORDER_WIDTH");
	MAP_COLOR = Settings::getInstance()->GetColor("MAP_COLOR");
	MAP_BORDER_COLOR = Settings::getInstance()->GetColor("MAP_BORDER_COLOR");

	MAP_ELEMENT_HOVER_BORDER_WIDTH = Settings::getInstance()->GetInt("MAP_ELEMENT_HOVER_BORDER_WIDTH");
	MAP_ELEMENT_HOVER_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_HOVER_COLOR");
	MAP_ELEMENT_HOVER_BORDER_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_HOVER_BORDER_COLOR");

	MAP_ELEMENT_SELECTED_BORDER_WIDTH = Settings::getInstance()->GetInt("MAP_ELEMENT_SELECTED_BORDER_WIDTH");
	MAP_ELEMENT_SELECTED_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_SELECTED_COLOR");
	MAP_ELEMENT_SELECTED_BORDER_COLOR = Settings::getInstance()->GetColor("MAP_ELEMENT_SELECTED_BORDER_COLOR");

	BUILDING_BORDER_WIDTH = Settings::getInstance()->GetInt("BUILDING_BORDER_WIDTH");
	BUILDING_COLOR = Settings::getInstance()->GetColor("BUILDING_COLOR");
	BUILDING_BORDER_COLOR = Settings::getInstance()->GetColor("BUILDING_BORDER_COLOR");

	DECORATION_BORDER_WIDTH = Settings::getInstance()->GetInt("DECORATION_BORDER_WIDTH");
	DECORATION_COLOR = Settings::getInstance()->GetColor("DECORATION_COLOR");
	DECORATION_BORDER_COLOR = Settings::getInstance()->GetColor("DECORATION_BORDER_COLOR");

	PARKING_SPACE_BORDER_WIDTH = Settings::getInstance()->GetInt("PARKING_SPACE_BORDER_WIDTH");
	PARKING_SPACE_COLOR = Settings::getInstance()->GetColor("PARKING_SPACE_COLOR");
	PARKING_SPACE_BORDER_COLOR = Settings::getInstance()->GetColor("PARKING_SPACE_BORDER_COLOR");

	VEHICLE_BORDER_WIDTH = Settings::getInstance()->GetInt("VEHICLE_BORDER_WIDTH");
	VEHICLE_COLOR = Settings::getInstance()->GetColor("VEHICLE_COLOR");
	VEHICLE_BORDER_COLOR = Settings::getInstance()->GetColor("VEHICLE_BORDER_COLOR");

	GRAPH_EDGE_WIDTH = Settings::getInstance()->GetInt("GRAPH_EDGE_WIDTH");
	GRAPH_EDGE_COLOR = Settings::getInstance()->GetColor("GRAPH_EDGE_COLOR");

	GRAPH_VERTEX_RADIUS = Settings::getInstance()->GetInt("GRAPH_VERTEX_RADIUS");
	GRAPH_VERTEX_COLOR = Settings::getInstance()->GetColor("GRAPH_VERTEX_COLOR");

	PATH_LINE_WIDTH = Settings::getInstance()->GetInt("PATH_LINE_WIDTH");
	PATH_LINE_COLOR = Settings::getInstance()->GetColor("PATH_LINE_COLOR");

	PATH_CIRCLE_WIDTH = Settings::getInstance()->GetInt("PATH_CIRCLE_WIDTH");
	PATH_CIRCLE_COLOR = Settings::getInstance()->GetColor("PATH_CIRCLE_COLOR");

	PATH_BSPLINE_WIDTH = Settings::getInstance()->GetInt("PATH_BSPLINE_WIDTH");
	PATH_BSPLINE_COLOR = Settings::getInstance()->GetColor("PATH_BSPLINE_COLOR");

	PATH_ELEMENT_SELECTED_WIDTH = Settings::getInstance()->GetInt("PATH_ELEMENT_SELECTED_WIDTH");
	PATH_ELEMENT_SELECTED_COLOR = Settings::getInstance()->GetColor("PATH_ELEMENT_SELECTED_COLOR");

	POLYGON_BORDER_WIDTH = Settings::getInstance()->GetInt("POLYGON_BORDER_WIDTH");
	POLYGON_COLOR = Settings::getInstance()->GetColor("POLYGON_COLOR");
	POLYGON_BORDER_COLOR = Settings::getInstance()->GetColor("POLYGON_BORDER_COLOR");

	TRANSFORM_SHAPE_MOVE_RADIUS = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_MOVE_RADIUS");
	TRANSFORM_SHAPE_ROTATE_RADIUS = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_ROTATE_RADIUS");
	TRANSFORM_SHAPE_ROTATE_WIDTH = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_ROTATE_WIDTH");
	TRANSFORM_SHAPE_RESIZE_RADIUS = Settings::getInstance()->GetInt("TRANSFORM_SHAPE_RESIZE_RADIUS");
	TRANSFORM_SHAPE_MOVE_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_MOVE_COLOR");
	TRANSFORM_SHAPE_ROTATE_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_ROTATE_COLOR");
	TRANSFORM_SHAPE_RESIZE_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_RESIZE_COLOR");
	TRANSFORM_SHAPE_HOVER_COLOR = Settings::getInstance()->GetColor("TRANSFORM_SHAPE_HOVER_COLOR");

	TERRAIN_HOVER_COLOR = Settings::getInstance()->GetColor("TERRAIN_HOVER_COLOR");

	meshOnMapSize = 25;

	int lala = 0;
}