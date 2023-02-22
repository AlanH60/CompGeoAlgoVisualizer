#pragma once
#include "IContainer.h"

namespace D2D
{
	class Rectangle;
	class Text;
	struct TextFormat;
}

class IPanel;

class ITabbedPanel : public IContainer
{
	private:
		class ITab : public IComponent
		{
			friend class ITabbedPanel;
			public:
				ITab(std::wstring& tabName, D2D::TextFormat& textFormat, ITabbedPanel* pTabbedPanel, IPanel* pPanel, int width, int height);

				void setWidth(int width) override;
				void setHeight(int height) override;
				void onPress(int x, int y, MouseEvent& e) override;
			private:
				ITabbedPanel* pTabbedPanel;
				IPanel* pPanel;
				D2D::Rectangle* pRect;
				D2D::Text* pText;
		};
	public:
		ITabbedPanel(int width, int height);
		~ITabbedPanel();
		void addPanel(std::wstring panelName, IPanel* pPanel);
		void onUpdate(IComponent* parent) override;
	private:
		void setCurrTab(ITab* tab);
	private:
		std::vector<ITab*> mTabs;
		ITab* pCurrTab = nullptr;
		
};
