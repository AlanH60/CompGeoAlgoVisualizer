#pragma once
#include "IComponent.h"

namespace D2D
{
	class Text;
	struct TextFormat;
}

class ILabel : public IComponent
{
	public:
		ILabel(std::wstring text, D2D::TextFormat& textFormat, int width, int height);
		void setText(std::wstring& text);
		void setTextFormat(D2D::TextFormat& textFormat);
	private:
		D2D::Text* pText;
};

