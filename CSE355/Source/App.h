#pragma once
#include "Direct2D/Application.h"
#include "Direct2D/Types.h"

class Point;
class Polygon;
class App : public Application
{
	public:
		class PointCompare
		{
			public:
				bool operator()(const FLOAT2& p1, const FLOAT2& p2) const 
				{
					if (std::abs(p1.x - p2.x) < 10.0f && std::abs(p1.y - p2.y) < 10.0f)
						return false;
					if (p1.x < p2.x)
						return true;
					if (p1.x > p2.x)
						return false;
					if (p1.y < p2.y)
						return true;
					if (p1.y > p2.y)
						return false;
				}
		};

	public:
		App();
		~App();
		void onDraw() override;
		void clearPoints();
	private:
		bool mDragging = false;
		Point* pSelectedPoint = nullptr;
		Polygon* pSelectedOutline = nullptr;
		std::map<FLOAT2, Point*, PointCompare> mPoints;
};