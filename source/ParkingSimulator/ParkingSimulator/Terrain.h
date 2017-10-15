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

BOOST_SERIALIZATION_SPLIT_FREE(glm::vec3)
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec2)

namespace boost
{
	namespace serialization
	{
		template<class Archive> void save(Archive& ar, const glm::vec2& v, unsigned int version)
		{
			ar & v.x & v.y;
		}

		template<class Archive> void load(Archive& ar, glm::vec2& v, unsigned int version)
		{
			ar & v.x & v.y;
		}

		template<class Archive> void save(Archive& ar, const glm::vec3& v, unsigned int version)
		{
			ar & v.x & v.y & v.z;
		}

		template<class Archive> void load(Archive& ar, glm::vec3& v, unsigned int version)
		{
			ar & v.x & v.y & v.z;
		}
	}
}

class Terrain
{
public:
	Terrain();
	Terrain(glm::vec2 position);
	Terrain(std::string modelPath, glm::vec3 color, glm::vec2 position);
	std::string GetModelPath();
	glm::vec3 GetColor();
	glm::vec2 GetPosition();
	void SetModelPath(std::string modelPath);
	void SetColor(glm::vec3 color);
	void SetPosition(glm::vec2 position);

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & modelPath;
		ar & color;
		ar & position;
	}
private:
	std::string modelPath;
	glm::vec3 color;
	glm::vec2 position;
};

#endif