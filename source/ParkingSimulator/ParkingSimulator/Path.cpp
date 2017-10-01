#include "Path.h"

Path::Path()
{
}

Path::Path(std::vector<Path*> pathParts)
{
	for (int i = 0; i < pathParts.size(); i++)
	{
		for (int j = 0; j < pathParts[i]->elements.size(); j++)
		{
			elements.push_back(pathParts[i]->elements[j]);
		}
	}
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

	element->SetLengthToElement(calculateLengthToElement(element));
}

void Path::RemoveElement(PathElement * element)
{
	std::vector<PathElement*> newElements;
	for (int i = 0; i < elements.size(); i++)
		if (elements[i] != element)
			newElements.push_back(elements[i]);
	this->elements = newElements;
}

void Path::SetElements(std::vector<PathElement*> elements)
{
	this->elements = elements;
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
	return pathElement->GetLengthToElement();
}

PathElement * Path::GetElement(double t)
{
	double pathLength = GetLength();
	double currentLength = t * pathLength;

	double tmpLength = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		tmpLength += elements[i]->GetLength();
		if (tmpLength >= currentLength)
			return elements[i];
	}

	return nullptr;
}

SimulationState Path::GetSimulationState(double t)
{
	try
	{
		PathElement *pathElement = GetElement(t);

		double pathLength = GetLength();
		double lengthToPathElementExclude = GetLengthToElement(pathElement);
		double lengthToPathElementInclude = GetLength() + lengthToPathElementExclude;

		double u = abs((lengthToPathElementExclude - (t * pathLength)) / pathElement->GetLength());
		SimulationState simulationState = pathElement->GetSimulationState(u);

		return simulationState;
	}
	catch (std::exception e)
	{
		int lala = 0;
	}
}

std::vector<SimulationState> Path::GetAllSimulationStates(double step)
{
	std::vector<SimulationState> simulationStates;
	for (double t = 0; t < 1.0; t += step)
		simulationStates.push_back(GetSimulationState(t));
	return simulationStates;
}

double Path::calculateLengthToElement(PathElement *pathElement)
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
