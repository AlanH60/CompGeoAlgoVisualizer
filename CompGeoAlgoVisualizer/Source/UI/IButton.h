#pragma once
#include "IComponent.h"

namespace D2D
{
	class Rectangle;
	class Text;
	struct TextFormat;
}

class IButton : public IComponent
{
	public:
		IButton(std::wstring text, float width, float height);
		IButton(std::wstring text, D2D::TextFormat& fontFormat, float width, float height);
		void setText(std::wstring text);
		void setColor(const Color& color);
		void setTextColor(const Color& color);
		void setWidth(float width) override;
		void setHeight(float height) override;
		void onHover(float x, float y, MouseEvent& mouseEvent) override;
		void onExit() override;
		void onPress(float x, float y, MouseEvent& mouseEvent) override;
		void onClick(float x, float y, MouseEvent& mouseEvent) override;
		void setOnClick(const std::function<void()>& handler);
		void setCornerRadius(float radius);
		bool inComponent(float x, float y) override;
	private:
		D2D::Text* pText;
		D2D::Rectangle* pRectangle;
		std::function<void()> mOnClickHandler;
		Color mTextColor;
};

