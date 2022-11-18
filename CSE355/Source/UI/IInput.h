#pragma once
#include "IComponent.h"

namespace D2D
{
	class Rectangle;
	class Line;
	class Text;
	class DynamicText;
	struct TextFormat;
}

class IInput : public IComponent
{
	public:
		IInput(std::wstring text, D2D::TextFormat& textFormat, int width, int height);
		~IInput();
		void setText(std::wstring& text);
		void setTextFormat(D2D::TextFormat& textFormat);
	private:
		D2D::Text* pText;
		D2D::Rectangle* pTextBox;
		D2D::DynamicText* pInputText;
		D2D::Line* pCaret;
};

