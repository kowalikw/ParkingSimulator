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
	vector<PathElement*> GetElements();
	void AddElement(PathElement *element);
	void RemoveElement(PathElement *element);
	void SetElements(std::vector<PathElement*> elements);
	double GetLength();
	double GetLengthToElement(PathElement *pathElement);
	PathElement *GetElement(double t);

	

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & elements.size();
		for (int i = 0; i < elements.size(); i++)
		{
			auto elementType = std::string(typeid(*elements[i]).name());
			ar & elementType;
			if(elementType == std::string(typeid(Line).name()))
				ar & *dynamic_cast<Line*>(elements[i]);
			else if (elementType == std::string(typeid(Circle).name()))
				ar & *dynamic_cast<Circle*>(elements[i]);
		}
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		int elementsCount;
		std::string elementType;
		PathElement *element;
		elements = std::vector<PathElement*>();

		ar & elementsCount;
		for (int i = 0; i < elementsCount; i++)
		{
			ar & elementType;
			if (elementType == std::string(typeid(Line).name()))
			{
				element = new Line();
			}
			else if (elementType == std::string(typeid(Circle).name()))
			{
				element = new Circle();
			}
			ar & *element;
			elements.push_back(element);
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	std::vector<PathElement*> elements;

	friend class boost::serialization::access;
};

#endif