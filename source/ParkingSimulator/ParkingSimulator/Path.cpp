#include "Path.h"

Path::Path()
{
}

Path::~Path()
{
}

PathElement *Path::GetAt(int i)
{
	return elements[i];
}

PathElement * Path::GetFirstElement()
{
	return elements[0];
}

PathElement * Path::GetLastElement()
{
	return elements[elements.size() - 1];
}

vector<PathElement*> Path::GetElements()
{
	return this->elements;
}

void Path::AddElement(PathElement *element)
{
	this->elements.push_back(element);
}

double Path::GetLength()
{
	double pathLength = 0;
	for (int i = 0; i < elements.size(); i++)
		pathLength += elements[i]->GetLength();
	return pathLength;
}

double Path::GetLengthToElement(PathElement *pathElement)
{
	double length = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i] == pathElement)
			return length;
		length += elements[i]->GetLength();
	}
	return length;
}

PathElement * Path::GetElement(double t)
{
	double pathLength = GetLength();
	double currentLength = t * pathLength;

	double tmpLength = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		tmpLength += elements[i]->GetLength();
		if (tmpLength > currentLength)
			return elements[i];
	}

	return nullptr;
}