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
				ITab(const std::wstring& tabName, const D2D::TextFormat& textFormat, ITabbedPanel* pTabbedPanel, IPanel* pPanel, float width, float height);

				void setWidth(float width) override;
				void setHeight(float height) override;
				void onPress(float x, float y, MouseEvent& e) override;
			private:
				ITabbedPanel* pTabbedPanel;
				IPanel* pPanel;
				D2D::Rectangle* pRect;
				D2D::Text* pText;
		};
	public:
		ITabbedPanel(float width, float height);
		~ITabbedPanel();
		void addPanel(const std::wstring& panelName, IPanel* pPanel);
		void onUpdate(IComponent* parent) override;
	private:
		void setCurrTab(ITab* tab);
	private:
		std::vector<ITab*> mTabs;
		ITab* pCurrTab = nullptr;
		
};
