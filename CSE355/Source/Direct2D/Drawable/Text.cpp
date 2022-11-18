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
		(unsigned char)textFormat.textAlignment, mWidth, mHeight, &pTextLayout);
	ASSERT_IF_FAILED(pTextLayout->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)mTextFormat.paraAlignment), "Failed to set paragraph alignment!");
}

D2D::Text::~Text()
{
	if (pTextLayout)
		pTextLayout->Release();
}

void D2D::Text::setText(std::wstring& text)
{
	pTextLayout->Release();
	pTextLayout = nullptr;
	mText = text;
	pGraphics->createTextLayout(mText, mTextFormat.fontFamily, mTextFormat.size, mTextFormat.bold, (unsigned char)mTextFormat.style,
		(unsigned char)mTextFormat.textAlignment, mWidth, mHeight, &pTextLayout);
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


void Text::draw()
{
	pGraphics->drawText(mPos + mOffset, pTextLayout, pBrush->get());
}

