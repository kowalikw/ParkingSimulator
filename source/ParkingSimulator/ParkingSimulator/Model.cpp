#include "Model.h"
#define INF 10e9;

Model::Model()
{
}

Model::Model(GLchar *path)
{
	this->path = path;
	this->loadModel(path, std::vector<InstanceData>());
}

Model::Model(std::string path, bool createModel)
{
	this->path = path;
	this->translation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);

	if(createModel)
		this->loadModel(path, std::vector<InstanceData>());
}

Model::Model(std::string path, std::vector<InstanceData> instances, std::vector<Mesh> *meshes)
{
	this->path = path;
	this->loadModel(path, instances, meshes);
}

Model::Model(GLchar *path, glm::vec3 translation) : Model(path)
{
	this->translation = translation;
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Model::Model(GLchar *path, glm::vec3 translation, glm::vec3 rotation) : Model(path, translation)
{
	this->rotation = rotation;
}

Model::Model(GLchar *path, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) : Model(path, translation, rotation)
{
	this->scale = scale;
}

void Model::Draw(Shader shader, bool instanced)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader, instanced);
}

void Model::Translate(glm::vec3 translation)
{
	this->translation = translation;
}

void Model::Rotate(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Model::RotateX(double angle)
{
	this->rotation.x = angle;
}

void Model::RotateY(double angle)
{
	this->rotation.y = angle;
}

void Model::RotateZ(double angle)
{
	this->rotation.z = angle;
}

void Model::Scale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::mat4x4 Model::GetModelMatrix()
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, this->translation);
	modelMatrix = glm::rotate(modelMatrix, this->rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, this->rotation.y, glm::vec3(0, 1, 0));	
	modelMatrix = glm::rotate(modelMatrix, this->rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, this->scale);
	return modelMatrix;
}

void Model::loadModel(string path, std::vector<InstanceData> instances, std::vector<Mesh> *meshes)
{
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Retrieve the directory path of the filepath
	this->directory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively
	this->processNode(scene->mRootNode, scene, instances, meshes);
}

void Model::processNode(aiNode* node, const aiScene* scene, std::vector<InstanceData> instances, std::vector<Mesh> *meshes)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene, instances, meshes != nullptr ? &meshes->at(i) : nullptr));
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene, instances, meshes);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<InstanceData> instances, Mesh *m_mesh)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	if (m_mesh == nullptr)
	{
		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			/*if (mesh->mNormals[i] != nullptr)
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}*/
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
	}
	else
	{
		vertices = m_mesh->vertices;
		indices = m_mesh->indices;
		instances = m_mesh->instances;
	}

	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	// Return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures, m_mesh != nullptr ? m_mesh->instances : instances);
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = textureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	aiColor4D color;
	if (textures.size() == 0 && aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
	{
		Texture texture;
		texture.id = textureFromColor(color);
		texture.type = typeName;
		texture.path = "";
		textures.push_back(texture);
		this->textures_loaded.push_back(texture);
	}

	return textures;
}

GLint Model::textureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

GLint Model::textureFromColor(aiColor4D color)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	float data[4];
	data[0] = color.r;
	data[1] = color.g;
	data[2] = color.b;
	data[3] = color.a;

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1, 1, 0, GL_RGBA, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

glm::vec3 Model::MeasureModel()
{
	double minX = INF;
	double maxX = -INF;
	double minY = INF;
	double maxY = -INF;
	double minZ = INF;
	double maxZ = -INF;

	for (int i = 0; i < meshes.size(); i++)
	{
		glm::vec3 minCoordinates = meshes[i].GetMeshMinCoordinates();
		glm::vec3 maxCoordinates = meshes[i].GetMeshMaxCoordinates();

		if (minCoordinates.x < minX) minX = minCoordinates.x;
		if (minCoordinates.y < minY) minY = minCoordinates.y;
		if (minCoordinates.z < minZ) minZ = minCoordinates.z;
		if (maxCoordinates.x > maxX) maxX = maxCoordinates.x;
		if (maxCoordinates.y > maxY) maxY = maxCoordinates.y;
		if (maxCoordinates.z > maxZ) maxZ = maxCoordinates.z;
	}

	this->measure = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);

	return this->measure;
}

glm::vec3 Model::GetMeasure()
{
	return this->measure;
}

void Model::SetMeasure(glm::vec3 measure)
{
	this->measure = measure;
}

glm::vec3 Model::GetTranslation()
{
	return this->translation;
}

glm::vec3 Model::GetRotation()
{
	return this->rotation;
}

glm::vec3 Model::GetScale()
{
	return this->scale;
}
