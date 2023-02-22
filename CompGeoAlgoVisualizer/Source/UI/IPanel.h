#pragma once
#include "IContainer.h"

namespace D2D
{
	class Rectangle;
}

class IPanel : public IContainer
{
	public:
		IPanel(float width, float height, Color color);
		void setWidth(float width) override;
		void setHeight(float height) override;
	private:
		D2D::Rectangle* pRect;
};
