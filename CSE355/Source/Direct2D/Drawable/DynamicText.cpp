#include "PCH.h"
#include "DynamicText.h"

using namespace D2D;

DynamicText::DynamicText(std::wstring text, TextFormat& textFormat, float width, float height)
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

DWRITE_HIT_TEST_METRICS DynamicText::getCaretMetrics(UINT32 textPosition)
{
	FLOAT2 caretPosition = {};
	DWRITE_HIT_TEST_METRICS metrics = {};
	ASSERT_IF_FAILED(pTextLayout->HitTestTextPosition(textPosition, 0, &(caretPosition.x), &(caretPosition.y), &metrics), "Failed to get caret position!");
	return metrics;
}

DWRITE_HIT_TEST_METRICS DynamicText::getCoordinateMetrics(FLOAT2 pos, BOOL* isTrailing, BOOL* isInside)
{
	DWRITE_HIT_TEST_METRICS metrics = {};
	ASSERT_IF_FAILED(pTextLayout->HitTestPoint(pos.x, pos.y, isTrailing, isInside, &metrics), "Failed to get caret position from point!");
	return metrics;
}


DynamicText::~DynamicText()
{
	if (pTextLayout)
		pTextLayout->Release();
}

void DynamicText::setText(std::wstring& string)
{
	mText = string;
}

void DynamicText::setTextFormat(TextFormat& textFormat)
{
	mTextFormat = textFormat;
	ASSERT_IF_FAILED(pTextLayout->SetFontFamilyName(mTextFormat.fontFamily.c_str(), { 0, (unsigned int)(mText.length()) }), "Failed to set font family!");
	ASSERT_IF_FAILED(pTextLayout->SetFontSize(mTextFormat.size, {0, (unsigned int)(mText.length())}), "Failed to set font size!");
	ASSERT_IF_FAILED(pTextLayout->SetFontStyle((DWRITE_FONT_STYLE)mTextFormat.style, { 0, (unsigned int)(mText.length()) }), "Failed to set font style!");
	ASSERT_IF_FAILED(pTextLayout->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)mTextFormat.textAlignment), "Failed to set text alignment");
	ASSERT_IF_FAILED(pTextLayout->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)mTextFormat.paraAlignment), "Failed to set paragraph alignment!");
	ASSERT_IF_FAILED(pTextLayout->SetFontWeight((mTextFormat.bold) ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL, { 0, (unsigned int)(mText.length()) }), "Failed to set font weight!");
}

void DynamicText::draw()
{
	if (isVisible)
		pGraphics->drawText(mText, mPos + mOffset, pTextLayout, pBrush->get());
}

