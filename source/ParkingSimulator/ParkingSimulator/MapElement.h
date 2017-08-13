#ifndef MAP_ELEMENT_H
#define MAP_ELEMENT_H
#define GLM_FORCE_RADIANS

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>

class MapElement
{
public:
	virtual ~MapElement();
	std::string GetName();
	double GetRotation();
	glm::vec2 GetPosition();
	glm::vec2 GetSize();
	std::vector<glm::vec2> GetPoints();
	void SetName(std::string name);
	void SetRotation(double rotation);
	void SetPosition(glm::vec2 position);
	void SetSize(glm::vec2 size);
	void SetPoints(std::vector<glm::vec2> points);
protected:
	std::string name;
	double rotation;
	glm::vec2 position;
	glm::vec2 size;
	std::vector<glm::vec2> points;

	void move();
	void rotate();
	void resize();
	void transform();
};

#endif