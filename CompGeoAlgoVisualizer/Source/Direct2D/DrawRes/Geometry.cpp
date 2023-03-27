#include "PCH.h"
#include "Geometry.h"
#include "../Graphics.h"
#include "../Types.h"

ID2D1PathGeometry* Geometry::get()
{
	return pGeometry.Get();
}

Geometry* Geometry::generatePolygon(Graphics& gfx, FLOAT2* vertices, unsigned int vertexCount, bool filled)
{
	Geometry* pPolygon = new Geometry();
	gfx.createPathGeometry(vertices, vertexCount, filled, pPolygon->pGeometry);
	return pPolygon;
}

Geometry* Geometry::generateQuadraticBezierCurve(Graphics& gfx, FLOAT2 startPoint, FLOAT2 controlPoint, FLOAT2 endPoint)
{
	Geometry* pCurve = new Geometry();
	gfx.createQuadraticBezierCurve(startPoint, controlPoint, endPoint, pCurve->pGeometry);
	return pCurve;
}
