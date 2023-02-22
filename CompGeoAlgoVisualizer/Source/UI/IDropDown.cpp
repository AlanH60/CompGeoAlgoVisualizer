#include "PCH.h"
#include "IDropDown.h"
#include "Direct2D/Drawable/Text.h"
#include "Direct2D/Drawable/Rectangle.h"
#include "Direct2D/Drawable/Polygon.h"

using namespace D2D;

IDropDown::IDropDownOption::IDropDownOption(IDropDown* pDropDown, std::wstring& string, int index)
	:
	IComponent(0, 0, pDropDown->mWidth, pDropDown->mHeight),
	pDropDown(pDropDown),
	mString(string),
	mIndex(index),
	pText(new Text(string, pDropDown->pSelectedText->getTextFormat(), pDropDown->mWidth - 2 * pDropDown->PADDING, pDropDown->mHeight)),
	pRect(new D2D::Rectangle({0, 0}, pDropDown->mWidth, pDropDown->mHeight, 0, 0, true, { 1.0f ,1.0f, 1.0f, 1.0f }))
{
	pText->setOffset({ pDropDown->PADDING, 0 });
	mDrawables.push_back(pRect);
	mDrawables.push_back(pText);
}

void IDropDown::IDropDownOption::setWidth(float width)
{
	IComponent::setWidth(width);
	pRect->setWidth(width);
	pText->setWidth(width);
}

void IDropDown::IDropDownOption::setHeight(float height)
{
	IComponent::setHeight(height);
	pRect->setHeight(height);
	pText->setHeight(height);
}

void IDropDown::IDropDownOption::onPress(float x, float y, MouseEvent& e)
{
	if (e.isConsumed)
		return;
	pDropDown->setOption(this);
	e.isConsumed = true;
}

void IDropDown::IDropDownOption::onHover(float x, float y, MouseEvent& e)
{
	if (e.isConsumed)
		return;
	pRect->setColor({ 0.2f, 0.6f, 1.0f, 1.0f });
	e.isConsumed = true;
}

void IDropDown::IDropDownOption::onExit()
{
	pRect->setColor({ 1.0f ,1.0f, 1.0f, 1.0f });
}

IDropDown::IDropDown(std::wstring text, float width, float height)
	:
	IDropDown(text, TextFormat(L"Arial", 14, true, Style::NORMAL, TextAlignment::LEFT, ParagraphAlignment::CENTER), width, height)
{}

IDropDown::IDropDown(std::wstring text, D2D::TextFormat& textFormat, float width, float height)
	:
	IContainer(0, 0, width, height),
	pRectangle(new D2D::Rectangle({ 0, 0 }, width, height, 0, width / 10, { 0.8f, 0.8f, 0.8f, 1.0f })),
	pSelectedText(new Text(text, textFormat, width - 2 * PADDING, height)),
	mSelectedIndex(0),
	isExpanded(false)
{
	pSelectedText->setOffset({ PADDING, 0 });
	FLOAT2 vertices[3] = { FLOAT2{ 0, 0 }, FLOAT2{ height / 3.0f, 0 }, FLOAT2{ height / 6.0f, height / 6.0f } };
	pDownArrow = new D2D::Polygon(vertices, 3, true, { 0, 0, 0, 1 });
	pDownArrow->setOffset({ width - height / 2.0f , 2.5f * height / 6.0f});
	IDropDownOption* pFirstOption = new IDropDownOption(this, text, 0);
	pFirstOption->setXDimension(IComponent::XDimension::RELATIVEX);
	pFirstOption->setRelativeWidth(1.0f);
	mOptions.push_back(pFirstOption);
	
	mDrawables.push_back(pRectangle);
	mDrawables.push_back(pSelectedText);
	mDrawables.push_back(pDownArrow);
}

IDropDown::IDropDown(float width, float height)
	:
	IDropDown(TextFormat(L"Arial", 14, true, Style::NORMAL, TextAlignment::LEFT, ParagraphAlignment::CENTER), width, height)
{
}

IDropDown::IDropDown(D2D::TextFormat& textFormat, float width, float height)
	:
	IContainer(0, 0, width, height),
	pRectangle(new D2D::Rectangle({ 0, 0 }, width, height, 0, width / 10, { 0.8f, 0.8f, 0.8f, 1.0f })),
	pSelectedText(new Text(L"", textFormat, width - 2 * PADDING, height)),
	mSelectedIndex(-1),
	isExpanded(false)
{
	pSelectedText->setOffset({ PADDING, 0 });
	FLOAT2 vertices[3] = { FLOAT2{ 0, 0 }, FLOAT2{ height / 3.0f, 0 }, FLOAT2{ height / 6.0f, height / 6.0f } };
	pDownArrow = new D2D::Polygon(vertices, 3, true, { 0, 0, 0, 1 });
	pDownArrow->setOffset({ width - height / 2.0f , 2.5f * height / 6.0f });
	mDrawables.push_back(pRectangle);
	mDrawables.push_back(pSelectedText);
	mDrawables.push_back(pDownArrow);
}

IDropDown::~IDropDown()
{
	if (!isExpanded)
		for (IDropDownOption* pOption : mOptions)
			delete pOption;
}

void IDropDown::setColor(const Color& color)
{
	IComponent::setColor(color);
	pRectangle->setColor(color);
}

int IDropDown::getSelectedIndex()
{
	return mSelectedIndex;
}

void IDropDown::setWidth(float width)
{
	IComponent::setWidth(width);
	pSelectedText->setWidth(width);
	pRectangle->setWidth(width);
	pDownArrow->setOffset({ mWidth - mHeight / 2.0f , 2.5f * mHeight / 6.0f });
}

void IDropDown::setHeight(float height)
{
	IComponent::setHeight(height);
	pSelectedText->setHeight(height);
	pRectangle->setHeight(height);
	FLOAT2 vertices[3] = { FLOAT2{ 0, 0 }, FLOAT2{ height / 3.0f, 0 }, FLOAT2{ height / 6.0f, height / 6.0f } };
	mDrawables.erase(--mDrawables.end());
	delete pDownArrow;
	pDownArrow = new D2D::Polygon(vertices, 3, true, { 0, 0, 0, 1 });
	pDownArrow->setOffset({ mWidth - mHeight / 2.0f , 2.5f * mHeight / 6.0f });
	mDrawables.push_back(pDownArrow);
}


void IDropDown::addOption(std::wstring text)
{
	close();
	IDropDownOption* pOption = new IDropDownOption(this, text, mOptions.size());
	pOption->setXDimension(XDimension::RELATIVEX);
	pOption->setRelativeWidth(1.0f);
	mOptions.push_back(pOption);
}

void IDropDown::addChild(IComponent* child)
{
	if (dynamic_cast<IDropDownOption*>(child))
		IContainer::addChild(child);
}

void IDropDown::onPress(float x, float y, MouseEvent& e)
{
	if (isExpanded)
	{
		IContainer::onPress(x, y, e);
		close();
		isExpanded = false;
	}
	else
	{
		for (int i = 0; i < mOptions.size(); i++)
		{
			mOptions[i]->setPos(0, mHeight * (i + 1));
			mOptions[i]->setHeight(mHeight);
			addChild(mOptions[i]);
		}
		mHeight = mHeight * (mOptions.size() + 1);
		isExpanded = true;
	}
}

void IDropDown::onUpdate(IComponent* parent)
{
	if (isDirty)
		close();
	IContainer::onUpdate(parent);
}

void IDropDown::onFocusLoss()
{
	close();
}

void IDropDown::setOption(IDropDownOption* pOption)
{
	if (mSelectedIndex != pOption->mIndex)
	{
		pSelectedText->setText(pOption->mString);
		mSelectedIndex = pOption->mIndex;
	}
}

void IDropDown::close()
{
	if (isExpanded)
	{
		mHeight = mHeight / (mOptions.size() + 1);
		for (IDropDownOption* pOption : mOptions)
			removeChild(pOption);
		isExpanded = false;
	}
}


