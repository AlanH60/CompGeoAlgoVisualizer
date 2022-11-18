#pragma once
#include "IComponent.h"

namespace D2D
{
	class Rectangle;
}

class ISlider : public IComponent
{
	public:
		ISlider(float* value, float min, float max, float width, float height);
		void onPress(int x, int y, MouseEvent& mouseEvent) override;
		void onMove(int x, int y, MouseEvent& mouseEvent) override;
		void onExit(MouseEvent& mouseEvent) override;
		void onDrag(int x, int y, MouseEvent& mouseEvent) override;
		void onDragRelease(int x, int y, MouseEvent& mouseEvent) override;
		void onClick(int x, int y, MouseEvent& mouseEvent) override;
	private:
		float* pValue;
		float mMin, mMax;
		D2D::Rectangle* pSliderBar;
		D2D::Rectangle* pHorizontalBar;
		std::function<void()> mOnClickHandler;
		bool isPressed = false;
};

