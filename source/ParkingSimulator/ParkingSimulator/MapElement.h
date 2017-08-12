#ifndef MAP_ELEMENT_H
#define MAP_ELEMENT_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>

class MapElement
{
public:
	virtual ~MapElement();
	std::string GetName();
	glm::vec2 GetPosition();
	glm::vec2 GetRotation();
	glm::vec2 GetSize();
protected:
	std::string name;
	glm::vec2 position;
	glm::vec2 rotation;
	glm::vec2 size;
};

#endif