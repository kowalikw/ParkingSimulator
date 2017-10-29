#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

using namespace std;

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture 
{
	GLuint id;
	string type;
	aiString path;
};

struct InstanceData
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};

class Mesh 
{
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	vector<InstanceData> instances;

	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, vector<InstanceData> instances);
	void Draw(Shader shader, bool instanced = true);
	glm::vec3 GetMeshMinCoordinates();
	glm::vec3 GetMeshMaxCoordinates();

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO, IBO, tbo;

	/*  Functions    */
	void setupMesh();
};

#endif