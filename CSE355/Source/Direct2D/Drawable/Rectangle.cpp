#include "PCH.h"
#include "Rectangle.h"

using namespace D2D;

Rectangle::Rectangle(FLOAT2 pos, float width, float height, bool filled, Color color)
	:
	Drawable(pos, color),
	mWidth(width),
	mHeight(height),
	mFilled(filled),
	mCornerRadius(0),
	mBorderWidth(1),
	pBorderBrush(new Brush(*pGraphics, color))
{

}

Rectangle::Rectangle(FLOAT2 pos, float width, float height, float cornerRadius, bool filled, Color color)
	:
	Drawable(pos, color),
	mWidth(width),
	mHeight(height),
	mCornerRadius(cornerRadius),
	mFilled(filled),
	mBorderWidth(1),
	pBorderBrush(new Brush(*pGraphics, color))
{
}

Rectangle::Rectangle(FLOAT2 pos, float width, float height, float cornerRadius, float borderWidth, bool filled, Color color)
	:
	Drawable(pos, color),
	mWidth(width),
	mHeight(height),
	mCornerRadius(cornerRadius),
	mFilled(filled),
	mBorderWidth(borderWidth),
	pBorderBrush(new Brush(*pGraphics, color))
{
}

void Rectangle::draw()
{
	pGraphics->drawRect(mPos + mOffset, mWidth, mHeight, mFilled, pBrush->get(), pBorderBrush->get(), mCornerRadius, mBorderWidth);
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

float D2D::Rectangle::getBorderWidth()
{
	return mBorderWidth;
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

void D2D::Rectangle::setBorderColor(Color color)
{
	pBorderBrush = std::unique_ptr<Brush>(new Brush(*pGraphics, color));
}

void D2D::Rectangle::setBorderWidth(float borderWidth)
{
	mBorderWidth = borderWidth;
}
