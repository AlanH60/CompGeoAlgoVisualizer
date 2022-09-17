#pragma once
#include "Drawable.h"
#include "../Types.h"

class Line : public Drawable
{
	public:
		Line(Graphics& gfx, FLOAT2 p1, FLOAT2 p2, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		~Line() = default;
		void draw(Graphics& gfx) override;
	private:
		FLOAT2 mP1, mP2;
};