#pragma once
#include <d2d1_3.h>
#include <wrl.h>

struct Color;
class Graphics;
class Brush
{
	public:
		Brush(Graphics& gfx, Color color);
		ID2D1SolidColorBrush* get();
	private:
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
};