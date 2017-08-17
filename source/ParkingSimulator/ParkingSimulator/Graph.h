#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

struct GraphVertex
{
	double x;
	double y;
	GraphVertex() {}
	GraphVertex(double x, double y) : x(x), y(y) {}
};

struct GraphEdge
{
	GraphVertex *v1;
	GraphVertex *v2;
	double weight;
	GraphEdge() {}
	GraphEdge(GraphVertex *v1, GraphVertex *v2) : v1(v1), v2(v2) { weight = sqrt((v1->x - v2->x) * (v1->x - v2->x) + (v1->y - v2->y) * (v1->y - v2->y)); }
};

class Graph
{
public:
	Graph(int verticesCount);
	GraphVertex *GetVertex(int v);
	GraphEdge *GetEdge(int v1, int v2);
	void AddVertex(double x, double y);
	void AddEdge(int v1, int v2);
	void AddEdge(GraphVertex *v1, GraphVertex *v2);
	void RemoveVertex(int v);
	void RemoveVertex(GraphVertex *v);
	void RemoveEdge(int v1, int v2);
	void RemoveEdge(GraphEdge *e);
private:
	std::vector<GraphVertex*> vertices;
	std::vector<std::vector<GraphEdge*>> edges;
};

#endif