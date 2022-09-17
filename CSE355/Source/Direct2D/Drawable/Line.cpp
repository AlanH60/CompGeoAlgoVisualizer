#include "PCH.h"
#include "Line.h"

Line::Line(Graphics& gfx, FLOAT2 p1, FLOAT2 p2, Color color)
	:
	Drawable(gfx, { 0, 0 }, color),
	mP1(p1),
	mP2(p2)
{
}

void Line::draw(Graphics& gfx)
{
	gfx.drawLine(mPos + mP1, mPos + mP2, pBrush->get());
}