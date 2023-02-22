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
	if (isVisible)
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

bool D2D::Rectangle::inRect(FLOAT2 point)
{
	FLOAT2 topLeft = mPos + mOffset;
	if (point.x >= topLeft.x && point.x <= topLeft.x + mWidth && point.y >= topLeft.y && point.y <= topLeft.y + mHeight)
	{
		bool inInterior = true;
		if (mCornerRadius > 0)
		{
			float radius = std::min(mCornerRadius, std::min(mWidth / 2, mHeight / 2));
			float radiusSqrd = radius * radius;
			for (int i = 0; i <= 1; i ++)
				for (int j = 0; j <= 1; j++)
				{
					FLOAT2 c = topLeft + FLOAT2{ abs(i * mWidth - radius), abs(j * mHeight - radius)};
					if ( ((i == 0) ? point.x < c.x : point.x > c.x) && ((j == 0) ? point.y < c.y : point.y > c.y) )
						inInterior = false;
					FLOAT2 pointToC = c - point;
					if (pointToC.x * pointToC.x + pointToC.y * pointToC.y < radiusSqrd)
						return true;
				}
		}
		if (inInterior)
			return true;
	}
	return false;
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
