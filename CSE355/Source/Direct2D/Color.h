#pragma once
#include <d2d1_3.h>
struct Color
{
	float r, g, b, a;

	operator D2D1_COLOR_F()
	{
		return { r, g, b, a };
	}
};