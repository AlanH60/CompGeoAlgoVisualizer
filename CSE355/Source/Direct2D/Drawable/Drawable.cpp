#include "PCH.h"
#include "Drawable.h"

using namespace D2D;

Graphics* Drawable::pGraphics = nullptr;

Drawable::Drawable(FLOAT2 pos, Color color)
	:
	mPos(pos)
{
	isVisible = true;
	pBrush = std::unique_ptr<Brush>(new Brush(*pGraphics, color));
}

void Drawable::setPos(FLOAT2 pos)
{
	mPos = pos;
}

void Drawable::setColor(Color color)
{
	pBrush = std::unique_ptr<Brush>(new Brush(*pGraphics, color));
}

void D2D::Drawable::setVisibility(bool visibility)
{
	isVisible = visibility;
}

void Drawable::setGraphics(Graphics* gfx)
{
	pGraphics = gfx;
}

FLOAT2 Drawable::getPos()
{
	return mPos;
}

FLOAT2 D2D::Drawable::getOffset()
{
	return mOffset;
}

void Drawable::setOffset(FLOAT2 offset)
{
	mOffset = offset;
}


