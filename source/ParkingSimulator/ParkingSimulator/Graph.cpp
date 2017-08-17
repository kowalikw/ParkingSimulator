#include "Graph.h"

Graph::Graph(int verticesCount)
{
	vertices.resize(verticesCount);
	for (int i = 0; i < verticesCount; i++)
		vertices[i] = new GraphVertex(0, 0);

	edges.resize(verticesCount);
	for (int i = 0; i < verticesCount; i++)
		edges[i].resize(verticesCount);

	for (int i = 0; i < verticesCount; i++)
		for (int j = 0; j < verticesCount; j++)
			edges[i][j] = nullptr;
}

GraphVertex * Graph::GetVertex(int v)
{
	return this->vertices[v];
}

GraphEdge * Graph::GetEdge(int v1, int v2)
{
	return this->edges[v1][v2];
}

void Graph::AddVertex(double x, double y)
{
}

void Graph::AddEdge(int v1, int v2)
{
}

void Graph::AddEdge(GraphVertex * v1, GraphVertex * v2)
{
}

void Graph::RemoveVertex(int v)
{
}

void Graph::RemoveVertex(GraphVertex * v)
{
}

void Graph::RemoveEdge(int v1, int v2)
{
}

void Graph::RemoveEdge(GraphEdge * e)
{
}
