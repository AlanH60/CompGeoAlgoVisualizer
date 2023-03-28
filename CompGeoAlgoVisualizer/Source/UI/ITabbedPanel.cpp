#include "PCH.h"
#include "ITabbedPanel.h"
#include "IPanel.h"
#include "Direct2D/Drawable/Rectangle.h"
#include "Direct2D/Drawable/Text.h"

using namespace D2D;

ITabbedPanel::ITab::ITab(const std::wstring& tabName, const TextFormat& textFormat, ITabbedPanel* pTabbedPanel, IPanel* pPanel, float width, float height)
	:
	IComponent(0, 0, width, height),
	pText(new Text(tabName, textFormat, width, height)),
	pTabbedPanel(pTabbedPanel),
	pPanel(pPanel),
	pRect(new D2D::Rectangle({ 0, 0 }, width, height, true, { 1, 1, 1, 1 }))
{
	pRect->setBorderColor({ 0, 0, 0, 1 });

	mDrawables.push_back(pRect);
	mDrawables.push_back(pText);
}

void ITabbedPanel::ITab::setWidth(float width)
{
	mWidth = width;
	pRect->setWidth(width);
	pText->setWidth(width);
}

void ITabbedPanel::ITab::setHeight(float height)
{
	mHeight = height;
	pRect->setHeight(height);
	pText->setHeight(height);
}

void ITabbedPanel::ITab::onPress(float x, float y, MouseEvent& e)
{
	pTabbedPanel->setCurrTab(this);
	e.isConsumed = true;
}



ITabbedPanel::ITabbedPanel(float width, float height)
	:
	IContainer(0, 0, width, height)
{
}

ITabbedPanel::~ITabbedPanel()
{
	for (ITab* pTab : mTabs)
	{
		if (pCurrTab && pTab == pCurrTab)
			continue;
		delete pTab->pPanel;
	}
}

void ITabbedPanel::addPanel(const std::wstring& panelName, IPanel* pPanel)
{
	ITab* pTab = new ITab(panelName, TextFormat(L"Arial", 16, true, Style::NORMAL, TextAlignment::CENTER, ParagraphAlignment::CENTER), this, pPanel, mWidth, mHeight / 25);
	pTab->setXDimension(XDimension::RELATIVEX);
	pTab->setYDimension(YDimension::RELATIVEY);
	pTab->setRelativeHeight(0.04f);
	pTab->setXOrientation(XOrientation::LEFT);
	pTab->setYOrientation(YOrientation::TOP);
	pPanel->setXDimension(XDimension::RELATIVEX);
	pPanel->setYDimension(YDimension::RELATIVEY);
	pPanel->setRelativeWidth(1.0f);
	pPanel->setRelativeHeight(0.96f);
	pPanel->setXOrientation(XOrientation::LEFT);
	pPanel->setYOrientation(YOrientation::BOTTOM);

	if (!pCurrTab)
	{
		pCurrTab = pTab;
		addChild(pPanel);
	}
	mTabs.push_back(pTab);
	addChild(pTab);
	for (int i = 0; i < mTabs.size(); i ++)
	{
		mTabs[i]->setPos(mWidth / mTabs.size() * i, 0);
		mTabs[i]->setRelativeWidth(1.0f / mTabs.size());
	}
}

void ITabbedPanel::onUpdate(IComponent* parent)
{
	bool isDirty = this->isDirty;
	IContainer::onUpdate(parent);
	if (isDirty)
		for (int i = 0; i < mTabs.size(); i++)
			mTabs[i]->setPos(mWidth / mTabs.size() * i, 0);
}

void ITabbedPanel::setCurrTab(ITab* tab)
{
	if (pCurrTab != tab)
	{
		removeChild(pCurrTab->pPanel);
		addChild(tab->pPanel);
		pCurrTab = tab;
	}
}
