#include "PCH.h"
#include "IPanel.h"
#include "Direct2D/Drawable/Rectangle.h"

IPanel::IPanel(int width, int height, Color color)
	:
	IContainer(0, 0, width, height),
	pRect(new D2D::Rectangle({ 0, 0 }, width, height, true, color))
{
	mDrawables.push_back(pRect);
}

void IPanel::setWidth(int width)
{
	mWidth = width;
	pRect->setWidth(width);
}

void IPanel::setHeight(int height)
{
	mHeight = height;
	pRect->setHeight(height);
}
