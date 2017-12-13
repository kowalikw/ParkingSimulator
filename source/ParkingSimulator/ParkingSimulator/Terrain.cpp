#include "Terrain.h"

Terrain::Terrain()
{
	this->modelPath = "Resources/models/terrains/grass/grass.obj";
	this->color = glm::vec3(20, 175, 5);
	this->position = glm::vec2();
}

Terrain::Terrain(glm::vec2 position)
{
	this->modelPath = "Resources/models/terrains/grass/grass.obj";
	this->color = glm::vec3(20, 175, 5);
	this->position = position;
}

Terrain::Terrain(std::string modelPath, glm::vec3 color, glm::vec2 position)
{
	this->modelPath = modelPath;
	this->color = color;
	this->position = position;
}

Terrain::~Terrain()
{
}

std::string Terrain::GetModelPath()
{
	return this->modelPath;
}

glm::vec3 Terrain::GetColor()
{
	return this->color;
}

glm::vec2 Terrain::GetPosition()
{
	return this->position;
}

void Terrain::SetModelPath(std::string modelPath)
{
	this->modelPath = modelPath;
}

void Terrain::SetColor(glm::vec3 color)
{
	this->color = color;
}

void Terrain::SetPosition(glm::vec2 position)
{
	int i = (int)(position.x / TERRAIN_SLICE_SIZE);
	int j = (int)(position.y / TERRAIN_SLICE_SIZE);

	this->position = glm::vec2(i * TERRAIN_SLICE_SIZE, j * TERRAIN_SLICE_SIZE);
}
