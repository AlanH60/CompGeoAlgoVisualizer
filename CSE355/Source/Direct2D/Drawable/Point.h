#pragma once
#include "../Types.h"
#include "Drawable.h"

namespace D2D
{
	class Point : public Drawable
	{
		public:
			Point(FLOAT2 pos, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
			~Point() = default;
			void draw() override;
	};
}