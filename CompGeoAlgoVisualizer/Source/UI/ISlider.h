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
		ISlider(float* value, float min, float max, float width, float height);
		void setWidth(float width) override;
		void setHeight(float height) override;
		void onPress(float x, float y, MouseEvent& mouseEvent) override;
		void onMove(float x, float y, MouseEvent& mouseEvent) override;
		void onExit() override;
		void onDrag(float x, float y, MouseEvent& mouseEvent) override;
		void onDragRelease(float x, float y, MouseEvent& mouseEvent) override;
		void onClick(float x, float y, MouseEvent& mouseEvent) override;
		void onUpdate(IComponent* parent) override;
		bool inComponent(float x, float y) override;
	private:
		float* pValue;
		float mMin, mMax;
		D2D::Rectangle* pSliderBar;
		D2D::Rectangle* pHorizontalBar;
		std::function<void()> mOnClickHandler;
		bool isPressed = false;
};

