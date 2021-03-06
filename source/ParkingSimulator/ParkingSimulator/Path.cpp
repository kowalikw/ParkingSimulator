#include "Path.h"

#pragma region Constructors and destructor.

Path::Path()
{
}

Path::Path(std::vector<Path*> pathParts)
{
	for (int i = 0; i < pathParts.size(); i++)
	{
		if (pathParts[i] == NULL) continue;

		for (int j = 0; j < pathParts[i]->elements.size(); j++)
		{
			auto element = pathParts[i]->elements[j];

			elements.push_back(pathParts[i]->elements[j]);

			element->SetLengthToElement(calculateLengthToElement(element));
		}
	}
}

Path::~Path()
{
	for (int i = 0; i < elements.size(); i++)
	{
		if(dynamic_cast<Circle*>(elements[i]) != nullptr)
			delete dynamic_cast<Circle*>(elements[i]);
		else if (dynamic_cast<BSpline*>(elements[i]) != nullptr)
			delete dynamic_cast<BSpline*>(elements[i]);
		else if (dynamic_cast<Line*>(elements[i]) != nullptr)
			delete dynamic_cast<Line*>(elements[i]);
		else if (dynamic_cast<Turn*>(elements[i]) != nullptr)
			delete dynamic_cast<Turn*>(elements[i]);
	}
}

#pragma endregion

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

PathElement * Path::GetNextElement(PathElement * element)
{
	int index = 0;
	for(int i = 0; i < elements.size(); i++)
		if (element == elements[i])
		{
			index = i;
			break;
		}

	return elements[index + 1];
}

PathElement * Path::GetPrevElement(PathElement * element)
{
	int index = 0;
	for (int i = 0; i < elements.size(); i++)
		if (element == elements[i])
		{
			index = i;
			break;
		}

	return elements[index - 1];
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

void Path::CalculatePathElementsLength()
{
	for (int i = 0; i < elements.size(); i++)
		elements[i]->SetLengthToElement(calculateLengthToElement(elements[i]));
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

void Path::Reverse()
{
	std::vector<PathElement*> reversedPathElements;
	std::vector<PathElement*> pathElements = GetElements();

	for (int i = pathElements.size() - 1; i >= 0; i--)
	{
		ManeuverType maneuverType = pathElements[i]->GetManeuverType();
		pathElements[i]->SetManeuverType(maneuverType == Front ? Back : Front);
		reversedPathElements.push_back(pathElements[i]);
	}

	SetElements(reversedPathElements);
}

SimulationState Path::GetSimulationState(double t)
{
	PathElement *pathElement = GetElement(t);

	if (pathElement == NULL)
		return SimulationState();

	double pathLength = GetLength();
	double lengthToPathElementExclude = GetLengthToElement(pathElement);
	double lengthToPathElementInclude = pathElement->GetLength() + lengthToPathElementExclude;

	double u = abs((lengthToPathElementExclude - (t * pathLength)) / pathElement->GetLength());
	SimulationState simulationState = pathElement->GetSimulationState(u);

	return simulationState;
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
