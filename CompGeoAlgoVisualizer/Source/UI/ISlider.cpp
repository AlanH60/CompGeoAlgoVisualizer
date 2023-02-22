#include "PCH.h"
#include "ISlider.h"
#include "Direct2D/Drawable/Rectangle.h"

using D2D::Rectangle;

ISlider::ISlider(float* pValue, float min, float max, float width, float height)
	:
	IComponent(0, 0, width, height),
	pValue(pValue),
	mMin(min),
	mMax(max)
{
	pHorizontalBar = new D2D::Rectangle({ 0, 0 }, width, height * HORIZONTAL_HEIGHT, height * HORIZONTAL_HEIGHT / 2, true, Color{ 0.33f, 0.33f, 0.33f, 1 });
	pHorizontalBar->setOffset({ 0, 3.0f * height / 8 });
	float prog = (*pValue - min) / (max - min);
	pSliderBar = new D2D::Rectangle({ 0, 0 }, height * SLIDER_WIDTH, height, height * SLIDER_WIDTH / 2, true, Color{ 0.75f, 0.75f, 0.75f, 1 });
	pSliderBar->setOffset({ prog * width - pSliderBar->getWidth() / 2, 0});
	mDrawables.push_back(pHorizontalBar);
	mDrawables.push_back(pSliderBar);
}

void ISlider::setWidth(float width)
{
	IComponent::setWidth(width);
	pHorizontalBar->setWidth(width);
	float prog = (*pValue - mMin) / (mMax - mMin);
	pSliderBar->setOffset({ prog * mWidth - pSliderBar->getWidth() / 2, 0 });
}

void ISlider::setHeight(float height)
{
	IComponent::setHeight(height);
	pSliderBar->setHeight(height);
	pSliderBar->setWidth(height * SLIDER_WIDTH);
	pSliderBar->setCornerRadius(height * SLIDER_WIDTH / 2);
	pHorizontalBar->setHeight(height * HORIZONTAL_HEIGHT);
	pHorizontalBar->setOffset({ 0, 3.0f * height / 8 });
	pHorizontalBar->setCornerRadius(height * HORIZONTAL_HEIGHT / 2);
}

void ISlider::onPress(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (!pSliderBar->inRect(FLOAT2{ (float)x, (float)y }))
	{
		float newX = (x - mPos.x) - pSliderBar->getWidth() / 2;
		newX = std::max(newX, (-pSliderBar->getWidth() / 2));
		newX = std::min(newX, (mWidth - pSliderBar->getWidth() / 2));
		pSliderBar->setOffset({ newX, 0 });

		float prog = (newX + pSliderBar->getWidth() / 2) / (mWidth);
		*pValue = mMin + (prog * (mMax - mMin));
	}
	isPressed = true;
	mouseEvent.isConsumed = true;
}


void ISlider::onMove(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pSliderBar->inRect(FLOAT2{ (float)x, (float)y }))
	{
		pSliderBar->setColor({ 0.85f, 0.85, 0.85f, 1 });
		pSliderBar->setBorderColor({ 0.85f, 0.85, 0.85f, 1 });
	}
	else
	{
		pSliderBar->setColor({ 0.75f, 0.75f, 0.75f, 1 });
		pSliderBar->setBorderColor({ 0.75f, 0.75f, 0.75f, 1 });
	}
	mouseEvent.isConsumed = true;
}

void ISlider::onExit()
{
	pSliderBar->setColor({ 0.75f, 0.75f, 0.75f, 1 });
}

void ISlider::onDrag(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (isPressed)
	{
		float newX = (x - mPos.x) - pSliderBar->getWidth() / 2;
		newX = std::max(newX, (-pSliderBar->getWidth() / 2));
		newX = std::min(newX, (mWidth - pSliderBar->getWidth() / 2));
		pSliderBar->setOffset({ newX, 0 });

		float prog = (newX + pSliderBar->getWidth() / 2) / (mWidth);
		*pValue = mMin + (prog * (mMax - mMin));
	}
	mouseEvent.isConsumed = true;
}

void ISlider::onDragRelease(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	isPressed = false;
	mouseEvent.isConsumed = true;
}

void ISlider::onClick(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	isPressed = false;
	mouseEvent.isConsumed = true;
}

void ISlider::onUpdate(IComponent* parent)
{
	IComponent::onUpdate(parent);
	float prog = (*pValue - mMin) / (mMax - mMin);
	pSliderBar->setOffset({ prog * mWidth - pSliderBar->getWidth() / 2, 0 });
}

bool ISlider::inComponent(float x, float y)
{
	return pSliderBar->inRect({ (float)x, (float)y }) || pHorizontalBar->inRect({ (float)x, (float)y });
}

