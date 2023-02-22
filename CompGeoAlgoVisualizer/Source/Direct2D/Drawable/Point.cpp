#include "PCH.h"
#include "Point.h"

using namespace D2D;

Point::Point(FLOAT2 pos, Color color)
	: 
	Drawable(pos, color)
{}

void Point::draw()
{
	if (isVisible)
		pGraphics->drawPoint(mPos + mOffset, pBrush->get());
}
