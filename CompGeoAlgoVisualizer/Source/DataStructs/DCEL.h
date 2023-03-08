#pragma once
#include "Edge.h"
#include "Vector2f.h"

class DCEL
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

			Vertex(const Vector2f& v)
			{
				this->v = v;
			}
		};

		struct Face
		{
			//Half Edge representing this face. rep->left == this.
			HalfEdge* rep = nullptr;
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

			Face* left = nullptr; //Face to the left of this half edge.

			HalfEdge(const Edge& e)
			{
				this->e = e;
			}
		};
	public:
		/**
		* Given a set of edges and vertices, generates a DCEL, filling up the inputted containers.
		* @param edges map from a vertex to all the vertices it has edges to.
		*/
		DCEL(std::unordered_map<Vector2f, std::vector<Vector2f>>& edges);
		~DCEL();

		std::unordered_map<Edge, DCEL::HalfEdge*>& getHalfEdges();
		std::unordered_map<Vector2f, DCEL::Vertex*> getVertices();
		std::vector<DCEL::Face*> getFaces();

	private:
		std::unordered_map<Edge, DCEL::HalfEdge*> mHalfEdges;
		std::unordered_map<Vector2f, DCEL::Vertex*> mVertices;
		std::vector<DCEL::Face*> mFaces;
};

