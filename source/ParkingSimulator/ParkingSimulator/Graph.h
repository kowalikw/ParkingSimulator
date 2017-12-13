#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <glm.hpp>

#include "Map.h"
#include "Path.h"

#include <boost/polygon/voronoi.hpp>
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;

#define INF 10e9;

struct Point2 {
	int a;
	int b;
	Point2(int x, int y) : a(x), b(y) {}
};

struct Segment {
	Point2 p0;
	Point2 p1;
	Segment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
};

template <>
struct boost::polygon::geometry_concept<Point2> {
	typedef point_concept type;
};

template <>
struct boost::polygon::point_traits<Point2> {
	typedef int coordinate_type;

	static inline coordinate_type get(
		const Point2& point, boost::polygon::orientation_2d orient) {
		return (orient == boost::polygon::HORIZONTAL) ? point.a : point.b;
	}
};

template <>
struct boost::polygon::geometry_concept<Segment> {
	typedef segment_concept type;
};

template <>
struct boost::polygon::segment_traits<Segment> {
	typedef int coordinate_type;
	typedef Point2 point_type;

	static inline point_type get(const Segment& segment, boost::polygon::direction_1d dir) {
		return dir.to_int() ? segment.p1 : segment.p0;
	}
};

struct GraphVertex
{
	double x;
	double y;
	GraphVertex() {}
	GraphVertex(double x, double y) : x(x), y(y) {}
	~GraphVertex() { }

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & x;
		ar & y;
	}
};

struct GraphEdge
{
	GraphVertex *v1; // from
	GraphVertex *v2; // to
	double weight;
	GraphEdge() { v1 = nullptr; v2 = nullptr; }
	GraphEdge(GraphVertex *v1, GraphVertex *v2) : v1(v1), v2(v2) 
	{ 
		if(v1 != nullptr && v2 != nullptr)
			weight = sqrt((v1->x - v2->x) * (v1->x - v2->x) + (v1->y - v2->y) * (v1->y - v2->y)); 
	}
	GraphEdge(GraphVertex *v1, GraphVertex *v2, double weight) : v1(v1), v2(v2), weight(weight) { }
	~GraphEdge() 
	{
		if (v1 != nullptr) delete v1;
		if (v2 != nullptr) delete v2;
	}

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & v1 != nullptr;
		ar & v2 != nullptr;

		if (v1 != nullptr) ar & *v1;
		if (v2 != nullptr) ar & *v2;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		bool isV1, isV2;

		ar & isV1;
		ar & isV2;

		if (isV1)
		{
			this->v1 = new GraphVertex();
			ar & *v1;
		}

		if (isV2)
		{
			this->v2 = new GraphVertex();
			ar & *v2;
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class Graph
{
public:
	Graph(bool directed = false);
	Graph(int verticesCount, bool directed = false);
	~Graph();

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
	int IndexOfVertex(GraphVertex *v);
	int VerticesCount();
	int FindIndexOfVertex(GraphVertex *v);
	int FindIndexOfVector(double x, double y);
	std::vector<GraphEdge*> EdgesFrom(int v);
	std::vector<GraphEdge*> EdgesTo(int v);

	void CreateVoronoiGraph(Map *map);
	void CreateVoronoiGraphWithExtraVertices(Map *map, int extraVerticesAlong, int extraVerticesAcross);
	void CreateVoronoiFullGraph(Map *map, bool addExtraVertices = false, int extraVerticesAlong = 0, int extraVerticesAcross = 0);
	void CreateVoronoiVisibilityFullGraph(Map *map, Line *from, Line *to, int *indexFrom, int *indexTo, bool addExtraVertices = false, int extraVerticesAlong = 0, int extraVerticesAcross = 0);
	Path *FindPath(int s, int t);

	bool CanCancelCalculation();

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & directed;
		ar & vertices.size();
		for (int i = 0; i < vertices.size(); i++)
			ar & *vertices[i];
		for (int i = 0; i < vertices.size(); i++)
			for (int j = 0; j < vertices.size(); j++)
			{
				if (edges[i][j] != nullptr)
					ar & *edges[i][j];
				else
					ar & GraphEdge(nullptr, nullptr);
			}
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		int verticesCount;
		GraphVertex *vertex;
		GraphEdge *edge;

		ar & directed;
		ar & verticesCount;

		for (int i = 0; i < verticesCount; i++)
		{
			vertex = new GraphVertex();
			ar & *vertex;
			this->vertices.push_back(vertex);
		}

		for (int i = 0; i < verticesCount; i++)
		{
			this->edges.push_back(std::vector<GraphEdge*>());
			for (int j = 0; j < verticesCount; j++)
			{
				edge = new GraphEdge();
				ar & *edge;
				if (edge->v1 != nullptr && edge->v2 != nullptr)
					this->edges[i].push_back(edge);
				else
					this->edges[i].push_back(nullptr);
			}
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
private:
	bool directed;
	std::vector<GraphVertex*> vertices;
	std::vector<std::vector<GraphEdge*>> edges;

	bool canCancelCalculation = true;
};

#endif