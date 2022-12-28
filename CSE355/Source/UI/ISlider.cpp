#include "PCH.h"
#include "ISlider.h"
#include "Direct2D/Drawable/Rectangle.h"

using D2D::Rectangle;

ISlider::ISlider(float* pValue, float min, float max, int width, int height)
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

void ISlider::onPress(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	int relativeX = x - mPosX;
	int relativeY = y - mPosY;
	int sliderX = pSliderBar->getOffset().x;
	int sliderY = pSliderBar->getOffset().y;
	if (!(relativeX >= sliderX && relativeX <= sliderX + pSliderBar->getWidth() &&
		relativeY >= sliderY && relativeY <= sliderY + pSliderBar->getHeight()))
	{
		float newX = (x - mPosX) - pSliderBar->getWidth() / 2;
		newX = std::max(newX, (-pSliderBar->getWidth() / 2));
		newX = std::min(newX, (mWidth - pSliderBar->getWidth() / 2));
		pSliderBar->setOffset({ newX, 0 });

		float prog = (newX + pSliderBar->getWidth() / 2) / (mWidth);
		*pValue = mMin + (prog * (mMax - mMin));
	}
	isPressed = true;
	mouseEvent.isConsumed = true;
}


void ISlider::onMove(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	int relativeX = x - mPosX;
	int relativeY = y - mPosY;
	int sliderX = pSliderBar->getOffset().x;
	int sliderY = pSliderBar->getOffset().y;
	if (relativeX >= sliderX && relativeX <= sliderX + pSliderBar->getWidth() && 
		relativeY >= sliderY && relativeY <= sliderY + pSliderBar->getHeight())
		pSliderBar->setColor({ 0.85f, 0.85, 0.85f, 1 });
	else
		pSliderBar->setColor({ 0.75f, 0.75f, 0.75f, 1 });
	mouseEvent.isConsumed = true;
}

void ISlider::onExit()
{
	pSliderBar->setColor({ 0.75f, 0.75f, 0.75f, 1 });
}

void ISlider::onDrag(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (isPressed)
	{
		float newX = (x - mPosX) - pSliderBar->getWidth() / 2;
		newX = std::max(newX, (-pSliderBar->getWidth() / 2));
		newX = std::min(newX, (mWidth - pSliderBar->getWidth() / 2));
		pSliderBar->setOffset({ newX, 0 });

		float prog = (newX + pSliderBar->getWidth() / 2) / (mWidth);
		*pValue = mMin + (prog * (mMax - mMin));
	}
	mouseEvent.isConsumed = true;
}

void ISlider::onDragRelease(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	isPressed = false;
	mouseEvent.isConsumed = true;
}

void ISlider::onClick(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	isPressed = false;
	mouseEvent.isConsumed = true;
}

void ISlider::onUpdate()
{
	float prog = (*pValue - mMin) / (mMax - mMin);
	pSliderBar->setOffset({ prog * mWidth - pSliderBar->getWidth() / 2, 0 });
}

bool ISlider::inComponent(int x, int y)
{
	return pSliderBar->inRect({ (float)x, (float)y }) || pHorizontalBar->inRect({ (float)x, (float)y });
}

