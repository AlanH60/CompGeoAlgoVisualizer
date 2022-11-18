#pragma once
#include "Drawable.h"
#include "../Types.h"
#include "TextFormat.h"
#include <dwrite.h>

namespace D2D
{
	class DynamicText : public Drawable
	{
		public:
			DynamicText(std::wstring string, TextFormat& textFormat, float width, float height);
			~DynamicText();
			DWRITE_HIT_TEST_METRICS getCaretMetrics(UINT32 textPosition);

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
