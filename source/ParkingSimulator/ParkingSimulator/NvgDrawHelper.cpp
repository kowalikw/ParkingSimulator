#include "NvgDrawHelper.h"

#pragma region Public methods.

NvgDrawHelper::NvgDrawHelper(NVGcontext * vg, glm::vec2 * widgetSize, glm::vec2 * offset, glm::vec2 * maxOffset, float * magnificationRatio)
{
	this->vg = vg;
	this->widgetSize = widgetSize;
	this->offset = offset;
	this->maxOffset = maxOffset;
	this->magnificationRatio = magnificationRatio;
}

void NvgDrawHelper::DrawMap(Map * map)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	float magnificationRatio = *this->magnificationRatio;

	drawAreaSize.x = map->GetWidth() * magnificationRatio;
	drawAreaSize.y = map->GetHeight() * magnificationRatio;

	drawAreaPosition.x = (widgetSize.x - drawAreaSize.x - 2 * MAP_BORDER_WIDTH) / 2;
	drawAreaPosition.y = (widgetSize.y - drawAreaSize.y - 2 * MAP_BORDER_WIDTH) / 2;

	maxOffset->x = drawAreaPosition.x < 0 ? abs(drawAreaPosition.x) : 0;
	maxOffset->y = drawAreaPosition.y < 0 ? abs(drawAreaPosition.y) : 0;

	if (drawAreaSize.x == 0 || drawAreaSize.y == 0) return;

	nvgBeginPath(vg);
	nvgRect(vg, drawAreaPosition.x + offset.x, drawAreaPosition.y + offset.y, drawAreaSize.x + 2 * MAP_BORDER_WIDTH, drawAreaSize.y + 2 * MAP_BORDER_WIDTH);
	nvgFillColor(vg, MAP_BORDER_COLOR);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, drawAreaPosition.x + offset.x + MAP_BORDER_WIDTH, drawAreaPosition.y + offset.y + MAP_BORDER_WIDTH, drawAreaSize.x, drawAreaSize.y);
	nvgFillColor(vg, MAP_COLOR);
	nvgFill(vg);

	drawMapElements(map->GetMapElements());
}

void NvgDrawHelper::DrawVehicle(Vehicle * vehicle)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	float magnificationRatio = (*this->magnificationRatio);

	if (drawAreaSize.x == 0 || drawAreaSize.y == 0) return;

	auto position = vehicle->GetPosition();
	auto wheelbase = vehicle->GetWheelbase() * magnificationRatio;
	auto track = vehicle->GetTrack() * magnificationRatio;
	auto dirWheelbase = vehicle->GetDirWheelbase();
	auto dirTrack = vehicle->GetDirTrack();
	auto tireRadius = 25.0f * magnificationRatio;

	//auto p0 = glm::vec2(drawAreaPosition.x + offset.x + position.x * magnificationRatio, drawAreaPosition.y + offset.y + position.y * magnificationRatio);
	//auto p1 = p0 + (float)(wheelbase / 2.0) * dirWheelbase;
	//auto p2 = p1 + (float)(track / 2.0) * dirTrack;
	//auto p3 = p1 - (float)(track / 2.0) * dirTrack;
	//auto p4 = p0 - (float)(wheelbase / 2.0) * dirWheelbase;
	//auto p5 = p4 + (float)(track / 2.0) * dirTrack;
	//auto p6 = p4 - (float)(track / 2.0) * dirTrack;

	auto p0 = glm::vec2(drawAreaPosition.x + offset.x + position.x * magnificationRatio, drawAreaPosition.y + offset.y + position.y * magnificationRatio);
	auto p1 = p0 + (float)wheelbase * dirWheelbase;
	auto p2 = p1 + (float)(track / 2.0) * dirTrack;
	auto p3 = p1 - (float)(track / 2.0) * dirTrack;
	auto p4 = p0;
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
	for (int i = 0; i < g->VerticesCount(); i++)
	{
		for (int j = 0; j < g->VerticesCount(); j++)
		{
			if (g->GetEdge(i, j) == NULL) continue;

			nvgBeginPath(vg);
			nvgMoveTo(vg, g->GetEdge(i, j)->v1->x, g->GetEdge(i, j)->v1->y);
			nvgLineTo(vg, g->GetEdge(i, j)->v2->x, g->GetEdge(i, j)->v2->y);
			nvgStrokeColor(vg, GRAPH_EDGE_COLOR);
			nvgStrokeWidth(vg, GRAPH_EDGE_WIDTH);
			nvgStroke(vg);
		}
	}

	for (int i = 0; i < g->VerticesCount(); i++)
	{
		nvgBeginPath(vg);
		nvgEllipse(vg, g->GetVertex(i)->x, g->GetVertex(i)->y, GRAPH_VERTEX_RADIUS, GRAPH_VERTEX_RADIUS);
		nvgFillColor(vg, GRAPH_VERTEX_COLOR);
		nvgFill(vg);
	}
}

void NvgDrawHelper::DrawSimulationFrame(Simulation * simulation)
{
	DrawMap(simulation->GetMap());

	DrawVehicle(simulation->GetVehicle());
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
	float magnificationRatio = (*this->magnificationRatio);
	
	if (drawAreaSize.x == 0 || drawAreaSize.y == 0) return;
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

	nvgStrokeColor(vg, BUILDING_BORDER_COLOR);
	nvgStrokeWidth(vg, BUILDING_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, BUILDING_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::drawParkingSpace(ParkingSpace * parkingSpace)
{
	glm::vec2 offset = *this->offset;
	glm::vec2 widgetSize = *this->widgetSize;
	float magnificationRatio = (*this->magnificationRatio);
	
	if (drawAreaSize.x == 0 || drawAreaSize.y == 0) return;
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

#pragma endregion