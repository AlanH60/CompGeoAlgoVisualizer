#pragma once
#include "Edge.h"
#include "Vector2f.h"

class VoronoiDiagram
{
	public:
		struct HalfEdge;

		struct Vertex
		{
			union
			{
				struct {
					float x, y;
				};
				Vector2f v;
			};
			//Half Edge representing this vertex.  rep->tail == this.
			HalfEdge* rep = nullptr;

			Vertex(const Vector2f& v) : v(v) {}
		};

		struct Face
		{
			//Site associated with the voronoi face.
			Vector2f site;
			//Half Edge representing this face. rep->left == this.
			HalfEdge* rep = nullptr;

			Face(const Vector2f& site) : site(site) {}
		};

		//A directed edge.
		struct HalfEdge
		{
			union
			{
				struct {
					Vector2f v1, v2;
				};
				Edge e;
			};
			HalfEdge* next = nullptr; //The next half edge in the cycle.
			HalfEdge* prev = nullptr; //The previous half edge in the cycle.
			HalfEdge* twin = nullptr; //The half edge's twin(other side).
			Vertex* tail = nullptr; //The vertex that the half edge points from.
			Vertex* head = nullptr; //The vertix that the half edge points to.
			Face* left = nullptr; //Face to the left of this half edge.

			HalfEdge(const Edge& e) : e(e) {}
		};

	public:
		VoronoiDiagram();
		~VoronoiDiagram();

		void insertFace(Face* pFace);
		void insertHalfEdge(HalfEdge* pHalfEdge);
		void insertVertex(Vertex* pVertex);
		Vertex* getVertex(const Vector2f& vertex);
		Face* getFace(const Vector2f& site);

		std::unordered_map<Edge, VoronoiDiagram::HalfEdge*>& getHalfEdges();
	private:
		std::vector<void*> pointers;
		std::unordered_map<Edge, VoronoiDiagram::HalfEdge*> mHalfEdges;
		std::unordered_map<Vector2f, VoronoiDiagram::Vertex*> mVertices;
		std::unordered_map<Vector2f, VoronoiDiagram::Face*> mFaces;
};

