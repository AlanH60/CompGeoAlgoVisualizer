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

struct Color;

/**
* Graphics class abstracts Direct2D API and exposes graphics API functionalities to the programmer. 
*/
class Graphics
{
	friend class Application;
	friend class Window;
	public:
		Graphics(Window* pWindow);
		~Graphics();

		/**
		* Draw a rectangle.
		* @param pos position of the top left corner of the rectangle.
		* @param width width of the rectangle.
		* @param height height of the rectangle.
		* @param filled flag whether the rectange should be filled.
		* @param pBrush pointer to the brush that will be used to fill the rectangle. (Created by createSolidColorBrush)
		* @param pBorderBrush pointer to the brush that will be used to stroke(draw the border of) the rectangle. (Created by createSolidColorBrush)
		* @param cornerRadius radius of rounded corners of the rectangle.
		* @param borderWidth width of the border.
		*/ 
		void drawRect(FLOAT2 pos, float width, float height, bool filled, ID2D1SolidColorBrush* pBrush, ID2D1SolidColorBrush* pBorderBrush, float cornerRadius = 0.0f, float borderWidth = 1.0f);
		/**
		* Draw a line.
		* @param p1 start point of the line.
		* @param p2 end point of the line.
		* @param pBrush pointer to the brush that will be used to color the line. (Created by createSolidColorBrush)
		* @param strokeWidth width of the line.
		*/
		void drawLine(FLOAT2 p1, FLOAT2 p2, ID2D1SolidColorBrush* pBrush, float strokeWidth = 1.0f);
		/**
		* Draw a point.
		* @param p coordinate of the point.
		* @param pBrush pointer to the brush that will be used to color the point. (Created by createSolidColorBrush)
		* @param radius size of the point.
		*/
		void drawPoint(FLOAT2 p, ID2D1SolidColorBrush* pBrush, float radius = 5.0f);
		/**
		* Draw text.
		* @param pos position of the top left corner of the text region.
		* @param pTextLayout pointer to the layout containing information on how to draw text.(Created by createTextLayout funct)
		* @param pBrush pointer to the brush that will be used to color the text. (Created by createSolidColorBrush)
		*/
		void drawText(FLOAT2 pos, IDWriteTextLayout* pTextLayout, ID2D1SolidColorBrush* pBrush);

		/**
		* Draw path geometry.
		* @param pGeometry pointer to the geometry. (Created by createPathGeometry)
		* @param pTextLayout pointer to the layout containing information on how to draw text.(Created by createTextLayout funct)
		* @param pBrush pointer to the brush that will be used to color the text. (Created by createSolidColorBrush)
		*/
		void drawGeometry(ID2D1PathGeometry* pGeometry, ID2D1SolidColorBrush* pBrush, bool filled, FLOAT2 offset = { 0, 0 }, float scale = 1);
		
		/**
		* Create a brush of a specified color that is used for draw calls.
		* @param color color of the brush.
		* @param pBrush comptr to a direct2d brush interface that should be filled with the new brush.
		*/ 
		void createSolidColorBrush(Color color, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& pBrush);
		/**
		* Create a path geometry, or shape, using a set of vertices..
		* @param vertices pointer to the array of vertices
		* @param vertexCount number of vertices in given array.
		* @param filled flag whether the path geometry should be filled.
		* @param pGeometry comptr to the direct2d geometry interface that should be filled with the new geometry.
		*/
		void createPathGeometry(FLOAT2* vertices, unsigned int vertexCount, bool filled, Microsoft::WRL::ComPtr<ID2D1PathGeometry>& pGeometry);
		/**
		* Create a quadratic bezier curve.
		* @param startPoitn start point of the bezier curve.
		* @param controlPoint control point of the bezier curve.
		* @param endPoint end point of the bezier curve.
		* @param pGeometry comptr to the direct2d geometry interface that should be filled with the new geometry.
		*/
		void createQuadraticBezierCurve(FLOAT2 startPoint, FLOAT2 controlPoint, FLOAT2 endPoint, Microsoft::WRL::ComPtr<ID2D1PathGeometry>& pGeometry);
		/**
		* Create a text layout used for drawing text.
		* @param wstr wide string containing the text.
		* @param fontFamily the font family of the text.
		* @param size font size.
		* @param bold flag whether the text should be bold.
		* @param style style of the text. (0 - normal, 1 - oblique, 2 - italics)
		* @param textAlignment horizontal alignment of the text. (0 - left, 1 - right, 2 - center)
		* @param paragraphAlignment vertical alignment of the text. (0 - top, 1 - bottom, 2 - center)
		* @param width width of the text region.
		* @param height height of the text region.
		* 
		*/
		void createTextLayout(std::wstring& wstr, std::wstring& fontFamily, float size, bool bold, unsigned char style, 
			unsigned char textAlignment, unsigned char paragraphAlignment, float width, float height, Microsoft::WRL::ComPtr<IDWriteTextLayout>& pTextLayout);

		/**
		* Push layer such that all subsequent draw calls are drawn on this layer.
		* @param rect rectangle that represents the bounds of the layer
		* @param pGeometry pointer to the direct2d geometry interface that represents the geometric mask for the layer.  Only those lying inside the mask is visible.
		* @param transform the transformation matrix done to the geometric mask
		*/
		void pushLayer(D2D1_RECT_F rect, ID2D1PathGeometry* pGeometry, D2D1_MATRIX_3X2_F& transform);
		/**
		* Pops the layer that has been pushed.  SHOULD only be called after pushLayer;
		*/
		void popLayer();

	private:
		//Sets up a new frame for draw calls. Called by application every frame.
		void beginFrame();
		//Displays the new frame. Called by application every frame.
		void endFrame();
		//Operation to handle the resize of the window, includeing resizing the back buffers of pSwapChain. Called within the window msg handler.
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
