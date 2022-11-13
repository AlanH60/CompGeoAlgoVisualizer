#pragma once
#include "../Graphics.h"
#include "../Color.h"
#include "../DrawRes/Brush.h"

namespace D2D
{
	class Drawable
	{
		friend Graphics;
	public:
		Drawable(FLOAT2 pos, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		void setPos(FLOAT2 pos);
		void setOffset(FLOAT2 offset);
		void setColor(Color color);


		FLOAT2 getPos();
		FLOAT2 getOffset();
		virtual ~Drawable() = default;
		virtual void draw() = 0;
	private:
		static void setGraphics(Graphics* gfx);

	protected:
		static Graphics* pGraphics;
		std::unique_ptr<Brush> pBrush = nullptr;
		FLOAT2 mPos = { 0, 0 };
		//Used to maintain structure when drawables are in a group, where all drawables within the group have the same position.
		FLOAT2 mOffset = { 0, 0 };
	};
}
