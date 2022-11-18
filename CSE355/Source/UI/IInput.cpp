#include "PCH.h"
#include "IInput.h"
#include "Direct2D/Drawable/Text.h"
#include "Direct2D/Drawable/Rectangle.h"
#include "Direct2D/Drawable/Line.h"
#include "Direct2D/Drawable/DynamicText.h"

using namespace D2D;

IInput::IInput(std::wstring text, TextFormat& textFormat, int width, int height)
	:
	IComponent(0, 0, width, height),
	pText(new Text(text, textFormat, width, height)),
	pTextBox(new D2D::Rectangle({ 0, 0 }, width, height, 0, (width + height) / 100, true, { 1, 0.9f, 0.65f, 1 })),
	pCaret(new Line({ 0, 0.1f * height }, { 0, 0.8f * height }, Color{ 0, 0, 0, 1 })),
	pInputText(new DynamicText(L"AA", textFormat, width, height))
{
	pText->setColor({ 0.7f, 0.7f, 0.7f, 0.7f });
	pText->setTextFormat(TextFormat(textFormat.fontFamily, textFormat.size, textFormat.bold, textFormat.style, TextAlignment::LEFT, ParagraphAlignment::CENTER));
	pInputText->setTextFormat(TextFormat(textFormat.fontFamily, textFormat.size, textFormat.bold, textFormat.style, TextAlignment::LEFT, ParagraphAlignment::CENTER));
	pTextBox->setBorderColor({ 0, 0, 0, 1 });
	auto caretMetrics = pInputText->getCaretMetrics(1);
	pCaret->setOffset({ caretMetrics.left, caretMetrics.top });
	pCaret->setPoints({ 0, 0 }, { 0, caretMetrics.height });
	mDrawables.push_back(pTextBox);
	mDrawables.push_back(pText);
	mDrawables.push_back(pCaret);
	mDrawables.push_back(pInputText);
}

IInput::~IInput()
{
	int i = 0;
}


void IInput::setText(std::wstring& text)
{
	pText->setText(text);
}

void IInput::setTextFormat(TextFormat& textFormat)
{
	pText->setTextFormat(textFormat);
}
