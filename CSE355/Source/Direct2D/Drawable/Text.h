#pragma once
#include "Drawable.h"
#include "../Types.h"
#include <dwrite.h>

namespace D2D
{
	class Text : public Drawable
	{
	public:
		enum class Style : unsigned char
		{
			NORMAL = 0,
			OBLIQUE,
			ITALIC
		};
		enum class Alignment : unsigned char
		{
			LEFT = 0,
			RIGHT = 1,
			CENTER = 2
		};
		struct TextFormat
		{
			std::wstring fontFamily;
			float size;
			Style style;
			Alignment alignment;
			bool bold;
		};
	public:
		Text(std::wstring string, TextFormat& fontFormat, float width, float height);
		~Text();
		void draw() override;
	private:
		float mWidth, mHeight;
		std::wstring mString;
		TextFormat mTextFormat;
		IDWriteTextLayout* pTextLayout;
		
	};
}
