#include "PCH.h"
#include "Line.h"

using namespace D2D;

Line::Line(FLOAT2 p1, FLOAT2 p2, Color color)
	:
	Drawable({ 0, 0 }, color),
	mP1(p1),
	mP2(p2)
{
}

void Line::draw()
{
	pGraphics->drawLine(mPos + mP1 + mOffset, mPos + mP2 + mOffset, pBrush->get());
}

FLOAT2 D2D::Line::getP1()
{
	return mP1;
}

FLOAT2 D2D::Line::getP2()
{
	return mP2;
}

void Line::setPoints(FLOAT2 p1, FLOAT2 p2)
{
	mP1 = p1;
	mP2 = p2;
}
