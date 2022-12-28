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
			float getWidth();
			float getHeight();
			TextFormat getTextFormat();


			void setWidth(float width);
			void setHeight(float height);
			void setDimensions(float width, float height);
			void setText(std::wstring& text);
			void setTextFormat(TextFormat& textFormat);
			//Returns the DWrite structure containing information regarding where to position the caret with respect to the inputted text position/index.
			DWRITE_HIT_TEST_METRICS getCaretMetrics(UINT32 textPosition);
			//Returns the DWrite structure containing information regarding where the inputted coordinate lies in the text.(Used for user clicks and drags)
			DWRITE_HIT_TEST_METRICS getCoordinateMetrics(FLOAT2 coords, BOOL* isTrailing, BOOL* isInside);
			//Returns the DWrite structure containing information regarding text that has exceeded the boundaries of the text.
			DWRITE_OVERHANG_METRICS getOverhangMetrics();
			void draw() override;
		private:
			//Max width and height of the area bounding the text.
			float mWidth, mHeight;
			//String of the text
			std::wstring mText;
			//Text format
			TextFormat mTextFormat;
			//Layout generated from graphics API- used for draw call.
			IDWriteTextLayout* pTextLayout;
		
	};
}
