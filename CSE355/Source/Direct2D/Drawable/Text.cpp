#include "PCH.h"
#include "Text.h"

using namespace D2D;

Text::Text(std::wstring string, FontFormat& fontFormat, float width, float height)
	:
	Drawable({ 0, 0 }, { 0, 0, 0, 1 }),
	mString(string),
	mFontFormat(fontFormat),
	mWidth(width),
	mHeight(height)
{
	pGraphics->createTextLayout(mString, fontFormat.fontFamily, fontFormat.size, fontFormat.bold, (unsigned char)fontFormat.style, 
		mWidth, mHeight, &pTextLayout);
}

D2D::Text::~Text()
{
	pTextLayout->Release();
}

void Text::draw()
{
	pGraphics->drawText(mPos + mOffset, pTextLayout, pBrush->get());
}

