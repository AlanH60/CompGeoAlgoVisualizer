#include "PCH.h"
#include "IComponent.h"

using D2D::Drawable;

IComponent::IComponent(int x, int y, int width, int height)
    :
    mPosX(x),
    mPosY(y),
    mWidth(width),
    mHeight(height)
{}

IComponent::~IComponent()
{
    for (Drawable* d : mDrawables)
        delete d;
}

int IComponent::getX()
{
    return mPosX;
}

int IComponent::getY()
{
    return mPosY;
}

int IComponent::getWidth()
{
    return mWidth;
}

int IComponent::getHeight()
{
    return mHeight;
}

bool IComponent::getDirtyFlag()
{
	return isDirty;
}

void IComponent::setX(int x)
{
    mPosX = x;
}

void IComponent::setY(int y)
{
    mPosY = y;
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

void IComponent::setPos(int x, int y)
{
    mPosX = x;
    mPosY = y;
}

void IComponent::setDirtyFlag(bool isDirty)
{
	this->isDirty = isDirty;
}

void IComponent::setWidth(int width)
{
    mWidth = width;
	isDirty = true;
}

void IComponent::setHeight(int height)
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
			mPosX = (parent->mWidth - mWidth) / 2;
			break;
		case XOrientation::LEFT:
			mPosX = 0;
			break;
		case XOrientation::RIGHT:
			mPosX = (parent->mWidth - mWidth);
			break;
		default:
			break;
	}
	switch (mYOrientation)
	{
		case YOrientation::CENTER:
			mPosY = (parent->mHeight - mHeight) / 2;
			break;
		case YOrientation::TOP:
			mPosY = 0;
			break;
		case YOrientation::BOTTOM:
			mPosY = (parent->mHeight - mHeight);
			break;
		default:
			break;
	}
	isDirty = false;
}

void IComponent::draw(int originX, int originY)
{
	for (D2D::Drawable* d : mDrawables)
	{
		d->setPos(FLOAT2{ (float)originX + mPosX ,  (float)originY + mPosY });
		d->draw();
		d->setPos(FLOAT2{ mPosX + 0.0f, mPosY + 0.0f });
	}
}

bool IComponent::inComponent(int x, int y)
{
	return (x >= mPosX && x <= mPosX + mWidth && y >= mPosY && y <= mPosY + mHeight);
}

