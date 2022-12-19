#include "PCH.h"
#include "Text.h"

using namespace D2D;

Text::Text(std::wstring& text, TextFormat& textFormat, float width, float height)
	:
	Drawable({ 0, 0 }, { 0, 0, 0, 1 }),
	mText(text),
	mTextFormat(textFormat),
	mWidth(width),
	mHeight(height)
{
	pGraphics->createTextLayout(mText, textFormat.fontFamily, textFormat.size, textFormat.bold, (unsigned char)textFormat.style,
		(unsigned char)textFormat.textAlignment, (unsigned char)textFormat.paraAlignment, mWidth, mHeight, &pTextLayout);
}

D2D::Text::~Text()
{
	if (pTextLayout)
		pTextLayout->Release();
}

float D2D::Text::getWidth()
{
	return mWidth;
}

float D2D::Text::getHeight()
{
	return mHeight;
}

void D2D::Text::setWidth(float width)
{
	mWidth = width;
	if (pTextLayout)
		pTextLayout->Release();
	pGraphics->createTextLayout(mText, mTextFormat.fontFamily, mTextFormat.size, mTextFormat.bold, (unsigned char)mTextFormat.style,
		(unsigned char)mTextFormat.textAlignment, (unsigned char)mTextFormat.paraAlignment, mWidth, mHeight, &pTextLayout);
}

void D2D::Text::setHeight(float height)
{
	mHeight = height;
	if (pTextLayout)
		pTextLayout->Release();
	pGraphics->createTextLayout(mText, mTextFormat.fontFamily, mTextFormat.size, mTextFormat.bold, (unsigned char)mTextFormat.style,
		(unsigned char)mTextFormat.textAlignment, (unsigned char)mTextFormat.paraAlignment, mWidth, mHeight, &pTextLayout);

}

void D2D::Text::setDimensions(float width, float height)
{
	mWidth = width;
	mHeight = height;
	if (pTextLayout)
		pTextLayout->Release();
	pGraphics->createTextLayout(mText, mTextFormat.fontFamily, mTextFormat.size, mTextFormat.bold, (unsigned char)mTextFormat.style,
		(unsigned char)mTextFormat.textAlignment, (unsigned char)mTextFormat.paraAlignment, mWidth, mHeight, &pTextLayout);

}

void D2D::Text::setText(std::wstring& text)
{
	pTextLayout->Release();
	pTextLayout = nullptr;
	mText = text;
	pGraphics->createTextLayout(mText, mTextFormat.fontFamily, mTextFormat.size, mTextFormat.bold, (unsigned char)mTextFormat.style,
		(unsigned char)mTextFormat.textAlignment, (unsigned char)mTextFormat.paraAlignment, mWidth, mHeight, &pTextLayout);
}

void Text::setTextFormat(TextFormat& textFormat)
{
	mTextFormat = textFormat;
	ASSERT_IF_FAILED(pTextLayout->SetFontFamilyName(mTextFormat.fontFamily.c_str(), { 0, (unsigned int)(mText.length()) }), "Failed to set font family!");
	ASSERT_IF_FAILED(pTextLayout->SetFontSize(mTextFormat.size, { 0, (unsigned int)(mText.length()) }), "Failed to set font size!");
	ASSERT_IF_FAILED(pTextLayout->SetFontStyle((DWRITE_FONT_STYLE)mTextFormat.style, { 0, (unsigned int)(mText.length()) }), "Failed to set font style!");
	ASSERT_IF_FAILED(pTextLayout->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)mTextFormat.textAlignment), "Failed to set text alignment");
	ASSERT_IF_FAILED(pTextLayout->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)mTextFormat.paraAlignment), "Failed to set paragraph alignment!");
	ASSERT_IF_FAILED(pTextLayout->SetFontWeight((mTextFormat.bold) ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL, { 0, (unsigned int)(mText.length()) }), "Failed to set font weight!");
}

DWRITE_HIT_TEST_METRICS Text::getCaretMetrics(UINT32 textPosition)
{
	FLOAT2 caretPosition = {};
	DWRITE_HIT_TEST_METRICS metrics = {};
	ASSERT_IF_FAILED(pTextLayout->HitTestTextPosition(textPosition, 0, &(caretPosition.x), &(caretPosition.y), &metrics), "Failed to get caret position!");
	return metrics;
}

DWRITE_HIT_TEST_METRICS Text::getCoordinateMetrics(FLOAT2 coords, BOOL* isTrailing, BOOL* isInside)
{
	DWRITE_HIT_TEST_METRICS metrics = {};
	ASSERT_IF_FAILED(pTextLayout->HitTestPoint(coords.x, coords.y, isTrailing, isInside, &metrics), "Failed to get caret position from point!");
	return metrics;
}

DWRITE_OVERHANG_METRICS D2D::Text::getOverhangMetrics()
{
	DWRITE_OVERHANG_METRICS m = {};
	pTextLayout->GetOverhangMetrics(&m);
	return m;
}

void Text::draw()
{
	if (isVisible)
		pGraphics->drawText(mPos + mOffset, pTextLayout, pBrush->get());
}

