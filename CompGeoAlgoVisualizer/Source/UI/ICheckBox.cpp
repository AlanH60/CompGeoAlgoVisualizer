#include "PCH.h"
#include "ICheckBox.h"
#include "Direct2D/Drawable/Text.h"
#include "Direct2D/Drawable/Rectangle.h"
#include "Direct2D/Drawable/Point.h"

using namespace D2D;

ICheckBox::ICheckBox(const std::wstring& text, int width, int height)
	:
	IComponent(0, 0, width, height),
	pText(new Text(text, TextFormat(L"Arial", 14, true, Style::NORMAL, TextAlignment::CENTER, ParagraphAlignment::CENTER), width, height)),
	pBox(new D2D::Rectangle({0.0f, 0.0f}, width, height, 0, width / 10, false, Color{ 1.0f, 1.0f, 1.0f, 1.0f }))
{
	pCheck = nullptr; ///?????????
}

bool ICheckBox::isChecked()
{
	return bIsChecked;
}
