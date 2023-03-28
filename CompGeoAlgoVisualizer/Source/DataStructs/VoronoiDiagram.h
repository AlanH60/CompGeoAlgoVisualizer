#pragma once
#include "EdgeD.h"
#include "Vector2D.h"

class VoronoiDiagram
{
	public:
		struct HalfEdge;

		struct Vertex
		{
			union
			{
				struct {
					double x, y;
				};
				Vector2D v;
			};
			//Half Edge representing this vertex.  rep->tail == this.
			HalfEdge* rep = nullptr;

			Vertex(const Vector2D& v) : v(v) {}
		};

		struct Face
		{
			//Site associated with the voronoi face.
			Vector2D site;
			//Half Edge representing this face. rep->left == this.
			HalfEdge* rep = nullptr;

			Face(const Vector2D& site) : site(site) {}
		};

		//A directed edge.
		struct HalfEdge
		{
			union
			{
				struct {
					Vector2D v1, v2;
				};
				EdgeD e;
			};
			HalfEdge* next = nullptr; //The next half edge in the cycle.
			HalfEdge* prev = nullptr; //The previous half edge in the cycle.
			HalfEdge* twin = nullptr; //The half edge's twin(other side).
			Vertex* tail = nullptr; //The vertex that the half edge points from.
			Vertex* head = nullptr; //The vertix that the half edge points to.
			Face* left = nullptr; //Face to the left of this half edge.

			HalfEdge(const EdgeD& e) : e(e) {}
		};

	public:
		VoronoiDiagram();
		~VoronoiDiagram();

		void insertFace(Face* pFace);
		void insertHalfEdge(HalfEdge* pHalfEdge);
		void insertVertex(Vertex* pVertex);
		Vertex* getVertex(const Vector2D& vertex);
		Face* getFace(const Vector2D& site);

		std::map<EdgeD, VoronoiDiagram::HalfEdge*>& getHalfEdges();
	private:
		std::map<EdgeD, VoronoiDiagram::HalfEdge*> mHalfEdges;
		std::map<Vector2D, VoronoiDiagram::Vertex*> mVertices;
		std::map<Vector2D, VoronoiDiagram::Face*> mFaces;
};

