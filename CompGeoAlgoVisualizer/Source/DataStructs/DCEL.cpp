#include "PCH.h"
#include "DCEL.h"
#include "../Primatives.h"

DCEL::DCEL(std::unordered_map<Vector2f, std::vector<Vector2f>>& edges)
{
	for (auto it = edges.begin(); it != edges.end(); it++)
	{
		if (mVertices.find(it->first) == mVertices.end())
			mVertices[it->first] = new DCEL::Vertex({ it->first });
		for (Vector2f& v : it->second)
		{
			if (mVertices.find(v) == mVertices.end())
				mVertices[v] = new DCEL::Vertex(v);
			if (mHalfEdges.find({ it->first, v }) == mHalfEdges.end())
			{
				DCEL::HalfEdge* pEdge1 = new DCEL::HalfEdge({ it->first, v });
				pEdge1->tail = mVertices[it->first];
				mVertices[it->first]->rep = pEdge1;
				DCEL::HalfEdge* pEdge2 = new DCEL::HalfEdge({ v, it->first });
				pEdge2->tail = mVertices[v];
				mVertices[v]->rep = pEdge2;
				pEdge1->twin = pEdge2;
				pEdge2->twin = pEdge1;
				mHalfEdges[{ it->first, v}] = pEdge1;
				mHalfEdges[{ v, it->first }] = pEdge2;
			}
		}
		std::sort(it->second.begin(), it->second.end(), [it](const Vector2f& v1, const Vector2f& v2)
			{
				Vector2f toV1 = v1 - it->first;
				Vector2f toV2 = v2 - it->first;
				if (toV1.y * toV2.y <= 0) //If one of them is negative.
					return toV1.y < toV2.y;
				//They're y's have the same sign.
				float dot1 = dot({ 1, 0 }, normalize(toV1));
				float dot2 = dot({ 1, 0 }, normalize(toV2));
				return (toV1.y > 0) ? dot1 < dot2 : dot1 > dot2;
			});
		if (it->second.size() == 1)
		{
			DCEL::HalfEdge* pEdge = mHalfEdges[{ it->first, it->second[0] }];
			pEdge->twin->next = pEdge;
			pEdge->prev = pEdge->twin;
		}
		else
		{
			for (size_t i = 0; i < it->second.size(); i++)
			{
				size_t next = (i + 1) % it->second.size();
				DCEL::HalfEdge* pEdge1 = mHalfEdges[{ it->first, it->second[i] }];
				DCEL::HalfEdge* pEdge2 = mHalfEdges[{ it->first, it->second[next]}];
				pEdge1->twin->next = pEdge2;
				pEdge2->prev = pEdge1->twin;
			}
		}

	}
	for (auto it = mHalfEdges.begin(); it != mHalfEdges.end(); it++)
	{
		DCEL::HalfEdge* pEdge = it->second;
		if (!pEdge->left)
		{
			DCEL::Face* pFace = new DCEL::Face();
			mFaces.push_back(pFace);
			pFace->rep = pEdge;
			do
			{
				pEdge->left = pFace;
				pEdge = pEdge->next;
			} while (pEdge != it->second);
		}
	}
}

DCEL::~DCEL()
{
	for (DCEL::Face* face : mFaces)
		delete face;
	for (auto& pair : mHalfEdges)
		delete pair.second;
	for (auto& pair : mVertices)
		delete pair.second;
	mFaces.clear();
	mHalfEdges.clear();
	mVertices.clear();

}

std::unordered_map<Edge, DCEL::HalfEdge*>& DCEL::getHalfEdges()
{
	return mHalfEdges;
}

std::unordered_map<Vector2f, DCEL::Vertex*> DCEL::getVertices()
{
	return mVertices;
}

std::vector<DCEL::Face*> DCEL::getFaces()
{
	return mFaces;
}
