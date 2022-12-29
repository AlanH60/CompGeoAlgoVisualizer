#pragma once
#include "IContainer.h"

namespace D2D
{
	class Rectangle;
	class Polygon;
	class Text;
	struct TextFormat;
}

class IDropDown : public IContainer
{
	private:
		const int PADDING = 5;
		class IDropDownOption : public IComponent
		{
			friend IDropDown;
			public:
				IDropDownOption(IDropDown* pDropDown, std::wstring& string);
				void onPress(int x, int y, MouseEvent& e) override;
				void onHover(int x, int y, MouseEvent& e) override;
				void onExit() override;
			private:
				IDropDown* pDropDown;
				D2D::Rectangle* pRect;
				D2D::Text* pText;
				std::wstring mString;
		};
	friend IDropDownOption;
	public:
		IDropDown(std::wstring text, int width, int height);
		IDropDown(std::wstring text, D2D::TextFormat& textFormat, int width, int height);
		~IDropDown();
		void addOption(std::wstring text);
		void addChild(IComponent* child) override;
		void onPress(int x ,int y, MouseEvent& e) override;
		void onUpdate(IComponent* parent) override;
		void onFocusLoss() override;
		void setDirtyFlag(bool dirty) override;
	private:
		void setOption(IDropDownOption* pOption);
		void close();
	private:
		std::wstring mSelectedString;
		D2D::Text* pSelectedText;
		D2D::Rectangle* pRectangle;
		D2D::Polygon* pDownArrow;
		std::vector<IDropDownOption*> mOptions; 
		bool isExpanded;
};

