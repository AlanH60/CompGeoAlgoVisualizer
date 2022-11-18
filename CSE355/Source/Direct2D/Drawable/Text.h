#pragma once
#include "Drawable.h"
#include "../Types.h"
#include "TextFormat.h"
#include <dwrite.h>

namespace D2D
{
	class Text : public Drawable
	{
		public:
			Text(std::wstring& text, TextFormat& textFormat, float width, float height);
			~Text();

			void setText(std::wstring& text);
			void setTextFormat(TextFormat& textFormat);
			void draw() override;
		private:
			float mWidth, mHeight;
			std::wstring mText;
			TextFormat mTextFormat;
			IDWriteTextLayout* pTextLayout;
		
	};
}
