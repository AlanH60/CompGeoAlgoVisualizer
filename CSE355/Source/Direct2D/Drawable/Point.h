#pragma once
#include "../Types.h"
#include "Drawable.h"

class Point : public Drawable
{
	public:
		Point(Graphics& gfx, FLOAT2 pos, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		~Point() = default;
		void draw(Graphics& gfx) override;
};