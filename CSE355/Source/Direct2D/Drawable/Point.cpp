#include "PCH.h"
#include "Point.h"


Point::Point(Graphics& gfx, FLOAT2 pos, Color color)
	: 
	Drawable(gfx, pos, color)
{}

void Point::draw(Graphics& gfx)
{
	gfx.drawPoint(mPos, pBrush->get());
}
