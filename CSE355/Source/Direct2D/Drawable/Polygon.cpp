#include "PCH.h"
#include "Polygon.h"

Polygon::Polygon(Graphics& gfx, FLOAT2* vertices, unsigned int vertexCount, bool filled, Color color)
	:
	Drawable(gfx, { 0,0 }, color),
	mFilled(filled)
{
	pGeometry = std::unique_ptr<Geometry>(new Geometry(gfx, vertices, vertexCount, filled));
}

void Polygon::draw(Graphics& gfx)
{
	gfx.drawGeometry(pGeometry->get(), pBrush->get(), mFilled, mPos);
}
