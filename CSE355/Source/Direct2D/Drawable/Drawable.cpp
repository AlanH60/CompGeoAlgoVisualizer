#include "PCH.h"
#include "Drawable.h"

Drawable::Drawable(Graphics& gfx, FLOAT2 pos, Color color)
	:
	mPos(pos)
{
	pBrush = std::unique_ptr<Brush>(new Brush(gfx, color));
}

void Drawable::setPos(FLOAT2 pos)
{
	mPos = pos;
}

FLOAT2 Drawable::getPos()
{
	return mPos;
}
