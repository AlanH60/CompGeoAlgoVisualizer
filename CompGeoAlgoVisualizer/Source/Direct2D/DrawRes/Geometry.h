#pragma once
#include "../Types.h"
#include <d2d1_3.h>
#include <wrl.h>

class Graphics;
class Geometry
{
	public:
		//Geometry(Graphics& gfx, FLOAT2* vertices, unsigned int vertexCount, bool filled);
		ID2D1PathGeometry* get();

		static Geometry* generatePolygon(Graphics& gfx, FLOAT2* vertices, unsigned int vertexCount, bool filled);
		static Geometry* generateQuadraticBezierCurve(Graphics& gfx, FLOAT2 startPoint, FLOAT2 controlPoint, FLOAT2 endPoint);
	private:
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> pGeometry;
};