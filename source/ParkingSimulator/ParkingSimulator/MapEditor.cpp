#include "MapEditor.h"

void MapEditor::CreateMap(int width, int height)
{
	map = new Map(width, height);
}

void MapEditor::SaveMap(string filePath)
{
	ofstream f(filePath, ios::out);

	boost::archive::text_oarchive oa(f);

	oa << map;
}

void MapEditor::LoadMap(string filePath)
{
	map = new Map(0, 0);

	std::ifstream ifs(filePath);

	boost::archive::text_iarchive ia(ifs);

	ia >> this->map;
}

void MapEditor::AddObstacle(string name, int width, int height, ObstacleType type)
{
	MapElement *newElement = new Obstacle(glm::vec2(), glm::vec2(width, height), ObstacleType::Building, name);
	this->newElement = newElement;
}

void MapEditor::AddObstacleConfirm()
{
	if (IsMapElementAdmissible(newElement))
	{
		map->AddMapElement(newElement);
		this->newElement = nullptr;
		this->resetAddButtons = true;
	}
}

Map *MapEditor::GetMap()
{
	return this->map;
}

bool MapEditor::GetAddBuilding()
{
	return this->addBuilding;
}

bool MapEditor::GetAddDecoration()
{
	return this->addDecoration;
}

bool MapEditor::GetAddRoad()
{
	return this->addRoad;
}

bool MapEditor::GetAddParkPlace()
{
	return this->addParkPlace;
}

bool MapEditor::GetResetAddButtons()
{
	if (resetAddButtons)
	{
		this->resetAddButtons = false;
		return true;
	}
	return false;
}

void MapEditor::SetMap(Map *map)
{
	this->map = map;
}

void MapEditor::SetAddBuilding(bool addBuilding)
{
	this->addBuilding = addBuilding;
}

void MapEditor::SetAddDecoration(bool addDecoration)
{
	this->addDecoration = addDecoration;
}

void MapEditor::SetAddRoad(bool addRoad)
{
	this->addRoad = addRoad;
}

void MapEditor::SetAddParkPlace(bool addParkPlace)
{
	this->addParkPlace = addParkPlace;
}

void MapEditor::SetResetAddButtons(bool resetAddButtons)
{
	this->resetAddButtons = resetAddButtons;
}

void MapEditor::SetNewElement(MapElement * mapElement)
{
	this->newElement = mapElement;
}

void MapEditor::SetSelectedElement(MapElement * mapElement)
{
	this->selectedElement = mapElement;
}

MapElement * MapEditor::GetNewElement()
{
	return this->newElement;
}

MapElement * MapEditor::GetSelectedElement()
{
	return this->selectedElement;
}

MapElement * MapEditor::GetHoverElement(glm::vec2 mousePosition)
{
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
			if (GeometryHelper::CheckPolygonContainsPoint(mapElements[i]->GetPoints(), mousePosition))
				return mapElements[i];
	}
	return nullptr;
}

MapElement * MapEditor::GetMapElementToMove(glm::vec2 mousePosition)
{
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
			if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, mapElements[i]->GetPosition()) < 20)
				return mapElements[i];
	}
	return nullptr;
}
MapElement * MapEditor::GetMapElementToRotate(glm::vec2 mousePosition)
{
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
			if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, mapElements[i]->GetPosition()) < 40)
				return mapElements[i];
	}
	return nullptr;
}

MapElement * MapEditor::GetMapElementToResize(glm::vec2 mousePosition, int * corner)
{
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
		{
			*corner = -1;

			glm::vec2 position = mapElements[i]->GetPosition();
			glm::vec2 size = mapElements[i]->GetSize();
			glm::vec2 dirX = mapElements[i]->GetDirX();
			glm::vec2 dirY = mapElements[i]->GetDirY();

			glm::vec2 p0 = position + size.x * dirX - size.y * dirY;
			glm::vec2 p1 = position + size.x * dirX + size.y * dirY;
			glm::vec2 p2 = position - size.x * dirX + size.y * dirY;
			glm::vec2 p3 = position - size.x * dirX - size.y * dirY;

			if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p0) < 20)
				*corner = 0;
			if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p1) < 20)
				*corner = 1;
			if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p2) < 20)
				*corner = 2;
			if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p3) < 20)
				*corner = 3;

			return mapElements[i];
		}
	}
	return nullptr;
}

bool MapEditor::IsMapElementAdmissible(MapElement * mapElement)
{
	return mapContainsMapElement(mapElement) && !mapElementIntersectsMapElement(mapElement);
}

bool MapEditor::mapContainsMapElement(MapElement * mapElement)
{
	std::vector<glm::vec2> mapPolygon = map->GetPoints();
	std::vector<glm::vec2> mapElementPolygon = mapElement->GetPoints();

	return GeometryHelper::CheckPolygonContainsPolygon(mapPolygon, mapElementPolygon);
}

bool MapEditor::mapElementIntersectsMapElement(MapElement * mapElement)
{
	std::vector<MapElement*> mapElements = map->GetMapElements();
	for (int i = 0; i < mapElements.size(); i++)
	{
		if (mapElements[i] == mapElement) continue;

		if (GeometryHelper::CheckPolygonIntersection(mapElement->GetPoints(), mapElements[i]->GetPoints()))
			return true;
	}

	return false;
}
