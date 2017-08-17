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
	return vertices[v];
}

GraphEdge * Graph::GetEdge(int v1, int v2)
{
	return edges[v1][v2];
}

void Graph::AddVertex(double x, double y)
{
	GraphVertex *v = new GraphVertex(x, y);
	vertices.push_back(v);
}

void Graph::AddEdge(int v1, int v2)
{
	GraphEdge *e = new GraphEdge(vertices[v1], vertices[v2]);
	edges[v1][v2] = e;
}

void Graph::AddEdge(GraphVertex * v1, GraphVertex * v2)
{
	GraphEdge *e = new GraphEdge(v1, v2);
	
	int v1Index, v2Index;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (v1 == vertices[i])
			v1Index = i;
		if (v2 == vertices[i])
			v2Index = i;
	}

	edges[v1Index][v2Index] = e;
}

void Graph::RemoveVertex(int v)
{
	std::vector<GraphVertex*> newVertices;
	for (int i = 0; i < vertices.size(); i++)
		if(i != v)
			newVertices.push_back(vertices[i]);
	vertices = newVertices;

	std::vector<std::vector<GraphEdge*>> newEdges;
	int v1 = 0, v2 = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (i == v) continue;
		for (int j = 0; j < vertices.size(); j++)
		{
			if (j == v) continue;
			newEdges[v1][v2] = edges[i][j];
			v2++;
		}
		v1++;
	}
	edges = newEdges;
}

void Graph::RemoveVertex(GraphVertex * v)
{
	int vertexIndex;
	for (int i = 0; i < vertices.size(); i++)
		if (v == vertices[i])
			vertexIndex = i;
	RemoveVertex(vertexIndex);
}

void Graph::RemoveEdge(int v1, int v2)
{
	edges[v1][v2] = nullptr;
}

void Graph::RemoveEdge(GraphEdge * e)
{
	int edgeStart, edgeEnd;
	for(int i = 0; i < vertices.size(); i++)
		for(int j = 0; j < vertices.size(); j++)
			if (e == edges[i][j])
			{
				edgeStart = i;
				edgeEnd = j;
				break;
			}
	RemoveEdge(edgeStart, edgeEnd);
}
