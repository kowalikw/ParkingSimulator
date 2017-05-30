#ifndef PATH_H
#define PATH_H

#include <vector>
#include <glm.hpp>
#include "PathElement.h"
#include "Vehicle.h"
#include "Map.h"

using namespace std;

enum PathType
{
	Simple,
	Spline
};

class Path
{
public:
	Path();
	~Path();
	PathElement GetAt(int i);
	vector<PathElement> GetElements();
	void AddElement(PathElement element);
	//void CreateAdmissiblePath(vector<PathElement> elements, Map map, Vehicle vehicle, PathType type = Simple);
private:
	vector<PathElement> elements;
};

#endif