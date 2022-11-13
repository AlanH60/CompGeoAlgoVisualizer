#include "PCH.h"
#include "Polygon.h"

using namespace D2D;

Polygon::Polygon(FLOAT2* vertices, unsigned int vertexCount, bool filled, Color color)
	:
	Drawable({ 0,0 }, color),
	mFilled(filled)
{
	pGeometry = std::unique_ptr<Geometry>(new Geometry(*pGraphics, vertices, vertexCount, filled));
}

void Polygon::draw()
{
	pGraphics->drawGeometry(pGeometry->get(), pBrush->get(), mFilled, mPos);
}
