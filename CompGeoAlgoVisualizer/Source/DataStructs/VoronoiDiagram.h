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
		~VoronoiDiagram();

		/*
		* Inserts a face into the voronoi diagram.
		* @param pFace pointer to the face.
		*/
		void insertFace(Face* pFace);
		/*
		* Inserts a half edge into the voronoi diagram.
		* @param pHalfEdge pointer to the half edge.
		*/
		void insertHalfEdge(HalfEdge* pHalfEdge);
		/*
		* Inserts a vertex into the voronoi diagram.
		* @param pVertex pointer to the vertex.
		*/
		void insertVertex(Vertex* pVertex);
		/*
		* Obtain a pointer to a vertex from the voronoi diagram.  If it doesn't exist, a new vertex is allocated.
		* @param vertex coordinate of the vertex.
		* @returns pointer to the VoronoiDiagram::Vertex.
		*/
		Vertex* getVertex(const Vector2D& vertex);
		/*
		* Obtain a pointer to a face from the voronoi diagram.  If it doesn't exist, a new face is allocated.
		* @param site coordinate of the site of the face.
		* @returns pointer to the VoronoiDiagram::Face.
		*/
		Face* getFace(const Vector2D& site);
		/*
		* @returns reference to the map of half edge pointers.
		*/
		std::map<EdgeD, VoronoiDiagram::HalfEdge*>& getHalfEdges();
	private:
		std::map<EdgeD, VoronoiDiagram::HalfEdge*> mHalfEdges;
		std::map<Vector2D, VoronoiDiagram::Vertex*> mVertices;
		std::map<Vector2D, VoronoiDiagram::Face*> mFaces;
};

