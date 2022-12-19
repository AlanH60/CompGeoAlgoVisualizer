#pragma once
#include "Direct2D/Drawable/Drawable.h"
#include "Direct2D/Event.h"
class IComponent
{
	public:
		IComponent(int x, int y, int width, int height);
		virtual ~IComponent();
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		void setX(int x);
		void setY(int y);
		void setPos(int x, int y);
		virtual void setWidth(int width);
		virtual void setHeight(int height);
		virtual void onPress(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onHover(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onExit(MouseEvent& mouseEvent) {}
		virtual void onMove(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onClick(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onDrag(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onDragRelease(int x, int y, MouseEvent& mouseEvent) {}
		virtual void onKeyPress(KeyEvent& keyEvent) {}
		virtual void onChar(CharEvent& charEvent) {}
		virtual void onFocusLoss() {}
		virtual void onEvent(Event& e)
		{
			if (e.isConsumed)
				return;
		}
		virtual void onUpdate() {};
		//Draws the component relative to origin.
		virtual void draw(int originX, int originY)
		{
			for (D2D::Drawable* d : mDrawables)
			{
				d->setPos(FLOAT2{ (float)originX + mPosX ,  (float)originY + mPosY });
				d->draw();
			}
		}
		//Returns true if the point (x,y) is within the bounds of the component
		virtual bool inComponent(int x, int y)
		{
			return (x >= mPosX && x <= mPosX + mWidth && y >= mPosY && y <= mPosY + mHeight);
		}

		protected:
			//x and y coordinates of the bottom-left of the component relative to the origin of its parent.
			int mPosX, mPosY;
			//Width and height of the component
			int mWidth, mHeight;
			//Drawables associated with component
			std::vector<D2D::Drawable*> mDrawables;
};