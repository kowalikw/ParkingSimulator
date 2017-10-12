#include "MapElement.h"

MapElement::~MapElement()
{
}

std::string MapElement::GetName()
{
	return this->name;
}

double MapElement::GetRotation()
{
	return this->rotation;
}

glm::vec2 MapElement::GetPosition()
{
	return this->position;
}

glm::vec2 MapElement::GetSize()
{
	return this->size;
}

glm::vec2 MapElement::GetDirX()
{
	glm::mat4 mtx;
	mtx = glm::rotate(mtx, (float)rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec4 dirX = mtx * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	return glm::normalize(glm::vec2(dirX.x, dirX.y));
}

glm::vec2 MapElement::GetDirY()
{
	glm::mat4 mtx;
	mtx = glm::rotate(mtx, (float)rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec4 dirY = mtx * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	return glm::normalize(glm::vec2(dirY.x, dirY.y));
}

std::vector<glm::vec2> MapElement::GetPoints()
{
	return this->points;
}

void MapElement::SetName(std::string name)
{
	this->name = name;
}

void MapElement::SetRotation(double rotation)
{
	this->rotation = rotation;

	transform();
}

void MapElement::SetPosition(glm::vec2 position)
{
	this->position = position;

	transform();
}

void MapElement::SetSize(glm::vec2 size)
{
	this->size = size;

	transform();
}

void MapElement::SetPoints(std::vector<glm::vec2> points)
{
	this->points = points;
}

void MapElement::Move(glm::vec2 offset)
{
	SetPosition(position + offset);

	transform();
}

void MapElement::Rotate(double offset)
{
	SetRotation(rotation + offset);

	transform();
}

void MapElement::Resize(glm::vec2 offset, int corner)
{
	double sizeX, sizeY;
	glm::vec2 dirX = GetDirX();
	glm::vec2 dirY = GetDirY();

	glm::vec2 p0 = position + size.x / 2.0f * dirX - size.y / 2.0f * dirY;
	glm::vec2 p1 = position + size.x / 2.0f * dirX + size.y / 2.0f * dirY;
	glm::vec2 p2 = position - size.x / 2.0f * dirX + size.y / 2.0f * dirY;
	glm::vec2 p3 = position - size.x / 2.0f * dirX - size.y / 2.0f * dirY;

	switch (corner)
	{
	case 0:
		p0 = offset;
		sizeX = glm::distance(p0, p3);
		sizeY = glm::distance(p0, p1);
		if (sizeX > MAP_ELEMENT_MIN_SIZE && sizeY > MAP_ELEMENT_MIN_SIZE)
		{
			size = glm::vec2(sizeX, sizeY);
			position = (p0 + p2) / 2.0f;
		}
		break;
	case 1:
		p1 = offset;
		sizeX = glm::distance(p1, p2);
		sizeY = glm::distance(p1, p0);
		if (sizeX > MAP_ELEMENT_MIN_SIZE && sizeY > MAP_ELEMENT_MIN_SIZE)
		{
			size = glm::vec2(sizeX, sizeY);
			position = (p1 + p3) / 2.0f;
		}
		break;
	case 2:
		p2 = offset;
		sizeX = glm::distance(p2, p1);
		sizeY = glm::distance(p2, p3);
		if (sizeX > MAP_ELEMENT_MIN_SIZE && sizeY > MAP_ELEMENT_MIN_SIZE)
		{
			size = glm::vec2(sizeX, sizeY);
			position = (p2 + p0) / 2.0f;
		}
		break;
	case 3:
		p3 = offset;
		sizeX = glm::distance(p3, p0);
		sizeY = glm::distance(p3, p2);
		if (sizeX > MAP_ELEMENT_MIN_SIZE && sizeY > MAP_ELEMENT_MIN_SIZE)
		{
			size = glm::vec2(sizeX, sizeY);
			position = (p3 + p1) / 2.0f;
		}
		break;
	}

	transform();
}

void MapElement::EnableMove(bool isMoveActive)
{
	this->isMoveActive = isMoveActive;
}

void MapElement::EnableRotation(bool isRotationActive)
{
	this->isRotationActive = isRotationActive;
}

void MapElement::EnableResize(bool isResizeActive)
{
	this->isResizeActive = isResizeActive;
}

void MapElement::SetMoveHover(bool isMoveHover)
{
	this->isMoveHover = isMoveHover;
}

void MapElement::SetRotationHover(bool isRotationHover)
{
	this->isRotationHover = isRotationHover;
}

void MapElement::SetResizeHover(bool isResizeHover)
{
	this->isResizeHover = isResizeHover;
}

bool MapElement::IsMoveActive()
{
	return this->isMoveActive;
}

bool MapElement::IsRotationActive()
{
	return this->isRotationActive;
}

bool MapElement::IsResizeActive()
{
	return this->isResizeActive;
}

bool MapElement::IsMoveHover()
{
	return this->isMoveHover;
}

bool MapElement::IsRotationHover()
{
	return this->isRotationHover;
}

bool MapElement::IsResizeHover()
{
	return this->isResizeHover;
}

int MapElement::GetResizeHoverCorner()
{
	return this->resizeHoverCorner;
}

void MapElement::SetResizeHoverCorner(int corner)
{
	this->resizeHoverCorner = corner;
}

bool MapElement::IsAdmissible()
{
	return this->isAdmissible;
}

void MapElement::SetIsAdmissible(bool isAdmissible)
{
	this->isAdmissible = isAdmissible;
}

void MapElement::move()
{
	for (int i = 0; i < points.size(); i++)
		points[i] = glm::vec2(points[i].x + position.x, points[i].y + position.y);
}

std::string MapElement::GetModelPath()
{
	return this->modelPath;
}

std::string MapElement::GetThumbnailPath()
{
	return this->thumbnailPath;
}

std::string MapElement::GetMapThumbnailPath()
{
	return this->thumbnailPath;
}

void MapElement::SetModelPath(std::string modelPath)
{
	this->modelPath = modelPath;
}

void MapElement::SetThumbnailPath(std::string thumbnailPath)
{
	this->thumbnailPath = thumbnailPath;
}

void MapElement::SetMapThumbnailPath(std::string mapThumbnailPath)
{
	this->mapThumbnailPath = mapThumbnailPath;
}

void MapElement::rotate()
{
	glm::mat4 mtx;
	mtx = glm::rotate(mtx, (float)rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < points.size(); i++)
	{
		glm::vec4 rotatedPoint = mtx * glm::vec4(points[i].x, points[i].y, 0.0f, 1.0f);
		points[i] = glm::vec2(rotatedPoint.x, rotatedPoint.y);
	}
}

void MapElement::resize()
{
	this->points[0] = glm::vec2(-size.x / 2.0f, -size.y / 2.0f);
	this->points[1] = glm::vec2(size.x / 2.0f, -size.y / 2.0f);
	this->points[2] = glm::vec2(size.x / 2.0f, size.y / 2.0f);
	this->points[3] = glm::vec2(-size.x / 2.0f, size.y / 2.0f);
}

void MapElement::transform()
{
	if(isRectangular) 
		resize();
	rotate();
	move();
}
