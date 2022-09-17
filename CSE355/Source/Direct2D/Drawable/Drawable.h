#pragma once
#include "../Graphics.h"
#include "../Color.h"
#include "../DrawRes/Brush.h"


class Drawable
{
	public:
		Drawable(Graphics& gfx, FLOAT2 pos, Color color = Color{1.0f, 0.0f, 0.0f, 1.0f});
		void setPos(FLOAT2 pos);
		FLOAT2 getPos();
		virtual ~Drawable() = default;
		virtual void draw(Graphics& gfx) = 0;
	protected:
		std::unique_ptr<Brush> pBrush = nullptr;
		FLOAT2 mPos;
};