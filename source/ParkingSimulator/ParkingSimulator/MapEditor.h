#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "Map.h"
#include "GeometryHelper.h"

class MapEditor
{
public:
	void CreateMap(int width, int height);
	void SaveMap(string filePath);
	void LoadMap(string filePath);
	void AddObstacle(string name, int width, int height, ObstacleType type);
	void AddObstacleConfirm();
	Map *GetMap();

	bool GetAddBuilding();
	bool GetAddDecoration();
	bool GetAddRoad();
	bool GetAddParkPlace();
	bool GetResetAddButtons();

	void SetMap(Map *map);
	void SetAddBuilding(bool addBuilding);
	void SetAddDecoration(bool addDecoration);
	void SetAddRoad(bool addRoad);
	void SetAddParkPlace(bool addParkPlace);
	void SetResetAddButtons(bool resetAddButtons);

	void SetNewElement(MapElement *mapElement);
	void SetSelectedElement(MapElement *mapElement);

	MapElement * GetNewElement();
	MapElement * GetSelectedElement();
	MapElement * GetHoverElement(glm::vec2 mousePosition);

	MapElement * GetMapElementToMove(glm::vec2 mousePosition);
	MapElement * GetMapElementToRotate(glm::vec2 mousePosition);
	MapElement * GetMapElementToResize(glm::vec2 mousePosition, int *corner);

	bool IsMapElementAdmissible(MapElement *mapElement);
private:
	Map *map = NULL;
	bool addBuilding = false;
	bool addDecoration = false;
	bool addRoad = false;
	bool addParkPlace = false;
	bool resetAddButtons = false;
	MapElement *newElement = NULL;
	MapElement *selectedElement = NULL;

	bool mapContainsMapElement(MapElement *mapElement);
	bool mapElementIntersectsMapElement(MapElement *mapElement);
};

#endif