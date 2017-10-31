#ifndef PATH_H
#define PATH_H

#include <vector>
#include <glm.hpp>
#include "PathElement.h"
#include "Vehicle.h"
#include "Map.h"
#include <string>

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
	PathElement *GetNextElement(PathElement *element);
	PathElement *GetPrevElement(PathElement *element);
	vector<PathElement*> GetElements();
	void AddElement(PathElement *element);
	void RemoveElement(PathElement *element);
	void SetElements(std::vector<PathElement*> elements);
	void CalculatePathElementsLength();
	double GetLength();
	double GetLengthToElement(PathElement *pathElement);
	PathElement *GetElement(double t);

	SimulationState GetSimulationState(double t);
	std::vector<SimulationState> GetAllSimulationStates(double step);

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & elements.size();
		for (int i = 0; i < elements.size(); i++)
		{
			auto elementType = std::string(typeid(*elements[i]).name());
			if (elementType == std::string(typeid(Line).name()))
			{
				ar & std::string("Line");
				ar & *dynamic_cast<Line*>(elements[i]);
			}
			else if (elementType == std::string(typeid(Circle).name()))
			{
				ar & std::string("Circle");
				ar & *dynamic_cast<Circle*>(elements[i]);
			}
			else if (elementType == std::string(typeid(BSpline).name()))
			{
				ar & std::string("BSpline");
				ar & *dynamic_cast<BSpline*>(elements[i]);
			}
		}
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		int tmp;
		int elementsCount;
		std::string elementType;

		ar & elementsCount;
		for (int i = 0; i < elementsCount; i++)
		{
			ar & elementType;
			if (elementType == std::string("Line"))
			{
				Line *element = new Line();
				ar & *element;
				elements.push_back(element);
			}
			else if (elementType == std::string("Circle"))
			{
				Circle *element = new Circle();
				ar & *element;
				elements.push_back(element);
			}
			else if (elementType == std::string("BSpline"))
			{
				BSpline *element = new BSpline();
				ar & *element;
				elements.push_back(element);
			}
		}

		CalculatePathElementsLength();
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	std::vector<PathElement*> elements;

	double calculateLengthToElement(PathElement *pathElement);

	friend class boost::serialization::access;
};

#endif