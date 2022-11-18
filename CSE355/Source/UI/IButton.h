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
		void onHover(int x, int y, MouseEvent& mouseEvent) override;
		void onExit(MouseEvent& mouseEvent) override;
		void onPress(int x, int y, MouseEvent& mouseEvent) override;
		void onClick(int x, int y, MouseEvent& mouseEvent) override;
		void setOnClick(const std::function<void()>& handler);
	private:
		D2D::Text* pText;
		D2D::Rectangle* pRectangle;
		std::function<void()> mOnClickHandler;
};

