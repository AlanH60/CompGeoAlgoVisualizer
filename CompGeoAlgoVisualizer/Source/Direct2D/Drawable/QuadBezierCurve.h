#pragma once
#include "Drawable.h"
#include "../DrawRes/Geometry.h"

namespace D2D
{
	class QuadBezierCurve : public Drawable
	{
		public:
			QuadBezierCurve(FLOAT2 pos, FLOAT2 controlPoint, FLOAT2 endPoint);

			void draw() override;
		private:
			std::unique_ptr<Geometry> pCurve;
			FLOAT2 mControlPoint;
			FLOAT2 mEndPoint;

	};
}
