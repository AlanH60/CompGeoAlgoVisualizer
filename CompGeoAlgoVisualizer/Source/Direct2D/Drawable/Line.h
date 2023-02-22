#pragma once
#include "Drawable.h"
#include "../Types.h"

namespace D2D
{
	/**
	* Line class represents a drawable line.
	*/ 
	class Line : public Drawable
	{
		public:
			Line(FLOAT2 p1, FLOAT2 p2, Color color = Color{ 1.0f, 0.0f, 0.0f, 1.0f });
			FLOAT2 getP1();
			FLOAT2 getP2();
			/**
			* Draw the line.
			*/ 
			void draw() override;
			/**
			* Set new end points for the line.
			* @param p1 end point 1
			* @param p2 end point 2
			*/ 
			void setPoints(FLOAT2 p1, FLOAT2 p2);
		private:
			//End points of the line.
			FLOAT2 mP1, mP2;
	};
}
