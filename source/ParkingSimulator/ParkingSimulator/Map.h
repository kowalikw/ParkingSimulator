#ifndef MAP_H
#define MAP_H

#include <vector>
#include "MapElement.h"
#include "Obstacle.h"
#include "ParkingSpace.h"
#include "Vehicle.h"

using namespace std;

class Map
{
public:
	Map();
	Map(int width, int height);
	int GetWidth();
	int GetHeight();
	void AddMapElement(MapElement *mapElement);
	vector<MapElement*> GetMapElements();
private:
	int width;
	int height;
	vector<MapElement*> mapElements;
};

#endif