#include "PCH.h"
#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram()
{
}

VoronoiDiagram::~VoronoiDiagram()
{
	for (auto& face : mFaces)
		delete face.second;
	for (auto& halfEdge : mHalfEdges)
		delete halfEdge.second;
	for (auto& vertex : mVertices)
		delete vertex.second;
}

void VoronoiDiagram::insertFace(Face* pFace)
{
	mFaces[pFace->site] = pFace;
}

void VoronoiDiagram::insertHalfEdge(HalfEdge* pHalfEdge)
{
	mHalfEdges[pHalfEdge->e] = pHalfEdge;
}

void VoronoiDiagram::insertVertex(Vertex* pVertex)
{
	mVertices[pVertex->v] = pVertex;
}

VoronoiDiagram::Vertex* VoronoiDiagram::getVertex(const Vector2D& vertex)
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

VoronoiDiagram::Face* VoronoiDiagram::getFace(const Vector2D& site)
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

std::map<EdgeD, VoronoiDiagram::HalfEdge*>& VoronoiDiagram::getHalfEdges()
{
	return mHalfEdges;
}
