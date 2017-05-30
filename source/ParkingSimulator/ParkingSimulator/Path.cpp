#include "Path.h"

Path::Path()
{
}

Path::~Path()
{
}

PathElement Path::GetAt(int i)
{
	return elements[i];
}

vector<PathElement> Path::GetElements()
{
	return this->elements;
}

void Path::AddElement(PathElement element)
{
	this->elements.push_back(element);
}
