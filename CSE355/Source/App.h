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
		static const unsigned int CHUNK_SIZE = 100;
	public:
		class ChunkCompare
		{
			public:
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
		std::map<FLOAT2, std::vector<Drawable*>, ChunkCompare> mChunks;
};