#include "PCH.h"
#include "Graphics.h"
#include "Window.h"

using Microsoft::WRL::ComPtr;
Graphics::Graphics(Window* pWindow)
{
	//*********************** Create 3D Device, Context, and SwapChain ***********************//
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifndef NDEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#else
#endif

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//Anti-aliasing
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2u;
	sd.OutputWindow = pWindow->getHandle();
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext);

	ASSERT_IF_FAILED(hr, "Failed to create Device and Swapchain!");

	//***************** Create 2D Device and Context ************************//
	//Query for IDXGIDevice with 3D device.
	ComPtr<IDXGIDevice> pIDXGIDevice;
	hr = pDevice->QueryInterface(__uuidof(IDXGIDevice), &pIDXGIDevice);
	ASSERT_IF_FAILED(hr, "Failed to query IDXGIDevice for creating 2D Device!");

	//Create 2D Device using IDXGIDevice
	D2D1_CREATION_PROPERTIES cProps;
	cProps.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
	cProps.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#ifndef NDEBUG
	cProps.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	cProps.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	hr = D2D1CreateDevice(pIDXGIDevice.Get(), &cProps, &pDevice2D);
	ASSERT_IF_FAILED(hr, "Failed to create 2D Device!");

	//Create a IDXGISurface for the 2D context to draw on
	ComPtr<IDXGISurface> pSurface;
	hr = pSwapChain->GetBuffer(0, _uuidof(IDXGISurface), &pSurface);
	ASSERT_IF_FAILED(hr, "Failed to obtain IDXGISurface from Swap Chain!")

	//Create 2D Context that will draw to swapchain's buffer
	hr = D2D1CreateDeviceContext(pSurface.Get(), cProps, &pContext2D);
	ASSERT_IF_FAILED(hr, "Failed to create 2D Device Context!");
}

void Graphics::beginFrame()
{
	pContext2D->BeginDraw();
	pContext2D->Clear(D2D1_COLOR_F{ 0.0f, 0.0f, 0.0f, 1.0f });
	
}

void Graphics::endFrame()
{
	pContext2D->EndDraw();
	pSwapChain->Present(1, 0);
}
