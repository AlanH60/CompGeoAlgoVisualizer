#pragma once
#include "Direct2D/Drawable/Drawable.h"
#include "Direct2D/Event.h"
class IComponent
{
	public:
		///XOrientation indicates the component's x-value relative to its parent container.
		enum class XOrientation
		{
			RELATIVEX, ///The component's x-value is relative to the x-position of the parent container. (Default)
			CENTER, ///The component's x-value is adjusted such that it is horizontally centered within the parent container.
			LEFT, ///The component's x-value is adjusted such that it is aligned to the left of the parent container.
			RIGHT, ///The component's x-value is adjusted such that it is aligned to the right of the parent container.
			RELATIVE_WIDTH ///The component's x-value is adjusted such that it is a factor of the parent container's width.
		};
		///YOrientation indicates the component's x-value relative to its parent container.
		enum class YOrientation
		{
			RELATIVEY, ///The component's y-value is relative to the y-position of the parent container. (Default)
			CENTER, ///The component's y-value is adjusted such that it is horizontally centered within the parent container.
			TOP, ///The component's y-value is adjusted such that it is aligned to the top of the parent container.
			BOTTOM, ///The component's y-value is adjusted such that it is aligned to the bottom of the parent container.
			RELATIVE_HEIGHT ///The component's y-value is adjusted such that it is a factor of the parent container's height.
		};
		///XDimension indicates the component's width relative to its parent container
		enum class XDimension
		{
			ABSOLUTEX, ///The component's width is not dependent on parent container.  (Default)
			RELATIVEX ///The component's width is a factor of the parent container's width.
		};
		///YDimension indicates the component's width relative to its parent container
		enum class YDimension
		{
			ABSOLUTEY, ///The component's height is not dependent on parent container.  (Default)
			RELATIVEY ///The component's height is a factor of the parent container's width.
		};
	public:
		IComponent(float x, float y, float width, float height);
		virtual ~IComponent();
		FLOAT2 getPos();
		float getWidth();
		float getHeight();
		bool getDirtyFlag();

		/**
		* Sets the x position of the component.
		* @param x new x-coordinate of the component.
		*/
		void setX(float x);
		/**
		* Sets the y position of the component.
		* @param y new y-coordinate of the component.
		*/
		void setY(float y);
		/**
		* Sets the position of the component.
		* @param pos new position of the component.
		*/
		void setPos(FLOAT2 pos);
		/**
		* Sets the position of the component.
		* @param x x-coordinate.
		* @param y y-coordinate.
		*/
		void setPos(float x, float y);
		/**
		* Sets the XOrientation setting for the component.
		* @param xOrientation XOrientation for the component.
		*/
		void setXOrientation(XOrientation xOrientation);
		/**
		* Sets the YOrientation setting for the component.
		* @param yOrientation YOrientation for the component.
		*/
		void setYOrientation(YOrientation yOrientation);
		/**
		* Sets the relative-width orientation of the component.
		* @param rHeightOrientation factor of the parent container's width that the x-coordinate should be.
		*/
		void setRelativeWidthOrientation(float rWidthOrientation);
		/**
		* Sets the relative-height orientation of the component.
		* @param rHeightOrientation factor of the parent container's height that the y-coordinate should be.
		*/
		void setRelativeHeightOrientation(float rHeightOrientation);
		/**
		* Sets the XDimension setting for the component.
		* @param xDimension XDimension for the component.
		*/
		void setXDimension(XDimension xDimension);
		/**
		* Sets the YDimension setting for the component.
		* @param yDimension YDimension for the component.
		*/
		void setYDimension(YDimension yDimension);
		/**
		* Sets the relative width of the component.
		* @param rWidth new relative width of the component.
		*/
		void setRelativeWidth(float rWidth);
		/**
		* Sets the relative height of the component.
		* @param rHeight new relative height of the component.
		*/
		void setRelativeHeight(float rHeight);
		/**
		* Sets the dirty flag of the component.
		* @param isDirty dirty flag.
		*/
		void setDirtyFlag(bool isDirty);

		virtual void setColor(const Color& color);
		/**
		* Sets the width of the component.  Child classes overriding this function should invoke this function.
		* @param width new width of the component.
		*/
		virtual void setWidth(float width);
		/**
		* Sets the height of the component.  Child classes overriding this function should invoke this function.
		* @param height new height of the component.
		*/
		virtual void setHeight(float height);
		/**
		* Function that is called when the user presses their mouse within the component.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onPress(float x, float y, MouseEvent& mouseEvent) {}
		/**
		* Function that is called when the user's mouse hovers/enters the component.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onHover(float x, float y, MouseEvent& mouseEvent) {}
		/**
		* Function that is called when the user's mouse exits the component.
		*/
		virtual void onExit() {}
		/**
		* Function that is called each time the user moves their mouse within this container.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onMove(float x, float y, MouseEvent& mouseEvent) {}
		/**
		* Function that is called when the user clicks this component.(Mouse is released)
		* @param x x-coordinate of the click relative to the parent container.
		* @param y y-coordinate of the click relative to the parent container.
		* @param mouseEvent reference to the mouse event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onClick(float x, float y, MouseEvent& mouseEvent) {}
		/**
		* Function that is called each time the user moves their mouse when dragging this component.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onDrag(float x, float y, MouseEvent& mouseEvent) {}
		/**
		* Function that is called if this component was being dragged and the user released.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onDragRelease(float x, float y, MouseEvent& mouseEvent) {}
		/**
		* Function that is called if this component is currently focused by the user and a key is pressed.
		* @param keyEvent reference to the key event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onKeyPress(KeyEvent& keyEvent) {}
		/**
		* Function that is called if this component is currently focused by the user and a character is pressed.
		* @param charEvent reference to the char event. Can be consumed such that no other component retrieves this event.
		*/
		virtual void onChar(CharEvent& charEvent) {}
		/**
		* Function that is called when the user clicks away from this component.
		*/
		virtual void onFocusLoss() {}
		/**
		* Handles events. Used exclusively by the root container to begin probagation of events.
		* Child classes have no need of implementing this function.
		* @param e event to be handled.
		*/
		virtual void onEvent(Event& e);
		/**
		* Updates the component if dirty flag is true.  This is where XYOrientations and XYDimensions are enforced.
		* Child classes overriding this function must call this function.
		* @param parent parent of the component.
		*/
		virtual void onUpdate(IComponent* parent);
		/**
		* Draws the component relative to the inputed origin.
		* @param originX absolute x-coordinate of origin.
		* @param originY absolute y-coordinate of origin.
		*/
		virtual void draw(float originX, float originY);
		/**
		* Check whether a given point is within the component.  Can be overriden to specialize for implementing classes.
		* @param x x-coordinate of the point.
		* @param y y-coordinate of the point.
		* @returns true if the point is within the bounds of the component.
		*/
		virtual bool inComponent(float x, float y);		
	protected:
		//Coordinates of the top-left of the component relative to the origin of its parent.
		FLOAT2 mPos;
		//Width and height of the component
		float mWidth, mHeight;
		//Flag used to indicate whether its orientation or dimensions should be updated
		bool isDirty = false;
		//Orientation
		XOrientation mXOrientation = XOrientation::RELATIVEX;
		YOrientation mYOrientation = YOrientation::RELATIVEY;
		/*
		 If RELATIVE_WIDTH is chosen for mXOrientation, this value determines the factor of the parent's width this 
		 component's position will be in.
		 Ex: mRWidthOrientation = 0.5f ==> mPosX = 0.5f * pParent->getWidth()
		*/
		float mRWidthOrientation = 0.0f;
		/*
		 If RELATIVE_HEIGHT is chosen for mYOrientation, this value determines the factor of the parent's height this 
		 component's position will be in.
		 Ex: mRHeightOrientation = 0.5f ==> mPosY = 0.5f * pParent->getHeight()
		*/
		float mRHeightOrientation = 0.0f;
		//XDimension setting of the component
		XDimension mXDimension = XDimension::ABSOLUTEX;
		//YDimension setting of the component
		YDimension mYDimension = YDimension::ABSOLUTEY;
		//If RELATIVEX is chosen for the mXDimension, mWidth = mRelativeWidth * pParent->getWidth().
		float mRelativeWidth = 1.0f;
		//If RELATIVEY is chosen for the mXDimension, mHeight = mRelativeHeight * pParent->getHeight().
		float mRelativeHeight = 1.0f;
		//Vector of drawables that make up this component.
		std::vector<D2D::Drawable*> mDrawables;
		//Color of the component
		Color mColor = Color{ 0, 0, 0, 0 };
};