#pragma once
#include "IContainer.h"

namespace D2D
{
	class Rectangle;
}

class IPanel : public IContainer
{
	public:
		IPanel(int width, int height, Color color);
		void setWidth(int width) override;
		void setHeight(int height) override;
	private:
		D2D::Rectangle* pRect;
};
