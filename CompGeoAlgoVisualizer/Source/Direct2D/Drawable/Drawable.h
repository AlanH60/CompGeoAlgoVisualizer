#pragma once
#include "../Graphics.h"
#include "../Color.h"
#include "../DrawRes/Brush.h"

namespace D2D
{
	/**
	* The Drawable class represents objects that can be drawn on the screen.
	*/ 
	class Drawable
	{
		friend Graphics;
		public:
			Drawable(FLOAT2 pos, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
			virtual ~Drawable() = default;
			FLOAT2 getPos();
			FLOAT2 getOffset();
			/**
			* Sets the position of the drawable.
			* pos - coordinates of the drawable.
			*/
			void setPos(FLOAT2 pos);
			/**
			* Sets the offset of the drawable.  This drawable will be drawn at pos + offset.
			* offset - vector representing the offset
			*/ 
			void setOffset(FLOAT2 offset);
			/**
			* Sets the color of the drawable's brush.
			* color - the new color.
			*/
			void setColor(Color color);
			/**
			* Sets the visibility of the drawable.  If visibility is false, this drawable will not be drawn.
			* visibility - flag whether this drawable should be visible.
			*/
			void setVisibility(bool visibility);
			/**
			* Draw the drawable.  Implementation required in child classes.  
			* Child classes must check isVisible flag before drawing, unless it is intentionally ignored.
			*/
			virtual void draw() = 0;
		private:
			//Set graphics links the drawable class and graphics API.  Done upon creation of graphics object.
			static void setGraphics(Graphics* gfx);

		protected:
			static Graphics* pGraphics;
			//The brush that should be used to color this drawable.
			std::unique_ptr<Brush> pBrush = nullptr;
			FLOAT2 mPos = { 0, 0 };
			//Used to maintain structure when drawables are in a group, where all drawables within the group have the same position.
			FLOAT2 mOffset = { 0, 0 };
			//Flag indicating whether to draw this drawable or not.
			bool isVisible;
	};
}
