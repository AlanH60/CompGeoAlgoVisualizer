#pragma once
#include "Types.h"
#include <d2d1_3.h>
#include <d3d11.h>
#include <wrl.h>

#define ASSERT_IF_FAILED(hr, message) if (!FAILED(hr)){} else{ assert(false && message); }

class Window;
struct Color;
class Graphics
{
	public:
		Graphics(Window* pWindow);
		void beginFrame();
		void endFrame();

		void drawLine(FLOAT2 p1, FLOAT2 p2, ID2D1SolidColorBrush* pBrush, float strokeWidth = 1.0f);
		void drawPoint(FLOAT2 p, ID2D1SolidColorBrush* pBrush, float radius = 10.0f);

		void drawGeometry(ID2D1PathGeometry* pGeometry, ID2D1SolidColorBrush* pBrush, bool filled, FLOAT2 offset = { 0, 0 });
		void createSolidColorBrush(Color color, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush);
		void createPathGeometry(FLOAT2* vertices, unsigned int vertexCount, bool filled, Microsoft::WRL::ComPtr<ID2D1PathGeometry> pGeometry);
		
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID2D1Device> pDevice2D;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> pContext2D;
		Microsoft::WRL::ComPtr<ID2D1Factory2> pFactory2D;
};
