#include "PCH.h"
#include "IComponent.h"

using D2D::Drawable;

IComponent::IComponent(float x, float y, float width, float height)
    :
    mPos({x, y}),
    mWidth(width),
    mHeight(height)
{}

IComponent::~IComponent()
{
    for (Drawable* d : mDrawables)
        delete d;
}

FLOAT2 IComponent::getPos()
{
    return mPos;
}

float IComponent::getWidth()
{
    return mWidth;
}

float IComponent::getHeight()
{
    return mHeight;
}

bool IComponent::getDirtyFlag()
{
	return isDirty;
}

bool IComponent::getVisibleFlag()
{
	return isVisible;
}

void IComponent::setX(float x)
{
    mPos.x = x;
}

void IComponent::setY(float y)
{
	mPos.y = y;
}

void IComponent::setPos(FLOAT2 pos)
{
	mPos = pos;
}

void IComponent::setPos(float x, float y)
{
	mPos = { x, y };
}

void IComponent::setXOrientation(XOrientation xOrientation)
{
    mXOrientation = xOrientation;
	isDirty = true;
}

void IComponent::setYOrientation(YOrientation yOrientation)
{
    mYOrientation = yOrientation;
	isDirty = true;
}

void IComponent::setRelativeWidthOrientation(float rWidthOrientation)
{
	mRWidthOrientation = rWidthOrientation;
	isDirty = true;
}

void IComponent::setRelativeHeightOrientation(float rHeightOrientation)
{
	mRHeightOrientation = rHeightOrientation;
	isDirty = true;
}

void IComponent::setXDimension(XDimension xDimension)
{
	mXDimension = xDimension;
	isDirty = true;
}

void IComponent::setYDimension(YDimension yDimension)
{
	mYDimension = yDimension;
	isDirty = true;
}

void IComponent::setRelativeWidth(float rWidth)
{
	mRelativeWidth = rWidth;
	isDirty = true;
}

void IComponent::setRelativeHeight(float rHeight)
{
	mRelativeHeight = rHeight;
	isDirty = true;
}

void IComponent::setVisibleFlag(bool isVisible)
{
	this->isVisible = isVisible;
}

void IComponent::setColor(const Color& color)
{
	mColor = color;
}

void IComponent::setDirtyFlag(bool isDirty)
{
	this->isDirty = isDirty;
}

void IComponent::setWidth(float width)
{
    mWidth = width;
	isDirty = true;
}

void IComponent::setHeight(float height)
{
    mHeight = height;
	isDirty = true;
}

void IComponent::onEvent(Event& e)
{
	if (e.isConsumed)
		return;
}

void IComponent::onUpdate(IComponent* parent)
{
	if (!parent || !isDirty)
		return;
	switch (mXDimension)
	{
		case XDimension::RELATIVEX:
			setWidth(mRelativeWidth * parent->mWidth);
			break;
		default:
			break;
	}
	switch (mYDimension)
	{
		case YDimension::RELATIVEY:
			setHeight(mRelativeHeight * parent->mHeight);
			break;
		default:
			break;
	}
	switch (mXOrientation)
	{
		case XOrientation::CENTER:
			mPos.x = (parent->mWidth - mWidth) / 2;
			break;
		case XOrientation::LEFT:
			mPos.x = 0;
			break;
		case XOrientation::RIGHT:
			mPos.x = (parent->mWidth - mWidth);
			break;
		case XOrientation::RELATIVE_WIDTH:
			mPos.x = mRWidthOrientation * parent->mWidth;
			break;
		default:
			break;
	}
	switch (mYOrientation)
	{
		case YOrientation::CENTER:
			mPos.y = (parent->mHeight - mHeight) / 2;
			break;
		case YOrientation::TOP:
			mPos.y = 0;
			break;
		case YOrientation::BOTTOM:
			mPos.y = (parent->mHeight - mHeight);
			break;
		case YOrientation::RELATIVE_HEIGHT:
			mPos.y = mRHeightOrientation * parent->mHeight;
			break;
		default:
			break;
	}
	isDirty = false;
}

void IComponent::draw(float originX, float originY)
{
	for (D2D::Drawable* d : mDrawables)
	{
		d->setPos(FLOAT2{ (float)originX + mPos.x ,  (float)originY + mPos.y });
		d->draw();
		d->setPos(FLOAT2{ mPos.x + 0.0f, mPos.y + 0.0f });
	}
}

bool IComponent::inComponent(float x, float y)
{
	return (x >= mPos.x && x <= mPos.x + mWidth && y >= mPos.y && y <= mPos.y + mHeight);
}

