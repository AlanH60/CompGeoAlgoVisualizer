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

	//Create 2D Factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory2), &pFactory2D);
	ASSERT_IF_FAILED(hr, "Failed to create 2D Factory!");

	//Create 2D Device using IDXGIDevice
	D2D1_CREATION_PROPERTIES cProps;
	cProps.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
	cProps.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#ifndef NDEBUG
	cProps.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	cProps.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

	hr = pFactory2D->CreateDevice(pIDXGIDevice.Get(), &pDevice2D);
	ASSERT_IF_FAILED(hr, "Failed to create 2D Device!");

	//Create a IDXGISurface for the 2D context to draw on
	ComPtr<IDXGISurface> pSurface;
	hr = pSwapChain->GetBuffer(0, _uuidof(IDXGISurface), &pSurface);
	ASSERT_IF_FAILED(hr, "Failed to obtain IDXGISurface from Swap Chain!")

	//Create 2D Context for graphics and draw commands
	hr = pDevice2D->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pContext2D);
	ASSERT_IF_FAILED(hr, "Failed to create 2D Device Context!");

	//Create a bitmap target of swapchain's buffer for 2D Context to draw on.
	ComPtr<ID2D1Bitmap1> pBitmap;
	D2D1_BITMAP_PROPERTIES1 bmpProps;
	bmpProps.dpiX = 0;
	bmpProps.dpiY = 0;
	bmpProps.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bmpProps.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bmpProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bmpProps.colorContext = nullptr;
	hr = pContext2D->CreateBitmapFromDxgiSurface(pSurface.Get(), bmpProps, &pBitmap);
	ASSERT_IF_FAILED(hr, "Failed to create bitmap context!");

	pContext2D->SetTarget(pBitmap.Get());


	setBrushColor({ 1, 0, 0, 1 });
}

void Graphics::beginFrame()
{
	pContext2D->BeginDraw();
	pContext2D->Clear(D2D1_COLOR_F{ 0.0f, 0.0f, 0.0f, 1.0f });
	drawGeometry(Geometry<5>{ { {0, 0}, { 100, 100 }, { 200, 150 }, { 300, 100 }, { 150, 50 }} }, true, { 0, 0, 0, 0 });
}

void Graphics::endFrame()
{
	pContext2D->EndDraw();
	HRESULT hr = pSwapChain->Present(1, 0);
	ASSERT_IF_FAILED(hr, "Swap Chain present error!");
}


void Graphics::setBrushColor(Color c)
{
	HRESULT hr = pContext2D->CreateSolidColorBrush(c, &pSCBrush);
	ASSERT_IF_FAILED(hr, "Failed to create Brush!");
}

void Graphics::drawLine(Point p1, Point p2, float strokeWidth = 1.0f)
{
	pContext2D->DrawLine(p1, p2, pSCBrush.Get(), strokeWidth);
}

void Graphics::drawPoint(Point p, float radius)
{
	pContext2D->DrawEllipse(D2D1_ELLIPSE{ p, radius, radius }, pSCBrush.Get());
	pContext2D->FillEllipse(D2D1_ELLIPSE{ p, radius, radius }, pSCBrush.Get());
}
