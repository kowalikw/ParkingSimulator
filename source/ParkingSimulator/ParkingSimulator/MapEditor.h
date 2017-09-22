#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "Map.h"
#include "GeometryHelper.h"
#include <qlist.h>
#include <qtreewidget.h>

class MapEditor
{
public:
	MapEditor();

	void CreateMap(int width, int height);
	void SaveMap(string filePath);
	void LoadMap(string filePath);

	void AddNewMapElement(MapElement *mapElement);
	void AddNewMapElementConfirm();

	void AddObstacle(string name, int width, int height, ObstacleType type);
	void AddObstacleConfirm();
	void AddParkingSpace(string name, int width, int height, ParkingSpaceType type);
	Map *GetMap();

	bool GetAddBuilding();
	bool GetAddDecoration();
	bool GetAddRoad();
	bool GetAddParkPlace();
	bool GetResetAddButtons();
	bool GetSelectedElementChanged();
	bool GetMapElementsChanged();
	bool GetMapElementsPropertiesChanged();
	bool SelectedElementChangedOnMap();

	void SetMap(Map *map);
	void SetAddBuilding(bool addBuilding);
	void SetAddDecoration(bool addDecoration);
	void SetAddRoad(bool addRoad);
	void SetAddParkPlace(bool addParkPlace);
	void SetResetAddButtons(bool resetAddButtons);
	void SetSelectedElementChanged(bool selectedElementChanged);
	void SetMapElementsChanged(bool mapElementsChanged);
	void SetMapElementsPropertiesChanged(bool mapElementsPropertiesChanged);
	void SetSelectedElementChangedOnMap(bool selectedElementChangedOnMap);

	void SetNewElement(MapElement *mapElement);
	void SetSelectedElement(MapElement *mapElement);

	MapElement * GetNewElement();
	MapElement * GetSelectedElement();
	MapElement * GetHoverElement(glm::vec2 mousePosition);

	MapElement * GetMapElementToMove(glm::vec2 mousePosition);
	MapElement * GetMapElementToRotate(glm::vec2 mousePosition);
	MapElement * GetMapElementToResize(glm::vec2 mousePosition, int *corner, MapElement *selectedElement = nullptr);

	bool IsMapElementAdmissible(MapElement *mapElement);
	bool MapContainsMapElement(MapElement *mapElement);
	bool MapElementIntersectsMapElement(MapElement *mapElement);

	QList<bool> GetMapElementsTreeItemsExpanded();
	QList<QTreeWidgetItem*> * GetMapElementsTreeItems();
private:
	QList<bool> mapElementsTreeItemsExpanded;
	QList<QTreeWidgetItem*> *mapElementsTreeItems;
	QTreeWidgetItem *mapElementsTree = NULL;
	QTreeWidgetItem *buildingsTree = NULL;
	QTreeWidgetItem *decorationsTree = NULL;
	QTreeWidgetItem *roadsTree = NULL;
	QTreeWidgetItem *parkingPlacesTree = NULL;

	Map *map = NULL;
	bool addBuilding = false;
	bool addDecoration = false;
	bool addRoad = false;
	bool addParkPlace = false;
	bool resetAddButtons = false;
	bool selectedElementChanged = false;
	bool mapElementsChanged = false;
	bool mapElementsPropertiesChanged = false;
	bool selectedElementChangedOnMap = false;
	MapElement *newElement = NULL;
	MapElement *selectedElement = NULL;

	void updateMapElementsTreeItem();

	const double DISTANCE_TO_MOVE = 15;
	const double DISTANCE_TO_ROTATE = 30;
	const double DISTANCE_TO_RESIZE = 15;
	const double DISTANCE_TO_ROTATE_MOVE_OFFSET = 3;
};

#endif