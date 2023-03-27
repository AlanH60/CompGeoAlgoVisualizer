#include "PCH.h"
#include "QuadBezierCurve.h"

D2D::QuadBezierCurve::QuadBezierCurve(FLOAT2 startPoint, FLOAT2 controlPoint, FLOAT2 endPoint)
	:
	Drawable({ 0, 0 }),
	mControlPoint(controlPoint),
	mEndPoint(endPoint)
{
	pCurve = std::unique_ptr<Geometry>(Geometry::generateQuadraticBezierCurve(*pGraphics, startPoint, controlPoint, endPoint));
}

void D2D::QuadBezierCurve::draw()
{
	pGraphics->drawGeometry(pCurve->get(), pBrush->get(), false);
}
