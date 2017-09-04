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

	edges.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
		edges[i].resize(vertices.size());
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

int Graph::IndexOfVertex(GraphVertex * v)
{
	int u;
	for (int i = 0; i < vertices.size(); i++)
		if (v == vertices[i])
		{
			u = i;
			break;
		}
	return u;
}

int Graph::VerticesCount()
{
	return vertices.size();
}

int Graph::FindIndexOfVertex(GraphVertex * v)
{
	for (int i = 0; i < vertices.size(); i++)
		if (v == vertices[i])
			return i;
	return -1; // index not found
}

int Graph::FindIndexOfVector(double x, double y)
{
	for (int i = 0; i < vertices.size(); i++)
		if (vertices[i]->x == x && vertices[i]->y == y)
			return i;
	return -1;
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
	std::vector<Point2> points;
	std::vector<Segment> segments;
	voronoi_diagram<double> vd;

	std::vector<glm::vec2> mapPoints = map->GetPoints();
	for(int i = 0; i < mapPoints.size(); i++)
		segments.push_back(Segment(mapPoints[i].x, mapPoints[i].y, mapPoints[(i + 1) % mapPoints.size()].x, mapPoints[(i + 1) % mapPoints.size()].y));

	std::vector<MapElement*> mapElements = map->GetMapElements();
	for (int i = 0; i < mapElements.size(); i++)
	{
		std::vector<glm::vec2> mapElementPoints = mapElements[i]->GetPoints();
		for (int j = 0; j < mapElementPoints.size(); j++)
			segments.push_back(Segment(mapElementPoints[j].x, mapElementPoints[j].y, mapElementPoints[(j + 1) % mapElementPoints.size()].x, mapElementPoints[(j + 1) % mapElementPoints.size()].y));
	}

	construct_voronoi(points.begin(), points.end(), segments.begin(), segments.end(), &vd);

	vertices.resize(vd.num_vertices());
	for (int i = 0; i < vd.num_vertices(); i++)
		vertices[i] = new GraphVertex(0, 0);

	edges.resize(vd.num_vertices());
	for (int i = 0; i < vd.num_vertices(); i++)
		edges[i].resize(vd.num_vertices());

	for (int i = 0; i < vd.num_vertices(); i++)
		for (int j = 0; j < vd.num_vertices(); j++)
			edges[i][j] = nullptr;

	for (int i = 0; i < vd.num_vertices(); i++)
	{
		GraphVertex *v = new GraphVertex(vd.vertices().at(i).x(), vd.vertices().at(i).y());
		vertices[i] = v;
	}

	for (voronoi_diagram<double>::const_cell_iterator it = vd.cells().begin(); it != vd.cells().end(); ++it) {
		const voronoi_diagram<double>::cell_type &cell = *it;
		const voronoi_diagram<double>::edge_type *edge = cell.incident_edge();
		
		do
		{
			bool edgeAdmissible = true;
			if (edge != NULL && edge->vertex0() != NULL && edge->vertex1() != NULL)
			{
				auto mapElements = map->GetMapElements();
				for (int i = 0; i < mapElements.size(); i++)
				{
					auto pts = mapElements[i]->GetPoints();
					for (int j = 0; j < pts.size(); j++)
					{
						if (pts[j].x == edge->vertex0()->x() && pts[j].y == edge->vertex0()->y())
							edgeAdmissible = false;
						if (pts[j].x == edge->vertex1()->x() && pts[j].y == edge->vertex1()->y())
							edgeAdmissible = false;
					}
				}

				auto mapPoints = map->GetPoints();
				for (int i = 0; i < mapPoints.size(); i++)
				{
					if (mapPoints[i].x == edge->vertex0()->x() && mapPoints[i].y == edge->vertex0()->y())
						edgeAdmissible = false;
					if (mapPoints[i].x == edge->vertex1()->x() && mapPoints[i].y == edge->vertex1()->y())
						edgeAdmissible = false;
				}
			}
			else
				edgeAdmissible = false;

			if (edgeAdmissible)
			{
				int from = FindIndexOfVector(edge->vertex0()->x(), edge->vertex0()->y());
				int to = FindIndexOfVector(edge->vertex1()->x(), edge->vertex1()->y());

				AddEdge(from, to);
			}

			edge = edge->next();
		} while (edge != cell.incident_edge());
	}
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
		GraphVertex *vU = T.front();
		int u = IndexOfVertex(vU);

		for (auto v = T.begin(); v != T.end(); v++)
		{
			auto vIndex = IndexOfVertex(*v);
			if (dist[vIndex] + estimatedDist[vIndex][t] < dist[u] + estimatedDist[u][t])
				u = vIndex;
		}
		T.remove(vertices[u]);

		if (u == t) break;

		std::vector<GraphEdge*> neighbours = EdgesFrom(u); // the same as EdgesTo with assumption of indirection
		for (int w = 0; w < neighbours.size(); w++)
		{
			GraphEdge *e = GetEdge(u, w);
			if (e != NULL && dist[w] > dist[u] + e->weight)
			{
				dist[w] = dist[u] + e->weight;
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
	path.push_back(s);

	std::reverse(path.begin(), path.end());

	return path;
}
