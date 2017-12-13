#ifndef TERRAIN_H
#define TERRAIN_H

#define TERRAIN_SLICE_SIZE 25

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

class Terrain
{
public:
	Terrain();
	Terrain(glm::vec2 position);
	Terrain(std::string modelPath, glm::vec3 color, glm::vec2 position);
	~Terrain();

	std::string GetModelPath();
	glm::vec3 GetColor();
	glm::vec2 GetPosition();
	void SetModelPath(std::string modelPath);
	void SetColor(glm::vec3 color);
	void SetPosition(glm::vec2 position);

#pragma region Boost serialization.

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & modelPath;
		ar & color;
		ar & position;
	}

#pragma endregion

private:
	std::string modelPath;
	glm::vec3 color;
	glm::vec2 position;
};

#endif