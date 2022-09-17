#pragma once
#include "Drawable.h"
#include "../DrawRes/Geometry.h"
#include "../Types.h"
class Polygon : public Drawable
{
	public:
		Polygon(Graphics& gfx, FLOAT2* vertices, unsigned int vertexCount, bool filled = true, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });

		~Polygon() = default;
		void draw(Graphics& gfx) override;
	private:
		std::unique_ptr<Geometry> pGeometry;
		bool mFilled;
};