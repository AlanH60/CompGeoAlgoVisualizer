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
		//Sets up a new frame for draw calls.
		void beginFrame();
		//Displays the new frame.
		void endFrame();

		//Draw a line from points p1 to p2, using pBrush.
		void drawLine(FLOAT2 p1, FLOAT2 p2, ID2D1SolidColorBrush* pBrush, float strokeWidth = 1.0f);
		//Draw a point on the specified coordinate, using pBrush.
		void drawPoint(FLOAT2 p, ID2D1SolidColorBrush* pBrush, float radius = 10.0f);

		//Draw a path geometry created back createPathGeometry()
		void drawGeometry(ID2D1PathGeometry* pGeometry, ID2D1SolidColorBrush* pBrush, bool filled, FLOAT2 offset = { 0, 0 });
		//Create a brush of a specified color that is used for draw calls.
		void createSolidColorBrush(Color color, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush);
		//Create a path geometry, or shape, using a set of vertices.
		void createPathGeometry(FLOAT2* vertices, unsigned int vertexCount, bool filled, Microsoft::WRL::ComPtr<ID2D1PathGeometry>& pGeometry);
		
		//Operation to handle the resize of the window.  This includes resizing the back buffers of pSwapChain.
		void onResize(int width, int height);
	private:
		//Interface that handles the presentation of frames and flipping of back buffers.
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		//Interface that handle draw calls and the creation of drawable objects.
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> pContext2D;
		//Factory used to create certain 2D interfaces like ID2D1Geometry and ID2D1DeviceContext
		Microsoft::WRL::ComPtr<ID2D1Factory2> pFactory2D;
};
