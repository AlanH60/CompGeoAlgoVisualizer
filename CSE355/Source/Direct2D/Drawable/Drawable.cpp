#include "PCH.h"
#include "Drawable.h"

Graphics* Drawable::pGraphics = nullptr;

Drawable::Drawable(Graphics& gfx, FLOAT2 pos, Color color)
	:
	mPos(pos)
{
	if (!pGraphics)
		pGraphics = &gfx;
	pBrush = std::unique_ptr<Brush>(new Brush(gfx, color));
}

void Drawable::setPos(FLOAT2 pos)
{
	mPos = pos;
}

void Drawable::setColor(Color color)
{
	pBrush = std::unique_ptr<Brush>(new Brush(*pGraphics, color));
}

FLOAT2 Drawable::getPos()
{
	return mPos;
}


