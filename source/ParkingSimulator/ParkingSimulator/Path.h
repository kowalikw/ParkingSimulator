#ifndef PATH_H
#define PATH_H

#include <vector>
#include <glm.hpp>
#include "PathElement.h"
#include "Vehicle.h"
#include "Map.h"

using namespace std;

class Path
{
public:
	Path();
	Path(std::vector<Path*> pathParts);
	~Path();
	PathElement *GetAt(int i);
	PathElement *GetFirstElement();
	PathElement *GetLastElement();
	vector<PathElement*> GetElements();
	void AddElement(PathElement *element);
	void RemoveElement(PathElement *element);
	void SetElements(std::vector<PathElement*> elements);
	double GetLength();
	double GetLengthToElement(PathElement *pathElement);
	PathElement *GetElement(double t);
private:
	vector<PathElement*> elements;
};

#endif