#include "PCH.h"
#include "Line.h"

Line::Line(FLOAT2 p1, FLOAT2 p2, Color color)
	:
	Drawable({ 0, 0 }, color),
	mP1(p1),
	mP2(p2)
{
}

void Line::draw()
{
	pGraphics->drawLine(mPos + mP1, mPos + mP2, pBrush->get());
}

void Line::setPoints(FLOAT2 p1, FLOAT2 p2)
{
	mP1 = p1;
	mP2 = p2;
}
