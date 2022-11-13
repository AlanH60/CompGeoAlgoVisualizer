#pragma once
#include "IComponent.h"
#include "Direct2D/Drawable/Text.h"

namespace D2D
{
	class Rectangle;
}

class IButton : public IComponent
{
	public:
		IButton(std::wstring text, float width, float height);
		IButton(std::wstring text, D2D::Text::TextFormat& fontFormat, float width, float height);
		void onHover(int x, int y) override;
		void onExit() override;
		void onClick(int x, int y) override;
		void setOnClick(const std::function<void()>& handler);
	private:
		D2D::Text* pText;
		D2D::Rectangle* pRectangle;
		std::function<void()> mOnClickHandler;
};

