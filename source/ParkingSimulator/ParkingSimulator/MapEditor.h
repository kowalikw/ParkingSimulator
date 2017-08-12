#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "Map.h"

class MapEditor
{
public:
	void CreateMap(int width, int height);
	void SaveMap();
	void LoadMap();
	void AddBuilding(MapElement *building);
	Map GetMap();

	bool GetAddBuilding();
	bool GetAddDecoration();
	bool GetAddRoad();
	bool GetAddParkPlace();

	void SetAddBuilding(bool addBuilding);
	void SetAddDecoration(bool addDecoration);
	void SetAddRoad(bool addRoad);
	void SetAddParkPlace(bool addParkPlace);
private:
	Map map;
	bool addBuilding = false;
	bool addDecoration = false;
	bool addRoad = false;
	bool addParkPlace = false;
};

#endif