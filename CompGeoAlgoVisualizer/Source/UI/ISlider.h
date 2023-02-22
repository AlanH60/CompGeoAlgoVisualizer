#pragma once
#include "IComponent.h"

namespace D2D
{
	class Rectangle;
}

class ISlider : public IComponent
{
	private:
		//Horizontal bar's height proportional to the total height
		const float HORIZONTAL_HEIGHT = 0.25f;
		//Slider's width proportional to the total height 
		const float SLIDER_WIDTH = 0.2f;
	public:
		ISlider(float* value, float min, float max, int width, int height);
		void setWidth(int width) override;
		void setHeight(int height) override;
		void onPress(int x, int y, MouseEvent& mouseEvent) override;
		void onMove(int x, int y, MouseEvent& mouseEvent) override;
		void onExit() override;
		void onDrag(int x, int y, MouseEvent& mouseEvent) override;
		void onDragRelease(int x, int y, MouseEvent& mouseEvent) override;
		void onClick(int x, int y, MouseEvent& mouseEvent) override;
		void onUpdate(IComponent* parent) override;
		bool inComponent(int x, int y) override;
	private:
		float* pValue;
		float mMin, mMax;
		D2D::Rectangle* pSliderBar;
		D2D::Rectangle* pHorizontalBar;
		std::function<void()> mOnClickHandler;
		bool isPressed = false;
};

