#include "PCH.h"
#include "IPanel.h"
#include "Direct2D/Drawable/Rectangle.h"

IPanel::IPanel(float width, float height, Color color)
	:
	IContainer(0, 0, width, height),
	pRect(new D2D::Rectangle({ 0, 0 }, width, height, true, color))
{
	mDrawables.push_back(pRect);
}

void IPanel::setWidth(float width)
{
	mWidth = width;
	pRect->setWidth(width);
}

void IPanel::setHeight(float height)
{
	mHeight = height;
	pRect->setHeight(height);
}
