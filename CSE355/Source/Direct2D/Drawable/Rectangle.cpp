#include "PCH.h"
#include "Rectangle.h"

using namespace D2D;

Rectangle::Rectangle(FLOAT2 pos, float width, float height, bool filled, Color color)
	:
	Drawable(pos, color),
	mWidth(width),
	mHeight(height),
	mFilled(filled),
	mCornerRadius(0)
{
}

Rectangle::Rectangle(FLOAT2 pos, float width, float height, float cornerRadius, bool filled, Color color)
	:
	Drawable(pos, color),
	mWidth(width),
	mHeight(height),
	mCornerRadius(cornerRadius),
	mFilled(filled)
{
}

void Rectangle::draw()
{
	pGraphics->drawRect(mPos, mWidth, mHeight, mFilled, pBrush->get(), mCornerRadius);
}

float Rectangle::getWidth()
{
	return mWidth;
}

float Rectangle::getHeight()
{
	return mHeight;
}

float Rectangle::getCornerRadius()
{
	return mCornerRadius;
}

void Rectangle::setWidth(float width)
{
	mWidth = width;
}

void Rectangle::setHeight(float height)
{
	mHeight = height;
}

void Rectangle::setCornerRadius(float cornerRadius)
{
	mCornerRadius = cornerRadius;
}
