#include "PCH.h"
#include "Geometry.h"
#include "../Graphics.h"
#include "../Types.h"
Geometry::Geometry(Graphics& gfx, FLOAT2* vertices, unsigned int vertexCount, bool filled)
{
	gfx.createPathGeometry(vertices, vertexCount, filled, pGeometry);
}

ID2D1PathGeometry* Geometry::get()
{
	return pGeometry.Get();
}
