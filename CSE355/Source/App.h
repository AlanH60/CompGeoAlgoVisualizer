#pragma once
#include "Direct2D/Application.h"
#include "Direct2D/Types.h"

struct Vector2f;
class Event;
class Point;
class Line;
class Polygon;
class Drawable;

class App : public Application
{
	public:
		//Size of each partition of the grid space.
		static const unsigned int CHUNK_SIZE = 100;
		enum State
		{
			CONVEX_HULL = 0x00,
			TRIANGULATE = 0x01
		};
	public:
		//Compare class used in mChunks
		class ChunkCompare
		{
			public:
				//Compares the chunks p1 and p2 are in.  Returns if p1's chunks should come before p2's chunk in mChunks.
				//If they are in different chunks, the x's are compared and the y's are compared to break ties.
				bool operator()(const FLOAT2& p1, const FLOAT2& p2) const
				{
					FLOAT2 p1Chunk = { (int)p1.x / CHUNK_SIZE, (int)p1.y / CHUNK_SIZE };
					FLOAT2 p2Chunk = { (int)p2.x / CHUNK_SIZE, (int)p2.y / CHUNK_SIZE };
					if (p1Chunk.x < p2Chunk.x)
						return true;
					else if (p1Chunk.x == p2Chunk.x && p1Chunk.y < p2Chunk.y)
						return true;
					return false;
				}
		};

	public:
		App();
		~App();
		void onDraw() override;
		//Clear all drawables on the grid
		void clear();
		void addPoint(Point* pPoint);
		bool removePoint(Point* pPoint);
		bool deletePoint(Point* pPoint);
		Point* getPoint(FLOAT2 pos);

		//Queue this from algorithm
		void setPointColor(FLOAT2 pos, Color color);

	private:
		template<typename T>
		void deleteAndClear(std::vector<T*>& v)
		{
			for (const T* ptr : v)
				delete ptr;
			v.clear();
		}
		void convexHullEventHandler(Event& e);

		void triangulateEventHandler(Event& e);
		void updatePolygonValidity();
	private:
		bool mDragging = false;
		Point* pSelectedPoint = nullptr;
		Polygon* pSelectedOutline = nullptr;
		std::vector<Line*> mGridLines;

		//Convex Hull
		std::vector<Line*> mHullLines;

		//Triangulate
		std::vector<Vector2f> mPolygon;
		std::vector<Line*> mTriangulationLines;
		std::vector<Line*> mPolygonLines;
		bool isValidPolygon = false;

		//Map of chunks, or grid partitions, to make it faster to find points based on where the user clicks.
		std::map<FLOAT2, std::vector<Point*>, ChunkCompare> mPoints;
		State mState = CONVEX_HULL;
};