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
	nvgRect(vg, drawAreaPosition->x + offset.x, drawAreaPosition->y + offset.y, drawAreaSize->x + 2 * MAP_BORDER_WIDTH, drawAreaSize->y + 2 * MAP_BORDER_WIDTH);
	nvgFillColor(vg, MAP_BORDER_COLOR);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, drawAreaPosition->x + offset.x + MAP_BORDER_WIDTH, drawAreaPosition->y + offset.y + MAP_BORDER_WIDTH, drawAreaSize->x, drawAreaSize->y);
	nvgFillColor(vg, MAP_COLOR);
	nvgFill(vg);

	drawMapElements(map->GetMapElements());
}

void NvgDrawHelper::DrawVehicle(Vehicle * vehicle)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	float magnificationRatio = (*this->magnificationRatio);

	if (drawAreaSize->x == 0 || drawAreaSize->y == 0) return;

	auto position = vehicle->GetPosition();
	auto wheelbase = vehicle->GetWheelbase() * magnificationRatio;
	auto track = vehicle->GetTrack() * magnificationRatio;
	auto dirWheelbase = vehicle->GetDirWheelbase();
	auto dirTrack = vehicle->GetDirTrack();
	auto tireRadius = 25.0f * magnificationRatio;

	auto p0 = glm::vec2(drawAreaPosition->x + offset.x + position.x * magnificationRatio, drawAreaPosition->y + offset.y + position.y * magnificationRatio);
	auto p1 = p0 + (float)(wheelbase / 2.0) * dirWheelbase;
	auto p2 = p1 + (float)(track / 2.0) * dirTrack;
	auto p3 = p1 - (float)(track / 2.0) * dirTrack;
	auto p4 = p0 - (float)(wheelbase / 2.0) * dirWheelbase;
	auto p5 = p4 + (float)(track / 2.0) * dirTrack;
	auto p6 = p4 - (float)(track / 2.0) * dirTrack;

	//auto p0 = glm::vec2(drawAreaPosition->x + offset.x + position.x * magnificationRatio, drawAreaPosition->y + offset.y + position.y * magnificationRatio);
	//auto p1 = p0 + (float)wheelbase * dirWheelbase;
	//auto p2 = p1 + (float)(track / 2.0) * dirTrack;
	//auto p3 = p1 - (float)(track / 2.0) * dirTrack;
	//auto p4 = p0;
	//auto p5 = p4 + (float)(track / 2.0) * dirTrack;
	//auto p6 = p4 - (float)(track / 2.0) * dirTrack;

	auto p2a = p2 - (tireRadius * dirWheelbase);
	auto p2b = p2 + (tireRadius * dirWheelbase);
	auto p3a = p3 - (tireRadius * dirWheelbase);
	auto p3b = p3 + (tireRadius * dirWheelbase);
	auto p5a = p5 - (tireRadius * dirWheelbase);
	auto p5b = p5 + (tireRadius * dirWheelbase);
	auto p6a = p6 - (tireRadius * dirWheelbase);
	auto p6b = p6 + (tireRadius * dirWheelbase);

	nvgBeginPath(vg);

	/*nvgMoveTo(vg, p1.x, p1.y);
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
	nvgLineTo(vg, p6b.x, p6b.y);*/

	nvgMoveTo(vg, p2.x, p2.y);
	nvgLineTo(vg, p3.x, p3.y);
	nvgLineTo(vg, p6.x, p6.y);
	nvgLineTo(vg, p5.x, p5.y);
	nvgLineTo(vg, p2.x, p2.y);

	nvgFillColor(vg, VEHICLE_COLOR);
	nvgFill(vg);
	
	/*nvgStrokeWidth(vg, 5);
	nvgStrokeColor(vg, MAP_BORDER_COLOR);
	nvgStroke(vg);*/
}

void NvgDrawHelper::DrawPath(Path * path)
{
	std::vector<PathElement*> pathElements = path->GetElements();
	for (int i = 0; i < pathElements.size(); i++)
	{
		if (dynamic_cast<Line*>(pathElements[i]) != NULL)
		{
			Line *line = dynamic_cast<Line*>(pathElements[i]);

			drawLine(line);
		}
		else if (dynamic_cast<Circle*>(pathElements[i]) != NULL)
		{
			Circle *circle = dynamic_cast<Circle*>(pathElements[i]);

			drawCircle(circle);
		}
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

void NvgDrawHelper::DrawStartFlag(glm::vec2 position)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	position = drawAreaPosition + position * magnificationRatio + offset;

	nvgBeginPath(vg);
	nvgEllipse(vg, position.x, position.y, SELECTED_MARKER_SIZE, SELECTED_MARKER_SIZE);
	nvgFillColor(vg, START_FLAG_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::DrawEndFlag(glm::vec2 position)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	position = drawAreaPosition + position * magnificationRatio + offset;

	nvgBeginPath(vg);
	nvgEllipse(vg, position.x, position.y, SELECTED_MARKER_SIZE, SELECTED_MARKER_SIZE);
	nvgFillColor(vg, END_FLAG_COLOR);
	nvgFill(vg);
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

void NvgDrawHelper::drawRoad(Road *road)
{
	
}

void NvgDrawHelper::drawLine(Line *line)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 drawAreaPosition = *this->drawAreaPosition;
	float magnificationRatio = (*this->magnificationRatio);

	glm::vec2 from = drawAreaPosition + line->GetFrom() * magnificationRatio + offset;
	glm::vec2 to = drawAreaPosition + line->GetTo() * magnificationRatio + offset;

	nvgBeginPath(vg);
	nvgMoveTo(vg, from.x, from.y);
	nvgLineTo(vg, to.x, to.y);
	nvgStrokeWidth(vg, PATH_LINE_WIDTH);
	nvgStrokeColor(vg, PATH_LINE_COLOR);
	nvgStroke(vg);
}

void NvgDrawHelper::drawCircle(Circle *circle)
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
		nvgStrokeWidth(vg, PATH_CIRCLE_WIDTH);
		nvgStrokeColor(vg, PATH_CIRCLE_COLOR);
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
		nvgStrokeWidth(vg, PATH_CIRCLE_WIDTH);
		nvgStrokeColor(vg, PATH_CIRCLE_COLOR);
		nvgStroke(vg);
	}
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