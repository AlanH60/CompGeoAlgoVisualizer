#include "PCH.h"
#include "IButton.h"
#include "Direct2D/Drawable/Text.h"
#include "Direct2D/Drawable/Rectangle.h"

using namespace D2D;

IButton::IButton(const std::wstring& text, float width, float height)
	:
	IButton(text, TextFormat(L"Arial", 14, true, Style::NORMAL, TextAlignment::CENTER, ParagraphAlignment::CENTER), width, height)
{
}

IButton::IButton(const std::wstring& text, const TextFormat& textFormat, float width, float height)
	:
	IComponent(0, 0, width, height),
	pText(new Text(text, textFormat, width, height)),
	pRectangle(new D2D::Rectangle({ 0, 0 }, width, height, 5, true, Color{ 1, 0.3f, 0.3f, 1 }))
{
	mTextColor = Color{ 0.95f, 0.95f, 0.95f, 1 };
	pText->setColor(mTextColor);
	mDrawables.push_back(pRectangle);
	mDrawables.push_back(pText);
}

void IButton::setText(std::wstring text)
{
	pText->setText(text);
}

void IButton::setColor(const Color& color)
{
	IComponent::setColor(color);
	pRectangle->setColor(color);
	pRectangle->setBorderColor(color);
}

void IButton::setTextColor(const Color& color)
{
	mTextColor = color;
	pText->setColor(color);
}

void IButton::setWidth(float width)
{
	IComponent::setWidth(width);
	pRectangle->setWidth(width);
	pText->setWidth(width);
}

void IButton::setHeight(float height)
{
	IComponent::setHeight(height);
	pRectangle->setHeight(height);
	pText->setHeight(height);
}

void IButton::onHover(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	Color hoveredColor = Color{ mColor.r + (1 - mColor.r) / 2, mColor.g + (1 - mColor.g) / 2, mColor.b + (1 - mColor.b) / 2, mColor.a };
	pRectangle->setColor(hoveredColor);
	pRectangle->setBorderColor(hoveredColor);
	pText->setColor(Color{ mTextColor.r + (1 - mTextColor.r) / 2, mTextColor.g + (1 - mTextColor.g) / 2, 
					mTextColor.b + (1 - mTextColor.b) / 2, mTextColor.a });
}

void IButton::onExit()
{
	pRectangle->setColor(mColor);
	pRectangle->setBorderColor(mColor);
	pText->setColor(mTextColor);
}

void IButton::onPress(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	mouseEvent.isConsumed = true;
}

void IButton::onClick(float x, float y, MouseEvent& mouseEvent)
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

void IButton::setCornerRadius(float radius)
{
	pRectangle->setCornerRadius(radius);
}

bool IButton::inComponent(float x, float y)
{
	return pRectangle->inRect({ x, y });
}
