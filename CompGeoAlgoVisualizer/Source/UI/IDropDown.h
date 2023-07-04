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
		const float PADDING = 5.0f;
		class IDropDownOption : public IComponent
		{
			friend IDropDown;
			public:
				IDropDownOption(IDropDown* pDropDown, const std::wstring& string, int index);
				void setWidth(float width) override;
				void setHeight(float height) override;
				void onPress(float x, float y, MouseEvent& e) override;
				void onHover(float x, float y, MouseEvent& e) override;
				void onExit() override;
			private:
				IDropDown* pDropDown;
				D2D::Rectangle* pRect;
				D2D::Text* pText;
				std::wstring mString;
				int mIndex;
		};
	friend IDropDownOption;
	public:
		IDropDown(const std::wstring& text, float width, float height);
		IDropDown(const std::wstring& text, const D2D::TextFormat& textFormat, float width, float height);
		IDropDown(float width, float height);
		IDropDown(const D2D::TextFormat& textFormat, float width, float height);
		~IDropDown();
		void setColor(const Color& color) override;
		int getSelectedIndex();
		void setWidth(float width) override;
		void setHeight(float height) override;
		void addOption(std::wstring text);
		void addChild(IComponent* child) override;
		void clearOptions(std::wstring newDefault);
		void onPress(float x ,float y, MouseEvent& e) override;
		void onUpdate(IComponent* parent) override;
		void onFocusLoss() override;
	private:
		void setOption(IDropDownOption* pOption);
		void close();
	private:
		D2D::Text* pSelectedText;
		D2D::Rectangle* pRectangle;
		D2D::Polygon* pDownArrow;
		std::vector<IDropDownOption*> mOptions; 
		int mSelectedIndex;
		bool isExpanded;
};

