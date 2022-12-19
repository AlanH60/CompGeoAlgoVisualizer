#include "PCH.h"
#include "ITabbedPanel.h"
#include "IPanel.h"
#include "Direct2D/Drawable/Rectangle.h"
#include "Direct2D/Drawable/Text.h"

using namespace D2D;

ITabbedPanel::ITab::ITab(std::wstring& tabName, TextFormat& textFormat, ITabbedPanel* pTabbedPanel, IPanel* pPanel, int width, int height)
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

void ITabbedPanel::ITab::setWidth(int width)
{
	mWidth = width;
	pRect->setWidth(width);
	pText->setWidth(width);
}

void ITabbedPanel::ITab::setHeight(int height)
{
	mHeight = height;
	pRect->setHeight(height);
	pText->setHeight(height);
}

void ITabbedPanel::ITab::onPress(int x, int y, MouseEvent& e)
{
	pTabbedPanel->setCurrTab(this);
	e.isConsumed = true;
}



ITabbedPanel::ITabbedPanel(int width, int height)
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

void ITabbedPanel::addPanel(std::wstring panelName, IPanel* pPanel)
{
	ITab* pTab = new ITab(panelName, TextFormat(L"Arial", 16, true, Style::NORMAL, TextAlignment::CENTER, ParagraphAlignment::CENTER), this, pPanel, mWidth, mHeight / 25);
	if (pPanel->getWidth() > mWidth || pPanel->getWidth() == 0)
		pPanel->setWidth(mWidth);
	if (pPanel->getHeight() > mHeight - mHeight / 25 || pPanel->getHeight() == 0)
		pPanel->setHeight(mHeight - mHeight / 25);
	pPanel->setPos(0, mHeight / 25);
	if (!pCurrTab)
	{
		pCurrTab = pTab;
		addChild(pPanel);
	}
	mTabs.push_back(pTab);
	addChild(pTab);
	for (int i = 0; i < mTabs.size(); i ++)
	{
		mTabs[i]->setWidth(mWidth / mTabs.size());
		mTabs[i]->setPos(mWidth / mTabs.size() * i, 0);
	}
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
