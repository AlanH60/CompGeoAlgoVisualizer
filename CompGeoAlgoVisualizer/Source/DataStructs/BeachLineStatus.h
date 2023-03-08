#pragma once
#include "../Primatives.h"
#include "RedBlackTree.h"

//Balanced binary tree used in triangulation sweep algorithm
class BeachLineStatus
{
	private:
		struct BeachNode
		{

			bool isEdge;
		};
	public:
		BeachLineStatus();
		/**
		* Inserts a point that will be the focus of a new parabolic arc into the beach line.
		* @param point focus of the arc to be inserted.
		*/
		bool insert(const Vector2f& point);
		/**
		* Removes an arc from the beach line.
		* @param point focus of the arc to be removed.
		*/
		bool remove(const Vector2f& point);
		/**
		* Searches for the neighbors(predecessor and successor) of the arc in the tree.
		* @param edge the element you want the neighbors of.
		* @param pPre pointer to the edge to be filled with the predecessor if one is found.
		* @param pSuc pointer to the edge to be filled with the successor if one is found.
		* @returns std::pair<bool, bool> where the each flag indicates whether a predecessor or successor is found respectively.
		*/
		std::pair<bool, bool> findNeighbors(const Vector2f& point, Vector2f* pPre, Vector2f* pSuc);

		void setY(float y);
	private:
		//Balanced binary tree used to store the edges.
		RedBlackTree<Vector2f> mTree;
		//Current y
		float y;
};