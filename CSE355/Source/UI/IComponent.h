#pragma once
#include "Direct2D/Drawable/Drawable.h"
#include "Direct2D/Event.h"
class IComponent
{
	public:
		enum class XOrientation
		{
			RELATIVEX,
			CENTER,
			LEFT,
			RIGHT,
			RELATIVE_WIDTH
		};
		enum class YOrientation
		{
			RELATIVEY,
			CENTER,
			TOP,
			BOTTOM,
			RELATIVE_HEIGHT
		};
		enum class XDimension
		{
			ABSOLUTEX,
			RELATIVEX
		};
		enum class YDimension
		{
			ABSOLUTEY,
			RELATIVEY
		};
	public:
		IComponent(int x, int y, int width, int height);
		virtual ~IComponent();
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		bool getDirtyFlag();
		void setX(int x);
		void setY(int y);
		void setXOrientation(XOrientation xOrientation);
		void setYOrientation(YOrientation yOrientation);
		void setRelativeWidthOrientation(float rWidthOrientation);
		void setRelativeHeightOrientation(float rHeightOrientation);
		void setXDimension(XDimension xDimension);
		void setYDimension(YDimension yDimension);
		void setRelativeWidth(float rWidth);
		void setRelativeHeight(float rHeight);
		void setPos(int x, int y);
		virtual void setColor(const Color& color);
		virtual void setDirtyFlag(bool isDirty);
		virtual void setWidth(int width);
		virtual void setHeight(int height);
		virtual void onPress(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onHover(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onExit() {}
		virtual void onMove(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onClick(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onDrag(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onDragRelease(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onKeyPress(KeyEvent& keyEvent) {}
		virtual void onChar(CharEvent& charEvent) {}
		virtual void onFocusLoss() {}
		virtual void onEvent(Event& e);
		virtual void onUpdate(IComponent* parent);
		//Draws the component relative to origin.
		virtual void draw(int originX, int originY);
		//Returns true if the point (x,y) is within the bounds of the component
		virtual bool inComponent(int x, int y);		
	protected:
		//x and y coordinates of the bottom-left of the component relative to the origin of its parent.
		int mPosX, mPosY;
		//Width and height of the component
		int mWidth, mHeight;
		//Flag used to indicate whether its orientation or dimensions should be updated
		bool isDirty = false;
		//Orientation
		XOrientation mXOrientation = XOrientation::RELATIVEX;
		YOrientation mYOrientation = YOrientation::RELATIVEY;
		//If RELATIVE_WIDTH or RELATIVE_HEIGHT orientations are chosen, 
		//these values determine the factor of the parent's dimensions this component's position will be in.
		//Ex: mRWidthOrientation = 0.5f ==> mPosX = 0.5f * pParent->getWidth()
		float mRWidthOrientation = 0.0f;
		float mRHeightOrientation = 0.0f;
		//Dimensions relative to parent or absolute
		XDimension mXDimension = XDimension::ABSOLUTEX;
		YDimension mYDimension = YDimension::ABSOLUTEY;
		float mRelativeWidth = 1.0f;
		float mRelativeHeight = 1.0f;
		//Drawables associated with component
		std::vector<D2D::Drawable*> mDrawables;
		//Color of the component
		Color mColor = Color{ 0, 0, 0, 0 };
};