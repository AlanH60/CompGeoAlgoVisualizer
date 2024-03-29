#include "PCH.h"
#include "Graphics.h"
#include "Window.h"
#include "Color.h"
#include "Drawable/Drawable.h"
#include <dwrite.h>

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


	ComPtr<ID2D1Device> pDevice2D;
	hr = pFactory2D->CreateDevice(pIDXGIDevice.Get(), &pDevice2D);
	ASSERT_IF_FAILED(hr, "Failed to create 2D Device!");

	//Query for the DXGISurface/backbuffer of the swap chain.
	ComPtr<IDXGISurface> pSurface;
	hr = pSwapChain->GetBuffer(0, _uuidof(IDXGISurface), &pSurface);
	ASSERT_IF_FAILED(hr, "Failed to obtain IDXGISurface from Swap Chain!");

	//Create 2D Context for graphics and draw commands
	hr = pDevice2D->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pContext2D);
	ASSERT_IF_FAILED(hr, "Failed to create 2D Device Context!");

	//Create a bitmap target pointing at the back buffer of the swap chain.
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

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory));

	ASSERT_IF_FAILED(hr, "Failed to create text factory!");

	ASSERT_IF_FAILED(pContext2D->CreateLayer(NULL, &pLayer), "Failed to create layer!");

	pWindow->setGraphics(this);
	D2D::Drawable::setGraphics(this);
}

Graphics::~Graphics()
{
	pWriteFactory->Release();
}

void Graphics::beginFrame()
{
	pContext2D->BeginDraw();
	pContext2D->Clear(D2D1_COLOR_F{ 1.0f, 1.0f, 1.0f, 1.0f });
}

void Graphics::endFrame()
{
	pContext2D->EndDraw();
	HRESULT hr = pSwapChain->Present(1, 0);
	ASSERT_IF_FAILED(hr, "Swap Chain presentation error!");
}

void Graphics::drawRect(FLOAT2 pos, float width, float height, bool filled, ID2D1SolidColorBrush* pBrush, ID2D1SolidColorBrush* pBorderBrush,
	float cornerRadius, float borderWidth)
{
	D2D1_RECT_F rect = { pos.x, pos.y + height, pos.x + width, pos.y };
	if (cornerRadius != 0)
	{
		if (filled)
			pContext2D->FillRoundedRectangle({ rect, cornerRadius, cornerRadius }, pBrush);
		pContext2D->DrawRoundedRectangle({ rect, cornerRadius, cornerRadius }, pBorderBrush, borderWidth);
		
	}
	else
	{
		if (filled)
			pContext2D->FillRectangle(rect, pBrush);
		pContext2D->DrawRectangle(rect, pBorderBrush, borderWidth);
	}
}


void Graphics::drawLine(FLOAT2 p1, FLOAT2 p2, ID2D1SolidColorBrush* pBrush, float strokeWidth)
{
	pContext2D->DrawLine({ p1.x, p1.y }, { p2.x, p2.y }, pBrush, strokeWidth);
}

void Graphics::drawPoint(FLOAT2 p, ID2D1SolidColorBrush* pBrush, float radius)
{
	pContext2D->DrawEllipse(D2D1_ELLIPSE{ p.x, p.y, radius, radius}, pBrush);
	pContext2D->FillEllipse(D2D1_ELLIPSE{ p.x, p.y, radius, radius}, pBrush);
}

void Graphics::drawText(FLOAT2 pos, IDWriteTextLayout* pTextLayout, ID2D1SolidColorBrush* pBrush)
{
	pContext2D->DrawTextLayout({ pos.x, pos.y }, pTextLayout, pBrush);
}


void Graphics::drawGeometry(ID2D1PathGeometry* pGeometry, ID2D1SolidColorBrush* pBrush, bool filled, FLOAT2 offset, float scale)
{
	pContext2D->SetTransform(D2D1_MATRIX_3X2_F { scale, 0, 0, scale, offset.x, offset.y });
	pContext2D->DrawGeometry(pGeometry, pBrush);
	if (filled)
		pContext2D->FillGeometry(pGeometry, pBrush);
	pContext2D->SetTransform({ 1, 0, 0, 1, 0, 0 });
}

void Graphics::createSolidColorBrush(Color color, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush)
{
	HRESULT hr = pContext2D->CreateSolidColorBrush(color, &pBrush);
	ASSERT_IF_FAILED(hr, "Failed to create solid color brush!");
}

void Graphics::createPathGeometry(FLOAT2* vertices, unsigned int vertexCount, bool filled, Microsoft::WRL::ComPtr<ID2D1PathGeometry>& pGeometry)
{
	HRESULT hr = pFactory2D->CreatePathGeometry(&pGeometry);
	ASSERT_IF_FAILED(hr, "Failed to create path geometry!");
	Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink;
	hr = pGeometry->Open(&pSink);
	ASSERT_IF_FAILED(hr, "Failed to open geometry sink!");
	pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
	pSink->BeginFigure({ vertices[0][0], vertices[0][1] }, (filled) ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);
	pSink->AddLines(reinterpret_cast<D2D1_POINT_2F*>(&vertices[0]), vertexCount);
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

	hr = pSink->Close();
	ASSERT_IF_FAILED(hr, "Failed to close geometry sink!");
}

void Graphics::createQuadraticBezierCurve(FLOAT2 startPoint, FLOAT2 controlPoint, FLOAT2 endPoint, Microsoft::WRL::ComPtr<ID2D1PathGeometry>& pGeometry)
{
	HRESULT hr = pFactory2D->CreatePathGeometry(&pGeometry);
	ASSERT_IF_FAILED(hr, "Failed to create path geometry!");
	Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink;
	hr = pGeometry->Open(&pSink);
	ASSERT_IF_FAILED(hr, "Failed to open geometry sink!");
	pSink->BeginFigure({ startPoint.x, startPoint.y }, D2D1_FIGURE_BEGIN_FILLED);
	pSink->AddQuadraticBezier({ {controlPoint.x, controlPoint.y }, { endPoint.x, endPoint.y } });
	pSink->EndFigure(D2D1_FIGURE_END_OPEN);

	hr = pSink->Close();
	ASSERT_IF_FAILED(hr, "Failed to close geometry sink!");
}

void Graphics::createTextLayout(const std::wstring& wstr, const std::wstring& fontFamily, float size, bool bold, unsigned char style, unsigned char textAlignment, unsigned char paragraphAlignment, float width, float height, ComPtr<IDWriteTextLayout>& pTextLayout)
{
	HRESULT hr;
	ComPtr<IDWriteTextFormat> pTextFormat;
	hr = pWriteFactory->CreateTextFormat(fontFamily.c_str(), NULL, (bold) ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
		(DWRITE_FONT_STYLE)style, DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", &pTextFormat);
	ASSERT_IF_FAILED(hr, "Failed to create text format!");
	pTextFormat->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)textAlignment);
	pTextFormat->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)paragraphAlignment);
	hr = pWriteFactory->CreateTextLayout(wstr.c_str(), wstr.length(), pTextFormat.Get(), width, height, &pTextLayout);
	ASSERT_IF_FAILED(hr, "Failed to create text layout!");
	hr = pTextLayout->SetWordWrapping({ DWRITE_WORD_WRAPPING_NO_WRAP });
	ASSERT_IF_FAILED(hr, "Failed to set word wrapping!");
}

void Graphics::pushLayer(const D2D1_RECT_F& rect, ID2D1PathGeometry* pGeometry, const D2D1_MATRIX_3X2_F& transform)
{
	D2D1_LAYER_PARAMETERS layerParams = D2D1::LayerParameters(rect, pGeometry, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, transform);
	pContext2D->PushLayer(layerParams, pLayer.Get());
}

void Graphics::popLayer()
{
	pContext2D->PopLayer();
}

void Graphics::onResize(int width, int height)
{
	//Obtain the buffer from swap chain
	ComPtr<IDXGISurface> pSurface;
	HRESULT hr = pSwapChain->GetBuffer(0, _uuidof(IDXGISurface), &pSurface);
	ASSERT_IF_FAILED(hr, "Failed to obtain IDXGISurface from Swap Chain!");
	pSurface.Reset(); //Buffer has to be released before resizing
	pContext2D->SetTarget(NULL);

	//Recreate the buffers with the same flags and format.
	DXGI_SWAP_CHAIN_DESC desc;
	hr = pSwapChain->GetDesc(&desc);
	ASSERT_IF_FAILED(hr, "Failed to obtain swap chain description");
	pSwapChain->ResizeBuffers(0, width, height, desc.BufferDesc.Format, desc.Flags);

	//Obtain the new buffer as a IDXGI surface.
	hr = pSwapChain->GetBuffer(0, _uuidof(IDXGISurface), &pSurface);
	ASSERT_IF_FAILED(hr, "Failed to obtain IDXGISurface from Swap Chain!");

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
}
