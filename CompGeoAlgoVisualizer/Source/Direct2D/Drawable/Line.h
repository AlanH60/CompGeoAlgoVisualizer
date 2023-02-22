#pragma once
#include "Drawable.h"
#include "../Types.h"

namespace D2D
{
	class Line : public Drawable
	{
	public:
		Line(FLOAT2 p1, FLOAT2 p2, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		~Line() = default;
		void draw() override;

		FLOAT2 getP1();
		FLOAT2 getP2();
		void setPoints(FLOAT2 p1, FLOAT2 p2);
	private:
		FLOAT2 mP1, mP2;
	};
}
