#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SOIL2.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "Mesh.h"

using namespace std;

class Model
{
public:
	Model();
	Model(GLchar* path);
	Model(std::string path, bool createModel = true);
	Model(std::string path, std::vector<InstanceData> instances, std::vector<Mesh> *meshes = nullptr);
	Model(GLchar* path, glm::vec3 translation);
	Model(GLchar* path, glm::vec3 translation, glm::vec3 rotation);
	Model(GLchar* path, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

	void Draw(Shader shader, bool instanced = true);

	void Translate(glm::vec3 translation);
	void Rotate(glm::vec3 rotation);
	void RotateX(double angle);
	void RotateY(double angle);
	void RotateZ(double angle);
	void Scale(glm::vec3 scale);

	glm::mat4 GetModelMatrix();
	glm::vec3 MeasureModel();
	glm::vec3 GetMeasure();

	void SetMeasure(glm::vec3 measure);

	glm::vec3 GetTranslation();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	std::string path;
	vector<Mesh> meshes;

#pragma region Boost serialization.

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & path;
		ar & translation;
		ar & rotation;
		ar & scale;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & path;
		ar & translation;
		ar & rotation;
		ar & scale;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

#pragma endregion

private:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	string directory;
	vector<Texture> textures_loaded;	

	glm::vec3 measure;

	void loadModel(string path, std::vector<InstanceData> instances, std::vector<Mesh> *meshes = nullptr);
	void processNode(aiNode* node, const aiScene* scene, std::vector<InstanceData> instances, std::vector<Mesh> *meshes = nullptr);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene, std::vector<InstanceData> instances, Mesh *m_mesh = nullptr);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint textureFromFile(const char* path, string directory);
	GLint textureFromColor(aiColor4D color);

	
};

#endif