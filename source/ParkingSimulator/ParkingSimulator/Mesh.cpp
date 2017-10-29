#include "Mesh.h"
#define INF 10e9;

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, vector<InstanceData> instances)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->instances = instances;

	this->setupMesh();
}

void Mesh::Draw(Shader shader, bool instanced)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = this->textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // Transfer GLuint to stream
		else if (name == "texture_specular")
			ss << specularNr++; // Transfer GLuint to stream
		number = ss.str();

		glUniform1f(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	/*GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(shader.Program, "lightColor");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Also set light's color (white)*/

	// Draw mesh
	glBindVertexArray(this->VAO);
	/*if(instanced)
		glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, this->instances.size());
	else*/
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	glGenBuffers(1, &this->IBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),	(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	if (instances.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->IBO);
		glBufferData(GL_ARRAY_BUFFER, this->instances.size() * sizeof(InstanceData),
			&this->instances[0], GL_DYNAMIC_DRAW);

		// Instances Positions
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (GLvoid*)(0));
		glVertexAttribDivisor(3, 1);
		// Instances Positions
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (GLvoid*)offsetof(InstanceData, Rotation));
		glVertexAttribDivisor(4, 1);
		// Instances Positions
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (GLvoid*)offsetof(InstanceData, Scale));
		glVertexAttribDivisor(5, 1);
	}


	// ------------------

	//// the offsets
	//GLfloat translationData[] = {
	//	20.0f, 0.0f, 0.0f,  // cube 0
	//	40.0f, 0.0f, 0.0f,  // cube 1
	//	60.0f, 0.0f, 0.0f,  // cube 2
	//	80.0f, 0.0f, 0.0f,  // cube 3
	//	100.0f, 0.0f, 0.0f,  // cube 4
	//	120.0f, 0.0f, 0.0f,  // cube 5
	//	130.0f, 0.0f, 0.0f,  // cube 6
	//	140.0f, 0.0f, 0.0f,  // cube 7
	//}; // 8 offsets with 3 components each

	//// --------------------

	//// generate and bind the vertex buffer object containing the
	//// instance offsets
	//glGenBuffers(1, &tbo);
	//glBindBuffer(GL_ARRAY_BUFFER, this->tbo);

	//   // fill with data
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 8, translationData, GL_DYNAMIC_DRAW);

	//// set up generic attrib pointers
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0));
	//glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

glm::vec3 Mesh::GetMeshMinCoordinates()
{
	double minX = INF;
	double minY = INF;
	double minZ = INF;

	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].Position.x < minX && !isnan(vertices[i].Position.x)) minX = vertices[i].Position.x;
		if (vertices[i].Position.y < minY && !isnan(vertices[i].Position.y)) minY = vertices[i].Position.y;
		if (vertices[i].Position.z < minZ && !isnan(vertices[i].Position.z)) minZ = vertices[i].Position.z;
	}

	return glm::vec3(minX, minY, minZ);
}

glm::vec3 Mesh::GetMeshMaxCoordinates()
{
	double maxX = -INF;
	double maxY = -INF;
	double maxZ = -INF;

	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].Position.x > maxX && !isnan(vertices[i].Position.x)) maxX = vertices[i].Position.x;
		if (vertices[i].Position.y > maxY && !isnan(vertices[i].Position.y)) maxY = vertices[i].Position.y;
		if (vertices[i].Position.z > maxZ && !isnan(vertices[i].Position.z)) maxZ = vertices[i].Position.z;
	}

	return glm::vec3(maxX, maxY, maxZ);
}

