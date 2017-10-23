#include "MapEditor.h"
#include "Language.h"

MapEditor::MapEditor()
{
	mapElementsTreeItems = new QList<QTreeWidgetItem*>();
}

void MapEditor::CreateMap(int width, int height)
{
	map = new Map(width, height);

	mapElementsChanged = true;
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

	SetMapElementsChanged(true);
	SetMapElementsPropertiesChanged(true);
}

void MapEditor::AddNewMapElement(MapElement * mapElement)
{
	this->newElement = mapElement;
	this->newElement->SetPosition(glm::vec2(map->GetWidth() / 2.0f, map->GetHeight() / 2.0f));
}

void MapEditor::AddNewMapElementConfirm()
{
	if (IsMapElementAdmissible(newElement))
	{
		map->AddMapElement(newElement);
		this->newElement = nullptr;
		this->resetAddButtons = true;
	}
}

void MapEditor::AddObstacle(string name, int width, int height, ObstacleType type)
{
	MapElement *newElement = new Obstacle(glm::vec2(), glm::vec2(width, height), type, name);
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

void MapEditor::AddParkingSpace(string name, int width, int height, ParkingSpaceType type)
{
	MapElement *newElement = new ParkingSpace(glm::vec2(), glm::vec2(width, height), type);
	this->newElement = newElement;
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

bool MapEditor::GetAddCar()
{
	return this->addCar;
}

bool MapEditor::GetAddParkPlace()
{
	return this->addParkPlace;
}

bool MapEditor::GetAddTerrain()
{
	return this->addTerrain;
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

bool MapEditor::GetSelectedElementChanged()
{
	return this->selectedElementChanged;
}

bool MapEditor::GetMapElementsChanged()
{
	return this->mapElementsChanged;
}

bool MapEditor::GetMapElementsPropertiesChanged()
{
	return this->mapElementsPropertiesChanged;
}

bool MapEditor::SelectedElementChangedOnMap()
{
	return this->selectedElementChangedOnMap;
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

void MapEditor::SetAddCar(bool addCar)
{
	this->addCar = addCar;
}

void MapEditor::SetAddParkPlace(bool addParkPlace)
{
	this->addParkPlace = addParkPlace;
}

void MapEditor::SetAddTerrain(bool addTerrain)
{
	this->addTerrain = addTerrain;
}

void MapEditor::SetResetAddButtons(bool resetAddButtons)
{
	this->resetAddButtons = resetAddButtons;
}

void MapEditor::SetSelectedElementChanged(bool selectedElementChanged)
{
	this->selectedElementChanged = selectedElementChanged;
}

void MapEditor::SetMapElementsChanged(bool mapElementsChanged)
{
	this->mapElementsChanged = mapElementsChanged;
}

void MapEditor::SetMapElementsPropertiesChanged(bool mapElementsPropertiesChanged)
{
	this->mapElementsPropertiesChanged = mapElementsPropertiesChanged;
}

void MapEditor::SetSelectedElementChangedOnMap(bool selectedElementChangedOnMap)
{
	this->selectedElementChangedOnMap = selectedElementChangedOnMap;
}

void MapEditor::SetNewElement(MapElement * mapElement)
{
	this->newElement = mapElement;
}

void MapEditor::SetSelectedElement(MapElement * mapElement)
{
	this->selectedElement = mapElement;

	SetSelectedElementChanged(true);
	SetMapElementsPropertiesChanged(true);
}

MapElement * MapEditor::GetNewElement()
{
	return this->newElement;
}

MapElement * MapEditor::GetSelectedElement()
{
	if (selectedElement != NULL)
	{
		selectedElement->SetIsAdmissible(true);
		if (!GeometryHelper::CheckPolygonContainsPolygon(map->GetPoints(), selectedElement->GetPoints()))
			selectedElement->SetIsAdmissible(false);
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
		{
			if (selectedElement == mapElements[i])
				continue;
			if (GeometryHelper::CheckPolygonIntersection(mapElements[i]->GetPoints(), selectedElement->GetPoints()))
				selectedElement->SetIsAdmissible(false);
		}
	}

	return this->selectedElement;
}

MapElement * MapEditor::GetHoverElement(glm::vec2 mousePosition)
{
	MapElement *hoverElement = nullptr;
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
		{
			if (mapElements[i] != hoverElement && (mapElements[i]->IsMoveActive() || mapElements[i]->IsRotationActive() || mapElements[i]->IsResizeActive()))
				return nullptr;
			if (GeometryHelper::CheckPolygonContainsPoint(mapElements[i]->GetPoints(), mousePosition))
				hoverElement = mapElements[i];
		}
	}
	return hoverElement;
}

MapElement * MapEditor::GetMapElementToMove(glm::vec2 mousePosition)
{
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
			if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, mapElements[i]->GetPosition()) < DISTANCE_TO_MOVE && !mapElements[i]->IsRotationActive() && !mapElements[i]->IsResizeActive())
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
		{
			double distance = GeometryHelper::GetDistanceBetweenPoints(mousePosition, mapElements[i]->GetPosition());
			if (distance >= DISTANCE_TO_MOVE + DISTANCE_TO_ROTATE_MOVE_OFFSET && distance < DISTANCE_TO_ROTATE && !mapElements[i]->IsMoveActive() && !mapElements[i]->IsResizeActive())
				return mapElements[i];
		}
	}
	return nullptr;
}

MapElement * MapEditor::GetMapElementToResize(glm::vec2 mousePosition, int * corner, MapElement *selectedElement)
{
	if (map != NULL)
	{
		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
		{
			if (selectedElement != NULL && selectedElement != mapElements[i])
				continue;

			*corner = -1;

			glm::vec2 position = mapElements[i]->GetPosition();
			glm::vec2 size = mapElements[i]->GetSize();
			glm::vec2 dirX = mapElements[i]->GetDirX();
			glm::vec2 dirY = mapElements[i]->GetDirY();

			glm::vec2 p0 = position + size.x / 2.0f * dirX - size.y / 2.0f * dirY;
			glm::vec2 p1 = position + size.x / 2.0f * dirX + size.y / 2.0f * dirY;
			glm::vec2 p2 = position - size.x / 2.0f * dirX + size.y / 2.0f * dirY;
			glm::vec2 p3 = position - size.x / 2.0f * dirX - size.y / 2.0f * dirY;

			if (!mapElements[i]->IsMoveActive() && !mapElements[i]->IsRotationActive())
			{
				if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p0) < DISTANCE_TO_RESIZE)
					*corner = 0;
				if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p1) < DISTANCE_TO_RESIZE)
					*corner = 1;
				if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p2) < DISTANCE_TO_RESIZE)
					*corner = 2;
				if (GeometryHelper::GetDistanceBetweenPoints(mousePosition, p3) < DISTANCE_TO_RESIZE)
					*corner = 3;

			}
			if(*corner != -1)
				return mapElements[i];
		}
	}
	return nullptr;
}

bool MapEditor::IsMapElementAdmissible(MapElement * mapElement)
{
	return MapContainsMapElement(mapElement) && !MapElementIntersectsMapElement(mapElement);
}

bool MapEditor::MapContainsMapElement(MapElement * mapElement)
{
	std::vector<glm::vec2> mapPolygon = map->GetPoints();
	std::vector<glm::vec2> mapElementPolygon = mapElement->GetPoints();

	return GeometryHelper::CheckPolygonContainsPolygon(mapPolygon, mapElementPolygon);
}

bool MapEditor::MapElementIntersectsMapElement(MapElement * mapElement)
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

void MapEditor::SetNewTerrain(Terrain * terrain)
{
	this->newTerrain = terrain;
}

Terrain * MapEditor::GetNewTerrain()
{
	return this->newTerrain;
}

Terrain * MapEditor::GetHoverTerrain(glm::vec2 mousePosition)
{
	std::ostringstream ss;
	ss << "X: " << mousePosition.x << endl;
	ss << "Y: " << mousePosition.y << endl;
	ss << endl;
	std::string s(ss.str());

	OutputDebugStringA(s.c_str());

	return map->GetTerrainSlice(mousePosition.x, mousePosition.y);
}

QList<bool> MapEditor::GetMapElementsTreeItemsExpanded()
{
	return this->mapElementsTreeItemsExpanded;
}

QList<QTreeWidgetItem*> * MapEditor::GetMapElementsTreeItems()
{
	updateMapElementsTreeItem();

	return this->mapElementsTreeItems;
}

void MapEditor::updateMapElementsTreeItem()
{
	std::map<std::string, std::string> dictionary = Language::getInstance()->GetDictionary();

	mapElementsTreeItemsExpanded.clear();

	if(mapElementsTree != NULL)
		mapElementsTreeItemsExpanded.append(mapElementsTree->isExpanded());
	mapElementsTree = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString::fromStdString(dictionary["MapEditor_MapElements"])));

	if (buildingsTree != NULL)
		mapElementsTreeItemsExpanded.append(buildingsTree->isExpanded());
	buildingsTree = new QTreeWidgetItem(mapElementsTree, QStringList(QString::fromStdString(dictionary["MapEditor_Buildings"])));

	if (decorationsTree != NULL)
		mapElementsTreeItemsExpanded.append(decorationsTree->isExpanded());
	decorationsTree = new QTreeWidgetItem(mapElementsTree, QStringList(QString::fromStdString(dictionary["MapEditor_Decorations"])));

	if (vehiclesTree != NULL)
		mapElementsTreeItemsExpanded.append(vehiclesTree->isExpanded());
	vehiclesTree = new QTreeWidgetItem(mapElementsTree, QStringList(QString::fromStdString(dictionary["MapEditor_Vehicles"])));

	if (parkingPlacesTree != NULL)
		mapElementsTreeItemsExpanded.append(parkingPlacesTree->isExpanded());
	parkingPlacesTree = new QTreeWidgetItem(mapElementsTree, QStringList(QString::fromStdString(dictionary["MapEditor_ParkingPlaces"])));

	mapElementsTreeItems->clear();
	mapElementsTreeItems->append(mapElementsTree);
	mapElementsTreeItems->append(buildingsTree);
	mapElementsTreeItems->append(decorationsTree);
	mapElementsTreeItems->append(vehiclesTree);
	mapElementsTreeItems->append(parkingPlacesTree);

	if (map != NULL)
	{
		std::vector<Obstacle*> buildings = map->GetObstacles();
		for (int i = 0; i < buildings.size(); ++i)
			if (buildings[i]->GetType() == ObstacleType::Building)
				mapElementsTreeItems->append(new QTreeWidgetItem(buildingsTree, QStringList(QString::fromStdString(buildings[i]->GetName()))));

		std::vector<Obstacle*> decorations = map->GetObstacles();
		for (int i = 0; i < decorations.size(); ++i)
			if (decorations[i]->GetType() == ObstacleType::Decoration)
				mapElementsTreeItems->append(new QTreeWidgetItem(decorationsTree, QStringList(QString::fromStdString(decorations[i]->GetName()))));

		std::vector<Vehicle*> vehicles = map->GetVehicles();
		for (int i = 0; i < vehicles.size(); ++i)
			mapElementsTreeItems->append(new QTreeWidgetItem(vehiclesTree, QStringList(QString::fromStdString(vehicles[i]->GetName()))));

		std::vector<ParkingSpace*> parkingSpaces = map->GetParkingSpaces();
		for (int i = 0; i < parkingSpaces.size(); ++i)
			mapElementsTreeItems->append(new QTreeWidgetItem(parkingPlacesTree, QStringList(QString::fromStdString(parkingSpaces[i]->GetName()))));
	}
}
