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
		IButton(std::wstring text, int width, int height);
		IButton(std::wstring text, D2D::TextFormat& fontFormat, int width, int height);
		void onHover(int x, int y, MouseEvent& mouseEvent) override;
		void onExit() override;
		void onPress(int x, int y, MouseEvent& mouseEvent) override;
		void onClick(int x, int y, MouseEvent& mouseEvent) override;
		void setOnClick(const std::function<void()>& handler);
		void setCornerRadius(int radius);
		bool inComponent(int x, int y) override;
	private:
		D2D::Text* pText;
		D2D::Rectangle* pRectangle;
		std::function<void()> mOnClickHandler;
};

