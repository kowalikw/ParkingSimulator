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


#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 
#include <Windows.h>

class MapElement
{
public:
	virtual ~MapElement();
	std::string GetName();
	double GetRotation();
	glm::vec2 GetPosition();
	glm::vec2 GetSize();
	glm::vec2 GetDirX();
	glm::vec2 GetDirY();
	std::vector<glm::vec2> GetPoints();
	void SetName(std::string name);
	void SetRotation(double rotation);
	void SetPosition(glm::vec2 position);
	void SetSize(glm::vec2 size);
	void SetPoints(std::vector<glm::vec2> points);
	void Move(glm::vec2 offset);
	void Rotate(double offset);
	void Resize(glm::vec2 offset, int corner);
	void EnableMove(bool isMoveActive);
	void EnableRotation(bool isRotationActive);
	void EnableResize(bool isResizeActive);
	void SetMoveHover(bool isMoveHover);
	void SetRotationHover(bool isRotationHover);
	void SetResizeHover(bool isResizeHover);
	bool IsMoveActive();
	bool IsRotationActive();
	bool IsResizeActive();
	bool IsMoveHover();
	bool IsRotationHover();
	bool IsResizeHover();
	int GetResizeHoverCorner();
	void SetResizeHoverCorner(int corner);
	bool IsAdmissible();
	void SetIsAdmissible(bool isAdmissible);

	std::string GetModelPath();
	std::string GetThumbnailPath();
	std::string GetMapThumbnailPath();
	void SetModelPath(std::string modelPath);
	void SetThumbnailPath(std::string thumbnailPath);
	void SetMapThumbnailPath(std::string thumbnailPath);

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
		std::ostringstream ss;
		ss << "P0 - serialize: " << points[0].x << ", " << points[0].y << endl;
		ss << "P1 - serialize: " << points[1].x << ", " << points[1].y << endl;
		ss << "P2 - serialize: " << points[2].x << ", " << points[2].y << endl;
		ss << "P3 - serialize: " << points[3].x << ", " << points[3].y << endl;
		ss << endl;
		std::string s(ss.str());
		OutputDebugStringA(s.c_str());
		ar & modelPath;
		ar & thumbnailPath;
		ar & mapThumbnailPath;
	}
protected:
	std::string name;
	double rotation;
	glm::vec2 position;
	glm::vec2 size;
	bool isRectangular;
	bool isMoveActive = false;
	bool isRotationActive = false;
	bool isResizeActive = false;
	bool isMoveHover = false;
	bool isRotationHover = false;
	bool isResizeHover = false;
	bool isAdmissible = true;
	int resizeHoverCorner;
	std::vector<glm::vec2> points;

	std::string modelPath;
	std::string thumbnailPath;
	std::string mapThumbnailPath;

	void move();
	void rotate();
	void resize();
	void transform();
private:
	double MAP_ELEMENT_MIN_SIZE = 45;
};

#endif