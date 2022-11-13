#pragma once
#include "Drawable.h"
#include "../DrawRes/Geometry.h"
#include "../Types.h"

namespace D2D
{
	class Polygon : public Drawable
	{
	public:
		Polygon(FLOAT2* vertices, unsigned int vertexCount, bool filled = true, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });

		~Polygon() = default;
		void draw() override;
	private:
		std::unique_ptr<Geometry> pGeometry;
		bool mFilled;
	};
}
