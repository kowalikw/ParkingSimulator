#ifndef MAP_ELEMENT_H
#define MAP_ELEMENT_H
#define GLM_FORCE_RADIANS

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

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

	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & rotation;
		ar & position;
		ar & size;
		ar & points;
	}
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