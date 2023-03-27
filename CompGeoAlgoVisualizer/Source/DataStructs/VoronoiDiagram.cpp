#include "PCH.h"
#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram()
{
}

VoronoiDiagram::~VoronoiDiagram()
{
	mFaces.clear();
	mHalfEdges.clear();
	mVertices.clear();
	for (int i = 0; i < pointers.size(); i++)
		delete pointers[i];
	pointers.clear();

}

void VoronoiDiagram::insertFace(Face* pFace)
{
	pointers.push_back(pFace);
	mFaces[pFace->site] = pFace;
}

void VoronoiDiagram::insertHalfEdge(HalfEdge* pHalfEdge)
{
	pointers.push_back(pHalfEdge);
	mHalfEdges[pHalfEdge->e] = pHalfEdge;
}

void VoronoiDiagram::insertVertex(Vertex* pVertex)
{
	pointers.push_back(pVertex);
	mVertices[pVertex->v] = pVertex;
}

VoronoiDiagram::Vertex* VoronoiDiagram::getVertex(const Vector2f& vertex)
{
	auto it = mVertices.find(vertex);
	if (it == mVertices.end())
	{
		Vertex* v = new Vertex(vertex);
		insertVertex(v);
		return v;
	}
	else
		return it->second;
}

VoronoiDiagram::Face* VoronoiDiagram::getFace(const Vector2f& site)
{
	auto it = mFaces.find(site);
	if (it == mFaces.end())
	{
		Face* f = new Face(site);
		insertFace(f);
		return f;
	}
	else
		return it->second;
}

std::unordered_map<Edge, VoronoiDiagram::HalfEdge*>& VoronoiDiagram::getHalfEdges()
{
	return mHalfEdges;
}
