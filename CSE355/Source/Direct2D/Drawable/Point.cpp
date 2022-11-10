#include "PCH.h"
#include "Point.h"


Point::Point(FLOAT2 pos, Color color)
	: 
	Drawable(pos, color)
{}

void Point::draw()
{
	pGraphics->drawPoint(mPos, pBrush->get());
}
