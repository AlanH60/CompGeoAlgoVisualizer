#pragma once
#include "../Primatives.h"
#include "RedBlackTree.h"
#include "Edge.h";

//Balanced binary tree used in triangulation sweep algorithm
class TriSweepLineStatus
{
	private:
		//Compare class used to compare edges.  
		bool edgeLess(const Edge& e1, const Edge& e2)
		{
			if (e1.v1 == e2.v1)
				return (e1.v2.x < e2.v2.x);
			float slope1 = (e1.v1.x - e1.v2.x) / (e1.v1.y - e1.v2.y);
			float slope2 = (e2.v1.x - e2.v2.x) / (e2.v1.y - e2.v2.y);
			return ((y - e1.v1.y) * slope1 + e1.v1.x) < ((y - e2.v1.y) * slope2 + e2.v1.x);
		}
	public:
		TriSweepLineStatus();
		/**
		* Inserts an edge into the sweep line status
		* @param edge edge to be inserted.
		*/
		bool insert(const Edge& edge);
		/**
		* Removes an edge into the sweep line status
		* @param edge edge to be removed.
		*/
		bool remove(const Edge& edge);
		/**
		* Searches for the neighbors(predecessor and successor) of the edge in the tree.
		* @param edge the element you want the neighbors of.
		* @param pPre pointer to the edge to be filled with the predecessor if one is found.
		* @param pSuc pointer to the edge to be filled with the successor if one is found.
		* @returns std::pair<bool, bool> where the each flag indicates whether a predecessor or successor is found respectively.
		*/
		std::pair<bool, bool> findNeighbors(const Edge& edge, Edge* pPre, Edge* pSuc);

		void setY(float y);
	private:
		//Balanced binary tree used to store the edges.
		RedBlackTree<Edge> mTree;
		//Current y
		float y;
};