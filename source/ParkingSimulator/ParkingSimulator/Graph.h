#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include "Map.h"
#define INF 10e9;

struct GraphVertex
{
	double x;
	double y;
	GraphVertex() {}
	GraphVertex(double x, double y) : x(x), y(y) {}
};

struct GraphEdge
{
	GraphVertex *v1; // from
	GraphVertex *v2; // to
	double weight;
	GraphEdge() {}
	GraphEdge(GraphVertex *v1, GraphVertex *v2) : v1(v1), v2(v2) { weight = sqrt((v1->x - v2->x) * (v1->x - v2->x) + (v1->y - v2->y) * (v1->y - v2->y)); }
	GraphEdge(GraphVertex *v1, GraphVertex *v2, double weight) : v1(v1), v2(v2), weight(weight) { }
};

class Graph
{
public:
	Graph(bool directed = false);
	Graph(int verticesCount, bool directed = false);
	GraphVertex *GetVertex(int v);
	GraphEdge *GetEdge(int v1, int v2);
	void AddVertex(double x, double y);
	void AddEdge(int v1, int v2);
	void AddEdge(int v1, int v2, double weight);
	void AddEdge(GraphVertex *v1, GraphVertex *v2);
	void RemoveVertex(int v);
	void RemoveVertex(GraphVertex *v);
	void RemoveEdge(int v1, int v2);
	void RemoveEdge(GraphEdge *e);
	std::vector<GraphEdge*> EdgesFrom(int v);
	std::vector<GraphEdge*> EdgesTo(int v);

	void CreateVoronoiGraph(Map *map);
	std::vector<int> FindPath(int s, int t, double **estimatedDist);
private:
	bool directed;
	std::vector<GraphVertex*> vertices;
	std::vector<std::vector<GraphEdge*>> edges;
};

#endif