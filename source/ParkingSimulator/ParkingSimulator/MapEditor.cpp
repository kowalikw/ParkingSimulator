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

void MapEditor::AddBuilding(MapElement *building)
{
	map.AddMapElement(building);
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
