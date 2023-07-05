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
	class QuadBezierCurve;
}

struct Vector2f;
struct Event;

class IContainer;
class IDropDown;
class IButton;

class App : public Application
{
	public:
		static const int GRID_SIZE = 20;
		//Size of each partition of the grid square.
		static const int CHUNK_SIZE = 100;
		enum class State : unsigned char
		{
			CONVEX_HULL = 0x0,
			TRIANGULATE = 0x1,
			VORONOI = 0x2
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
		bool isDrawingAlgorithmVisualize();
		D2D::Point* getPoint(FLOAT2 pos);

		int getWidth();
		int getHeight();

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
		bool isCCW();
		
		void voronoiDiagramEventHandler(Event& e);

		bool startConvexHull();
		bool startTriangulation();
		void startVoronoiDiagram();
	private:
		void updateAlgorithmTypeDropdown();
		
		bool mDragging = false;
		D2D::Point* pSelectedPoint = nullptr;
		D2D::Polygon* pSelectedOutline = nullptr;
		std::vector<D2D::Line*> mGridLines;
			
		/********************* Algorithm Visualizer *************************/
		AlgorithmVisualizer* pVisualizer = nullptr;
		float mVisualizerSpeed;
		//Convex Hull
		std::vector<D2D::Line*> mHullLines;

		//Triangulate
		std::vector<Vector2f> mPolygon;
		std::vector<D2D::Line*> mTriangulationLines;
		std::vector<D2D::Line*> mPolygonLines;
		bool isValidPolygon = false;

		//Voronoi Diagram
		std::vector<D2D::Line*> mVoronoiEdges;

		//Map of chunks, or grid partitions, to make it faster to find points based on where the user clicks.
		std::map<FLOAT2, std::vector<D2D::Point*>, ChunkCompare> mPoints;
		State mState = State::CONVEX_HULL;

		AlgorithmVisualizer::ConvexHullAlgorithm mCHAlgorithm = AlgorithmVisualizer::ConvexHullAlgorithm::QUICK_HULL;
		AlgorithmVisualizer::TriangulationAlgorithm mTriAlgorithm = AlgorithmVisualizer::TriangulationAlgorithm::EAR_CLIPPING;
		AlgorithmVisualizer::VoronoiDiagramAlgorithm mVoronoiAlgorithm = AlgorithmVisualizer::VoronoiDiagramAlgorithm::FORTUNE;
		bool isDrawingAlgo = false;

		

		//*******************UI********************//
		IContainer* pRoot;
		IDropDown* pAlgorithmDropDown;
		IDropDown* pAlgorithmTypeDropDown;
		IButton* pStartButton;
		IButton* pClearButton;
		IButton* pPauseButton;
};