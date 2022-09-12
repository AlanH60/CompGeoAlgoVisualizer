#pragma once
#include <d2d1_1.h>
#include <d3d11.h>
#include <wrl.h>


class Window;
class Graphics
{
	public:
		Graphics(Window* pWindow);
		void beginFrame();
		void endFrame();
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID2D1Device> pDevice2D;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> pContext2D;
};

#define ASSERT_IF_FAILED(hr, message) if (!FAILED(hr)){} else{ assert(false && message); }
