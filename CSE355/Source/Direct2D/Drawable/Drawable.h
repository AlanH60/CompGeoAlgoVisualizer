#pragma once
#include "../Graphics.h"
#include "../Color.h"
#include "../DrawRes/Brush.h"


class Drawable
{
	friend Graphics;
	public:
		Drawable(FLOAT2 pos, Color color = Color{1.0f, 0.0f, 0.0f, 1.0f});
		void setPos(FLOAT2 pos);
		void setColor(Color color);
		

		FLOAT2 getPos();
		virtual ~Drawable() = default;
		virtual void draw() = 0;
	private:
		static void setGraphics(Graphics* gfx);

	protected:
		static Graphics* pGraphics;
		std::unique_ptr<Brush> pBrush = nullptr;
		FLOAT2 mPos;
};