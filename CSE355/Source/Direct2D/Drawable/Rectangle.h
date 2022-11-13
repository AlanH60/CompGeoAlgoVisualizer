#pragma once
#include "Drawable.h"
#include "../Types.h"

namespace D2D
{
	class Rectangle : public Drawable
	{
	public:
		Rectangle(FLOAT2 pos, float width, float height, bool filled = true, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		Rectangle(FLOAT2 pos, float width, float height, float cornerRadius, bool filled = true, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		~Rectangle() = default;
		void draw() override;

		float getWidth();
		float getHeight();
		float getCornerRadius();

		void setWidth(float width);
		void setHeight(float height);
		void setCornerRadius(float cornerRadius);

	private:
		float mWidth, mHeight;
		float mCornerRadius;
		bool mFilled;
	};
}
