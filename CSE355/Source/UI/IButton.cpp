#include "PCH.h"
#include "IButton.h"
#include "Direct2D/Drawable/Rectangle.h"

using D2D::Text;
using D2D::Rectangle;

IButton::IButton(std::wstring text, float width, float height)
	:
	IButton(text, Text::FontFormat{ L"Georgia", 18, Text::Style::NORMAL, false }, width, height)
{
}

IButton::IButton(std::wstring text, D2D::Text::FontFormat& fontFormat, float width, float height)
	:
	pText(new Text(text, fontFormat, width, height))
{
	pRectangle = new D2D::Rectangle({ 0, 0 }, width, height, true, Color{ 0.8f, 0.8f, 0.8f, 1 });
	mDrawables.push_back(pRectangle);
	mDrawables.push_back(pText);
	mPosX = 0;
	mPosY = 0;
	
}
