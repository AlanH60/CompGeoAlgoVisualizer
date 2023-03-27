#include "PCH.h"
#include "Polygon.h"
#include "Rectangle.h"

using namespace D2D;

Polygon::Polygon(FLOAT2* vertices, unsigned int vertexCount, bool filled, Color color)
	:
	Drawable({ 0,0 }, color),
	mFilled(filled),
	mScale(1)
{
	pGeometry = std::unique_ptr<Geometry>(Geometry::generatePolygon(*pGraphics, vertices, vertexCount, filled));
}

void D2D::Polygon::setScale(float scale)
{
	mScale = scale;
}

void D2D::Polygon::pushLayer(D2D::Rectangle& rect, D2D1_MATRIX_3X2_F& transform)
{
	pGraphics->pushLayer({ rect.getPos().x, rect.getPos().y, rect.getPos().x + rect.getWidth(), rect.getPos().y + rect.getHeight() }, pGeometry->get(), transform);
}

void D2D::Polygon::popLayer()
{
	pGraphics->popLayer();
}

void Polygon::draw()
{
	if (isVisible)
		pGraphics->drawGeometry(pGeometry->get(), pBrush->get(), mFilled, mPos + mOffset, mScale);
}
