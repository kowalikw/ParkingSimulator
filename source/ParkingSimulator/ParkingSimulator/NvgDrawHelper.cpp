#include "NvgDrawHelper.h"

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

	if (drawAreaSize.x == 0 || drawAreaSize.y == 0) return;

	/*maxOffsetX = mapPositionX < 0 ? abs(mapPositionX) : 0;
	maxOffsetY = mapPositionY < 0 ? abs(mapPositionY) : 0;*/

	nvgBeginPath(vg);
	nvgRect(vg, drawAreaPosition.x + offset.x, drawAreaPosition.y + offset.y, drawAreaSize.x + 2 * MAP_BORDER_WIDTH, drawAreaSize.y + 2 * MAP_BORDER_WIDTH);
	nvgFillColor(vg, MAP_BORDER_COLOR);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, drawAreaPosition.x + offset.x + MAP_BORDER_WIDTH, drawAreaPosition.y + offset.y + MAP_BORDER_WIDTH, drawAreaSize.x, drawAreaSize.y);
	nvgFillColor(vg, MAP_COLOR);
	nvgFill(vg);

	drawMapElements(&map->GetMapElements());
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

	auto p0 = glm::vec2(drawAreaPosition.x + offset.x + position.x * magnificationRatio, drawAreaPosition.y + offset.y + position.y * magnificationRatio);
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

void NvgDrawHelper::DrawPath(Path * path)
{
	std::vector<glm::vec2> pointss;
	auto pathElements = path->GetElements();
	for (int i = 0; i < pathElements.size(); i++)
	{
		if (pathElements[i].GetType() == PathElementType::Line)
		{
			auto line = pathElements[i];

			nvgBeginPath(vg);
			nvgMoveTo(vg, line.from.x, line.from.y);
			nvgLineTo(vg, line.to.x, line.to.y);
			nvgStrokeWidth(vg, PATH_LINE_WIDTH);
			nvgStrokeColor(vg, PATH_LINE_COLOR);
			nvgStroke(vg);
		}
		else if (pathElements[i].GetType() == PathElementType::Circle)
		{
			auto circle = pathElements[i];

			nvgBeginPath(vg);
			for (double angle = circle.angleFrom; angle < circle.angleTo; angle += 0.01)
			{
				if (angle == circle.angleFrom)
				{
					auto p = circle.GetCirclePoint(angle);
					nvgMoveTo(vg, p.x, p.y);
				}
				auto p = circle.GetCirclePoint(angle);
				nvgLineTo(vg, p.x, p.y);
			}
			nvgStrokeWidth(vg, PATH_ARC_WIDTH);
			nvgStrokeColor(vg, PATH_ARC_COLOR);
			nvgStroke(vg);

			nvgBeginPath(vg);
			for (double angle = circle.angleFrom; angle > circle.angleTo; angle -= 0.01)
			{
				if (angle == circle.angleFrom)
				{
					auto p = circle.GetCirclePoint(angle);
					nvgMoveTo(vg, p.x, p.y);
				}
				auto p = circle.GetCirclePoint(angle);
				nvgLineTo(vg, p.x, p.y);
			}
			nvgStrokeWidth(vg, PATH_ARC_WIDTH);
			nvgStrokeColor(vg, PATH_ARC_COLOR);
			nvgStroke(vg);

			/*nvgBeginPath(vg);
			for (double angle = 0; angle < 2 * M_PI; angle += 0.01)
			{
				if (angle == circle.angleFrom)
				{
					auto p = circle.GetCirclePoint(angle);
					nvgMoveTo(vg, p.x, p.y);
				}
				auto p = circle.GetCirclePoint(angle);
				nvgLineTo(vg, p.x, p.y);
			}
			nvgStrokeWidth(vg, PATH_ARC_WIDTH);
			nvgStrokeColor(vg, PATH_ARC_COLOR);
			nvgStroke(vg);*/
		}

		/*nvgBeginPath(vg);
		for (int i = 0; i < (int)pointss.size() - 2; i++)
		{
			nvgMoveTo(vg, pointss[i].x, pointss[i].y);
			nvgLineTo(vg, pointss[i + 1].x, pointss[i + 1].y);
		}
		nvgStrokeWidth(vg, PATH_ARC_WIDTH);
		nvgStrokeColor(vg, PATH_ARC_COLOR);
		nvgStroke(vg);*/
	}
}

void NvgDrawHelper::updateDrawAreaProperties()
{
}

void NvgDrawHelper::drawMapElements(std::vector<MapElement*>* mapElements)
{
	for (int i = 0; i < mapElements->size(); i++)
	{
		if (dynamic_cast<Obstacle*>((*mapElements)[i]) != NULL)
		{
			auto obstacle = dynamic_cast<Obstacle*>((*mapElements)[i]);
			drawObstacle(obstacle);
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
	int buildingPositionX = drawAreaPosition.x + obstacle->GetPosition().x * magnificationRatio + offset.x;
	int buildingPositionY = drawAreaPosition.y + obstacle->GetPosition().y * magnificationRatio + offset.y;

	nvgBeginPath(vg);
	nvgMoveTo(vg, drawAreaPosition.x + points[0].x * magnificationRatio + offset.x, drawAreaPosition.y + points[0].y * magnificationRatio + offset.y);
	for (int i = 0; i <= points.size(); i++)
		nvgLineTo(vg, drawAreaPosition.x + points[i % points.size()].x * magnificationRatio + offset.x, drawAreaPosition.y + points[i % points.size()].y * magnificationRatio + offset.y);
	nvgStrokeColor(vg, BUILDING_BORDER_COLOR);
	nvgStrokeWidth(vg, BUILDING_BORDER_WIDTH);
	nvgStroke(vg);
	nvgFillColor(vg, BUILDING_COLOR);
	nvgFill(vg);
}

void NvgDrawHelper::drawParkingSpace(ParkingSpace * parkingSpace)
{

}