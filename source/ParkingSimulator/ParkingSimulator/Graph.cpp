#include "Graph.h"

Graph::Graph(bool directed)
{
	this->directed = directed;
}

Graph::Graph(int verticesCount, bool directed)
{
	this->directed = directed;

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
	if (!directed)
		edges[v2][v1] = e;
}

void Graph::AddEdge(int v1, int v2, double weight)
{
	GraphEdge *e = new GraphEdge(vertices[v1], vertices[v2], weight);
	edges[v1][v2] = e;
	if (!directed)
		edges[v2][v1] = e;
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

	AddEdge(v1Index, v2Index);
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

std::vector<GraphEdge*> Graph::EdgesFrom(int v)
{
	std::vector<GraphEdge*> edgesFrom;

	for (int i = 0; i < vertices.size(); i++)
		if (edges[v][i] != NULL)
			edgesFrom.push_back(edges[v][i]);

	return edgesFrom;
}

std::vector<GraphEdge*> Graph::EdgesTo(int v)
{
	std::vector<GraphEdge*> edgesTo;

	for (int i = 0; i < vertices.size(); i++)
		if (edges[i][v] != NULL)
			edgesTo.push_back(edges[i][v]);

	return edgesTo;
}

void Graph::CreateVoronoiGraph(Map * map)
{

}

std::vector<int> Graph::FindPath(int s, int t, double **estimatedDist)
{
	std::vector<int> path;
	std::list<GraphVertex*> T;
	std::vector<double> dist(vertices.size());
	std::vector<int> prev(vertices.size());

	for (int i = 0; i < vertices.size(); i++)
	{
		dist[i] = INF;
		prev[i] = -1;
		T.push_back(vertices[i]);
	}
	dist[s] = 0;

	while (!T.empty())
	{
		int u;
		GraphVertex *vU = T.front();
		for(int i = 0; i < vertices.size(); i++)
			if (vU == vertices[i])
			{
				u = i;
				break;
			}

		for (int i = 0; i < T.size(); i++)
			if (dist[i] + estimatedDist[i][t] < dist[u] + estimatedDist[u][t])
				u = i;
		T.remove(vertices[u]);

		if (u == t) break;

		std::vector<GraphEdge*> neighbours = EdgesFrom(u); // the same as EdgesTo with assumption of indirection
		for (int w = 0; w < neighbours.size(); w++)
		{
			if (dist[w] > dist[u] + GetEdge(u, w)->weight)
			{
				dist[w] = dist[u] + GetEdge(u, w)->weight;
				prev[w] = u;
			}
		}
	}

	int w = t;
	while (prev[w] != -1)
	{
		path.push_back(w);
		w = prev[w];
	}

	return path;
}
