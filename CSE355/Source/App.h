#pragma once
#include "Direct2D/Application.h"
#include "Direct2D/Types.h"
#include "Algorithms.h"

namespace D2D
{
	class Drawable;
	class Point;
	class Line;
	class Polygon;
}

struct Vector2f;
class Event;

class IContainer;
class App : public Application
{
	public:
		static const unsigned int GRID_SIZE = 20;
		//Size of each partition of the grid square.
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
		void onUpdate() override;
		void onDraw() override;
		//Clear all drawables on the grid
		void clear();
		void addPoint(D2D::Point* pPoint);
		bool removePoint(D2D::Point* pPoint);
		bool deletePoint(D2D::Point* pPoint);
		D2D::Point* getPoint(FLOAT2 pos);

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
		AlgorithmVisualizer* pVisualizer = nullptr;
		bool mDragging = false;
		D2D::Point* pSelectedPoint = nullptr;
		D2D::Polygon* pSelectedOutline = nullptr;
		std::vector<D2D::Line*> mGridLines;

		//Convex Hull
		std::vector<D2D::Line*> mHullLines;

		//Triangulate
		std::vector<Vector2f> mPolygon;
		std::vector<D2D::Line*> mTriangulationLines;
		std::vector<D2D::Line*> mPolygonLines;
		bool isValidPolygon = false;

		//Map of chunks, or grid partitions, to make it faster to find points based on where the user clicks.
		std::map<FLOAT2, std::vector<D2D::Point*>, ChunkCompare> mPoints;
		State mState = CONVEX_HULL;

		AlgorithmVisualizer::ConvexHullAlgorithm mCHAlgorithm = AlgorithmVisualizer::ConvexHullAlgorithm::QUICK_HULL;
		AlgorithmVisualizer::TriangulationAlgorithm mTriAlgorithm = AlgorithmVisualizer::TriangulationAlgorithm::EAR_CLIPPING;

		//*******************UI********************//
		IContainer* pRoot;
};