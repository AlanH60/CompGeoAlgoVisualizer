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
		IButton(std::wstring text, D2D::Text::FontFormat& fontFormat, float width, float height);
	private:
		D2D::Text* pText;
		D2D::Rectangle* pRectangle;
		std::function<void(MouseEvent&)> onClickHandler;
};

