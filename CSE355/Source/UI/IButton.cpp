#include "PCH.h"
#include "IButton.h"
#include "Direct2D/Drawable/Text.h"
#include "Direct2D/Drawable/Rectangle.h"

using namespace D2D;

IButton::IButton(std::wstring text, int width, int height)
	:
	IButton(text, TextFormat(L"Arial", 14, true, Style::NORMAL, TextAlignment::CENTER, ParagraphAlignment::CENTER), width, height)
{
}

IButton::IButton(std::wstring text, TextFormat& textFormat, int width, int height)
	:
	IComponent(0, 0, width, height),
	pText(new Text(text, textFormat, width, height)),
	pRectangle(new D2D::Rectangle({ 0, 0 }, width, height, 5, true, Color{ 1, 0.3f, 0.3f, 1 }))
{
	pText->setColor({ 0.95f, 0.95f, 0.95f, 1 });
	mDrawables.push_back(pRectangle);
	mDrawables.push_back(pText);
}

void IButton::setWidth(int width)
{
	IComponent::setWidth(width);
	pRectangle->setWidth(width);
	pText->setWidth(width);
}

void IButton::setHeight(int height)
{
	IComponent::setHeight(height);
	pRectangle->setHeight(height);
	pText->setHeight(height);
}

void IButton::onHover(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	pRectangle->setColor({ 0.95f, 0.95f, 0.95f, 1 });
	pRectangle->setBorderColor({ 0.95f, 0.95f, 0.95f, 1 });
	pText->setColor({ 1, 0.3f, 0.3f, 1 });
}

void IButton::onExit()
{
	pRectangle->setColor({ 1, 0.3f, 0.3f, 1 });
	pRectangle->setBorderColor({ 1, 0.3f, 0.3f, 1 });
	pText->setColor({ 0.95f, 0.95f, 0.95f, 1 });
}

void IButton::onPress(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	mouseEvent.isConsumed = true;
}

void IButton::onClick(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	mouseEvent.isConsumed = true;
	mOnClickHandler();
}

void IButton::setOnClick(const std::function<void()>& handler)
{
	mOnClickHandler = handler;
}

void IButton::setCornerRadius(int radius)
{
	pRectangle->setCornerRadius(radius);
}

bool IButton::inComponent(int x, int y)
{
	return pRectangle->inRect({ (float)x, (float)y });
}
