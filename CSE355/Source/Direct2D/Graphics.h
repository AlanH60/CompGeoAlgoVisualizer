#pragma once
#include "Types.h"
#include <d2d1_3.h>
#include <d3d11.h>
#include <wrl.h>

//Assert if an hr result has failed with a specified message.
#ifndef NDEBUG
	#define ASSERT_IF_FAILED(hr, message) if (!FAILED(hr)){} else{ assert(false && message); }
#else
	#define ASSERT_IF_FAILED(hr, message)
#endif

class Window;
struct Color;
class Graphics
{
	public:
		Graphics(Window* pWindow);
		~Graphics();
		//Sets up a new frame for draw calls.
		void beginFrame();
		//Displays the new frame.
		void endFrame();

		//Draw a rectangle
		void drawRect(FLOAT2 pos, float width, float height, bool filled, ID2D1SolidColorBrush* pBrush, ID2D1SolidColorBrush* pBorderBrush, float cornerRadius = 0.0f, float borderWidth = 1.0f);
		//Draw a line from points p1 to p2, using pBrush.
		void drawLine(FLOAT2 p1, FLOAT2 p2, ID2D1SolidColorBrush* pBrush, float strokeWidth = 1.0f);
		//Draw a point on the specified coordinate, using pBrush.
		void drawPoint(FLOAT2 p, ID2D1SolidColorBrush* pBrush, float radius = 5.0f);
		//Draw text
		void drawText(FLOAT2 pos, IDWriteTextLayout* pTextLayout, ID2D1SolidColorBrush* pBrush);
		//Draw dynamic text
		void drawText(std::wstring& text, FLOAT2 pos, IDWriteTextLayout* pTextLayout, ID2D1SolidColorBrush* pBrush);

		//Draw a path geometry created back createPathGeometry()
		void drawGeometry(ID2D1PathGeometry* pGeometry, ID2D1SolidColorBrush* pBrush, bool filled, FLOAT2 offset = { 0, 0 }, float scale = 1);
		//Create a brush of a specified color that is used for draw calls.
		void createSolidColorBrush(Color color, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush);
		//Create a path geometry, or shape, using a set of vertices.
		void createPathGeometry(FLOAT2* vertices, unsigned int vertexCount, bool filled, Microsoft::WRL::ComPtr<ID2D1PathGeometry>& pGeometry);
		//Create a text layout. For style, 0 - normal, 1 - oblique, 2 - italics.  For alignment, 0 - left, 1 - right, 2 - center.
		void createTextLayout(std::wstring& wstr, std::wstring& fontFamily, float size, bool bold, unsigned char style, 
			unsigned char textAlignment, unsigned char paragraphAlignment, float width, float height, IDWriteTextLayout** ppTextLayout);

		//Push layer such that all subsequent draw calls are drawn on this layer.
		void pushLayer(D2D1_RECT_F rect, ID2D1PathGeometry* pGeometry, D2D1_MATRIX_3X2_F& transform);
		//Pop a layer that was previously pushed.
		void popLayer();
		
		//Operation to handle the resize of the window.  This includes resizing the back buffers of pSwapChain.
		void onResize(int width, int height);
	private:
		//Interface that handles the presentation of frames and flipping of back buffers.
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		//Interface that handle draw calls and the creation of drawable objects.
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> pContext2D;
		//Factory used to create certain 2D interfaces like ID2D1Geometry and ID2D1DeviceContext
		Microsoft::WRL::ComPtr<ID2D1Factory2> pFactory2D;
		//Factory used to create text
		IDWriteFactory* pWriteFactory;
		//Layer used to region clipping.
		Microsoft::WRL::ComPtr <ID2D1Layer> pLayer;
};
