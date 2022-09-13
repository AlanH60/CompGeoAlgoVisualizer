#pragma once
#include <d2d1_2.h>
#include <d3d11.h>
#include <wrl.h>


#define ASSERT_IF_FAILED(hr, message) if (!FAILED(hr)){} else{ assert(false && message); }

class Window;
class Graphics
{
	public:
		struct Point
		{
			float x, y;

			operator D2D1_POINT_2F()
			{
				return { x, y };
			}
		};
		template<unsigned int sides>
		struct Geometry
		{
			Point vertices[sides];
		};
		struct Color
		{
			float r, g, b, a;

			operator D2D1_COLOR_F()
			{
				return { r, g, b, a };
			}
		};
	public:
		Graphics(Window* pWindow);
		void beginFrame();
		void endFrame();

		void setBrushColor(Color c);
		void drawLine(Point p1, Point p2, float strokeWidth);
		void drawPoint(Point p, float radius);

		template<unsigned int v>
		void drawGeometry(Geometry<v> g, bool filled, Color fillColor)
		{
			HRESULT hr = pFactory2D->CreatePathGeometry(&pGeometry);
			ASSERT_IF_FAILED(hr, "Failed to create path geometry!");
			Microsoft::WRL::ComPtr<ID2D1GeometrySink> pSink;
			hr = pGeometry->Open(&pSink);
			ASSERT_IF_FAILED(hr, "Failed to open geometry sink!");
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
			pSink->BeginFigure(g.vertices[0], (filled) ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);
			pSink->AddLines(reinterpret_cast<D2D1_POINT_2F*>(&g.vertices[0]), v);
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

			hr = pSink->Close();
			ASSERT_IF_FAILED(hr, "Failed to close geometry sink!");
			
			pContext2D->DrawGeometry(pGeometry.Get(), pSCBrush.Get());
			if (filled)
				pContext2D->FillGeometry(pGeometry.Get(), pSCBrush.Get());
		}
		
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID2D1Device> pDevice2D;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> pContext2D;
		Microsoft::WRL::ComPtr<ID2D1Factory2> pFactory2D;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pSCBrush;
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> pGeometry;
};
