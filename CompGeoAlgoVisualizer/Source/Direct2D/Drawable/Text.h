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
			Text(const std::wstring& text, const TextFormat& textFormat, float width, float height);
			~Text();
			float getWidth();
			float getHeight();
			TextFormat getTextFormat();

			/**
			* Sets the width of the bounding text region.
			* @param width new width of the text region.
			*/
			void setWidth(float width);
			/**
			* Sets the height of the bounding text region.
			* @param height new height of the text region.
			*/
			void setHeight(float height);
			/**
			* Sets the width and height of the bounding text region.
			* @param width new width of the text region.
			* @param height new height of the text region.
			*/
			void setDimensions(float width, float height);
			/**
			* Sets the text.
			* @param text wide string containing the new text.
			*/
			void setText(std::wstring& text);
			/**
			* Sets the text format.
			* @param textFormat reference to the new textformat for the text.
			*/
			void setTextFormat(const TextFormat& textFormat);
			/**
			* Get information regarding the coordinates of the caret given its position within the text.
			* @param textPosition index within the text that the caret is at.
			* @returns A DWrite structure containing information regarding where to position the caret with respect to the inputted text position/index.
			*/
			DWRITE_HIT_TEST_METRICS getCaretMetrics(UINT32 textPosition);
			/**
			* Get information about where a given coordinate is within the text.
			* @param coords the coordinates we are interested in.
			* @param isTrailing output flag that indicates whether the coordinate is in the front or trailing side of the character.
			* @param isInside output flag that indicates whether the coordinate is inside the text bounds.  If not inside, the position closest to the coordinate is returned in the struct. 
			* @returns A DWrite structure containing information regarding where the inputted coordinate lies in the text.(Used for user clicks and drags)
			*/
			DWRITE_HIT_TEST_METRICS getCoordinateMetrics(FLOAT2 coords, BOOL* isTrailing, BOOL* isInside);
			/**
			* Get information about text that have crossed text boundary.
			* @returns A DWrite structure containing information regarding text that has exceeded the boundaries of the text.
			*/
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
			Microsoft::WRL::ComPtr<IDWriteTextLayout> pTextLayout;
		
	};
}
