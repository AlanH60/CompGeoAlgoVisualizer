#pragma once
#include "Direct2D/Application.h"
#include "Direct2D/Types.h"

class Point;
class Line;
class Polygon;
class Drawable;
class App : public Application
{
	public:
		//Size of each partition of the grid space.
		static const unsigned int CHUNK_SIZE = 100;
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
		void addDrawable(Drawable* pDrawable);
		bool removeDrawable(Drawable* pDrawable);
		bool deleteDrawable(Drawable* pDrawable);
		Drawable* getDrawable(FLOAT2 pos);
	private:
		bool mDragging = false;
		Point* pSelectedPoint = nullptr;
		Polygon* pSelectedOutline = nullptr;
		std::vector<Line*> mGridLines;
		std::vector<Line*> mHullLines;
		//Map of chunks, or grid partitions, to make it faster to find points based on where the user clicks.
		std::map<FLOAT2, std::vector<Drawable*>, ChunkCompare> mDrawables;
};