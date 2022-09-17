#include "PCH.h"
#include "Brush.h"
#include "../Color.h"
#include "../Graphics.h"

Brush::Brush(Graphics& gfx, Color color)
{
	gfx.createSolidColorBrush(color, pBrush);
}

ID2D1SolidColorBrush* Brush::get()
{
	return pBrush.Get();
}

