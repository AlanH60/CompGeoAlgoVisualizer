#pragma once
#include "IComponent.h"

namespace D2D
{
	class Text;
	class Rectangle;
	class Polygon;
}

class ICheckBox : public IComponent
{
	public:
		ICheckBox(const std::wstring& text, int width, int height);
		bool isChecked();
	private:
		D2D::Text* pText;
		D2D::Rectangle* pBox;
		D2D::Polygon* pCheck;
		bool bIsChecked;
};