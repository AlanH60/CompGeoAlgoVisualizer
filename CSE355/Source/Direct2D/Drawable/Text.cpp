#include "PCH.h"
#include "Text.h"

using namespace D2D;

Text::Text(std::wstring string, TextFormat& textFormat, float width, float height)
	:
	Drawable({ 0, 0 }, { 0, 0, 0, 1 }),
	mString(string),
	mTextFormat(textFormat),
	mWidth(width),
	mHeight(height)
{
	pGraphics->createTextLayout(mString, textFormat.fontFamily, textFormat.size, textFormat.bold, (unsigned char)textFormat.style,
		(unsigned char)textFormat.alignment, mWidth, mHeight, &pTextLayout);
}

D2D::Text::~Text()
{
	pTextLayout->Release();
}

void Text::draw()
{
	pGraphics->drawText(mPos + mOffset, pTextLayout, pBrush->get());
}

