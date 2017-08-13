#include "MapEditor.h"

void MapEditor::CreateMap(int width, int height)
{
	map = Map(width, height);
}

void MapEditor::SaveMap()
{
}

void MapEditor::LoadMap()
{
}

void MapEditor::AddBuilding(int width, int height)
{
	MapElement *newElement = new Obstacle(glm::vec2(), glm::vec2(width, height), ObstacleType::Building, "Budyneczek");
	this->newElement = newElement;
}

Map MapEditor::GetMap()
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

MapElement * MapEditor::GetNewElement()
{
	return this->newElement;
}

MapElement * MapEditor::GetSelectedElement()
{
	return this->selectedElement;
}

bool MapEditor::IsMapElementAdmissible(MapElement * mapElement)
{
	return mapContainsMapElement(mapElement);// && mapElementIntersectsMapElement(mapElement);
}

bool MapEditor::mapContainsMapElement(MapElement * mapElement)
{
	std::vector<glm::vec2> mapPolygon = map.GetPoints();
	std::vector<glm::vec2> mapElementPolygon = mapElement->GetPoints();

	return GeometryHelper::CheckPolygonContainsPolygon(mapPolygon, mapElementPolygon);
}

bool MapEditor::mapElementIntersectsMapElement(MapElement * mapElement)
{
	return false;
}
