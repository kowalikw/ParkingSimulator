#include "MapEditor.h"

void MapEditor::CreateMap(int width, int height)
{
	map = Map(width, height);
}

void MapEditor::SaveMap(string filePath)
{
	ofstream f(filePath, ios::out);

	boost::archive::text_oarchive oa(f);

	oa << map;
}

void MapEditor::LoadMap(string filePath)
{
	map = Map(0, 0);

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
		map.AddMapElement(newElement);
		this->newElement = nullptr;
		this->resetAddButtons = true;
	}
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

bool MapEditor::GetResetAddButtons()
{
	if (resetAddButtons)
	{
		this->resetAddButtons = false;
		return true;
	}
	return false;
}

void MapEditor::SetMap(Map map)
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
	return mapContainsMapElement(mapElement) && !mapElementIntersectsMapElement(mapElement);
}

bool MapEditor::mapContainsMapElement(MapElement * mapElement)
{
	std::vector<glm::vec2> mapPolygon = map.GetPoints();
	std::vector<glm::vec2> mapElementPolygon = mapElement->GetPoints();

	return GeometryHelper::CheckPolygonContainsPolygon(mapPolygon, mapElementPolygon);
}

bool MapEditor::mapElementIntersectsMapElement(MapElement * mapElement)
{
	std::vector<MapElement*> mapElements = map.GetMapElements();
	for (int i = 0; i < mapElements.size(); i++)
	{
		if (mapElements[i] == mapElement) continue;

		if (GeometryHelper::CheckPolygonIntersection(mapElement->GetPoints(), mapElements[i]->GetPoints()))
			return true;
	}

	return false;
}
