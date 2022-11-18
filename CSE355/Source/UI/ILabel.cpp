#include "PCH.h"
#include "ILabel.h"
#include "Direct2D/Drawable/Text.h"

using namespace D2D;

ILabel::ILabel(std::wstring text, TextFormat& textFormat, int width, int height)
	:
	IComponent(0, 0, width, height),
	pText(new Text(text, textFormat, width, height))
{
	mDrawables.push_back(pText);
}

void ILabel::setText(std::wstring& text)
{
	pText->setText(text);
}

void ILabel::setTextFormat(TextFormat& textFormat)
{
	pText->setTextFormat(textFormat);
}
