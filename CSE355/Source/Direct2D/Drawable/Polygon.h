#pragma once
#include "Drawable.h"
#include "../DrawRes/Geometry.h"
#include "../Types.h"

namespace D2D
{
	class Rectangle;
	class Polygon : public Drawable
	{
	public:
		Polygon(FLOAT2* vertices, unsigned int vertexCount, bool filled = true, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		~Polygon() = default;

		//Use this polygon's geometry as the mask for a clipping layer.
		void pushLayer(D2D::Rectangle& rect, D2D1_MATRIX_3X2_F& transform);
		//Pop layer must be used after push layer.
		void popLayer();
		void draw() override;
	private:
		std::unique_ptr<Geometry> pGeometry;
		bool mFilled;
	};
}
