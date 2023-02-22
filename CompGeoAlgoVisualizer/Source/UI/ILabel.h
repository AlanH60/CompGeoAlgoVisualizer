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
		ILabel(std::wstring text, D2D::TextFormat& textFormat, float width, float height);
		void setWidth(float width) override;
		void setHeight(float height) override;
		void setText(std::wstring& text);
		void setTextFormat(D2D::TextFormat& textFormat);
	private:
		D2D::Text* pText;
};

