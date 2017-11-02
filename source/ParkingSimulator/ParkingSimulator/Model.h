#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glew.h> // Contains all the necessery OpenGL includes
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
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model();
	Model(GLchar* path);
	Model(std::string path, bool createModel = true);
	Model(std::string path, std::vector<InstanceData> instances, std::vector<Mesh> *meshes = nullptr);
	Model(GLchar* path, glm::vec3 translation);
	Model(GLchar* path, glm::vec3 translation, glm::vec3 rotation);
	Model(GLchar* path, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

	// Draws the model, and thus all its meshes
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


	/* Model matrix data */
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	/*  Functions   */
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string path, std::vector<InstanceData> instances, std::vector<Mesh> *meshes = nullptr);

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene, std::vector<InstanceData> instances, std::vector<Mesh> *meshes = nullptr);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene, std::vector<InstanceData> instances, Mesh *m_mesh = nullptr);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint textureFromFile(const char* path, string directory);
	
	GLint textureFromColor(aiColor4D color);

	glm::vec3 measure;
};

#endif