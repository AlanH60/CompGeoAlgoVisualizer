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

void IComponent::setX(int x)
{
    mPosX = x;
}

void IComponent::setY(int y)
{
    mPosY = y;
}

void IComponent::setPos(int x, int y)
{
    mPosX = x;
    mPosY = y;
}

void IComponent::setWidth(int width)
{
    mWidth = width;
}

void IComponent::setHeight(int height)
{
    mHeight = height;
}

