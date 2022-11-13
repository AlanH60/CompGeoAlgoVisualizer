#include "PCH.h"
#include "IButton.h"
#include "Direct2D/Drawable/Rectangle.h"

using D2D::Text;
using D2D::Rectangle;

IButton::IButton(std::wstring text, float width, float height)
	:
	IButton(text, Text::TextFormat{ L"Arial", 14, Text::Style::NORMAL, Text::Alignment::CENTER, true }, width, height)
{
}

IButton::IButton(std::wstring text, D2D::Text::TextFormat& textFormat, float width, float height)
	:
	IComponent(0, 0, width, height),
	pText(new Text(text, textFormat, width, height))
{
	pRectangle = new D2D::Rectangle({ 0, 0 }, width, height, 5, true, Color{ 1, 0.3f, 0.3f, 1 });
	pText->setColor({ 0.95f, 0.95f, 0.95f, 1 });
	mDrawables.push_back(pRectangle);
	mDrawables.push_back(pText);
	
	
}

void IButton::onHover(int x, int y)
{
	pRectangle->setColor({ 0.95f, 0.95f, 0.95f, 1 });
	pText->setColor({ 1, 0.3f, 0.3f, 1 });
}

void IButton::onExit()
{
	pRectangle->setColor({ 1, 0.3f, 0.3f, 1 });
	pText->setColor({ 0.95f, 0.95f, 0.95f, 1 });
}

void IButton::onClick(int x, int y)
{
	mOnClickHandler();
}

void IButton::setOnClick(const std::function<void()>& handler)
{
	mOnClickHandler = handler;
}
