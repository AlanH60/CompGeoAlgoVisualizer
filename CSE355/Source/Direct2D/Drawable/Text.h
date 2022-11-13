#pragma once
#include "Drawable.h"
#include "../Types.h"
#include <dwrite.h>

namespace D2D
{
	class Text : public Drawable
	{
	public:
		enum Style
		{
			NORMAL = 0,
			OBLIQUE,
			ITALIC
		};
		struct FontFormat
		{
			std::wstring fontFamily;
			float size;
			Style style;
			bool bold;
		};
	public:
		Text(std::wstring string, FontFormat& fontFormat, float width, float height);
		~Text();
		void draw() override;
	private:
		float mWidth, mHeight;
		std::wstring mString;
		FontFormat mFontFormat;
		IDWriteTextLayout* pTextLayout;
		
	};
}
