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
			void setScale(float scale);

			/**
			* Use this polygon's geometry as the mask for a clipping layer.
			* @param rect reference to a rect to use as the layer's bounds.
			* @param transform transformation that should be done to the geometric mask.
			*/ 
			void pushLayer(D2D::Rectangle& rect, D2D1_MATRIX_3X2_F& transform);
			/**
			* Pops the layer that has been pushed.  SHOULD only be called after pushLayer;
			*/
			void popLayer();
			void draw() override;
		private:
			std::unique_ptr<Geometry> pGeometry;
			float mScale;
			bool mFilled;
	};
}
