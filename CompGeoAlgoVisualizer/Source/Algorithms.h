#pragma once
#include "Primatives.h"
#include "DataStructs/Edge.h"
#include "DataStructs/DCEL.h"
#include "DataStructs/BeachLineStatus.h"

#define WAIT(pVisualizer, multiplier)			\
do												\
{												\
	pVisualizer->getDrawMutex().unlock();		\
	pVisualizer->wait(multiplier);				\
	pVisualizer->getDrawMutex().lock();			\
} while (false)

class App;
namespace D2D
{
	class Drawable;
	class Line;
	class Point;
	class QuadBezierCurve;
}


class AlgorithmVisualizer
{
	public:
		//Indicates whether the visualizer is idle, running an algorithm, or finished.  
		//Results of an algorithm can be obtained when it is in a finished state.
		enum class State
		{
			IDLE, //The no algorithm is running
			RUNNING, //Algorithm is running
			FINISHED //Algorithm is done and result is ready to be queried by App.
		};
		enum class ConvexHullAlgorithm
		{
			GRAHAM_SCAN = 0,
			GIFT_WRAPPING = 1,
			QUICK_HULL = 2
		};
		enum class TriangulationAlgorithm
		{
			EAR_CLIPPING = 0,
			SWEEP = 1
		};
		enum class VoronoiDiagramAlgorithm
		{
			FORTUNE = 0
		};
	protected:
		//EAR-CLIPPING: Contains the index of the previous vertex, next vertex, whether this vertex is an ear, and whether is has been clipped.
		struct VertexStatus
		{
			size_t prev = 0;
			size_t next = 0;
			bool isEar = false;
			bool isClipped = false;
		};
		//Sweep algorithm
		//Comparison class used to sort points by greatest y-value.
		class VertexCompareY
		{
			public:
				//Return true if v1 has lesser y value than v2. X value used as tie breaker.
				bool operator()(const Vector2f& v1, const Vector2f& v2)
				{
					return v1.y < v2.y || (v1.y == v2.y && v1.x > v2.x);
				}
		};

	public:
		AlgorithmVisualizer(App* pApp);
		~AlgorithmVisualizer();
		/**
		* Gets the current state of the algorithm visualizer. 
		* @return current state of the algorithm visualizer.
		*/
		State getState();
		/**
		* Check whether the algorithm visualizer is idle(No algorithm is running).
		* @returns true if no algorithm is running.
		*/
		bool isIdle();
		/**
		* Check whether the algorithm visualizer is running.
		* @returns true if algorithm visualizer is running.
		*/
		bool isRunning();
		/**
		* Check whether the algorithm visualizer is finished(Results can be obtained).
		* @returns true if algorithm visualizer is finished.
		*/
		bool isFinished();

		/**
		* TODO: Flag whether the algorithm should produce visualization of the algorithm.
		*/
		bool shouldVisualize();

		/**
		* Check whether the algorithm visualizer is paused.
		* @returns true if algorithm visualizer is paused.
		*/
		bool shouldPause();

		/**
		* Set the should visualize flag
		* @param visualize the new value of the flag.
		*/
		void setVisualization(bool visualize);
		/**
		* Set the should pause flag.
		* @param shouldPause flag whether the visualizer should be paused.
		*/
		void setShouldPause(bool shouldPause);
		/**
		* Get the speed of the algorithm
		* @returns speed of the algorithm(1 - 10)
		*/
		float getSpeed();
		/**
		* Sets the speed of the algorithm.
		* @param speed new speed.
		*/
		void setSpeed(float speed);
		/**
		* Adds to the speed of the algorithm.
		* @param modifier value to add to speed.
		*/ 
		void addSpeed(float modifier);
		/**
		* Get the lines produced by the algorithm.
		* @returns vector of pointers to lines produced by the algorithm.
		*/
		std::vector<D2D::Line*>& getLines();
		/**
		* Get the points produced by the algorithm.
		* @returns vector of pointers to points produced by the algorithm.
		*/
		std::vector<D2D::Point*>& getPoints();
		/**
		* Get the arcs produced by the algorithm.
		* @returns vector of pointers to QuadBezierCurves produced by the algorithm.
		*/
		std::vector<D2D::QuadBezierCurve*>& getArcs();
		/**
		* Starts the algorithm thread to compute the convex hull of a set of points.
		* @param points vector to the points.
		* @param algorithm convex hull algorithm to use.
		*/
		void computeConvexHull(std::vector<Vector2f>& points, ConvexHullAlgorithm algorithm = ConvexHullAlgorithm::QUICK_HULL);
		/**
		* Starts the algorithm thread to compute the triangulation of a polygon or PSLG.
		* @param polygon vector to the points.
		* @param edges vector of edges. (Used for PSLG)
		* @param algorithm triangulation algorithm to use.
		*/
		void computeTriangulation(std::vector<Vector2f>& polygon, std::unordered_map<Vector2f, std::vector<Vector2f>>& edges, TriangulationAlgorithm algorithm = TriangulationAlgorithm::EAR_CLIPPING);
		/**
		* Starts the algorithm thread to compute the voronoi diagram of a set of points.
		* @param points vector to the points.
		* @param algorithm voronoi diagram algorithm to use.
		*/
		void computeVoronoiDiagram(std::vector<Vector2f>& points, VoronoiDiagramAlgorithm algorithm = VoronoiDiagramAlgorithm::FORTUNE);
		/**
		* Get the resulting edges/lines associated with the algorithm result.
		* @returns a vector of pairs of points.
		*/
		std::vector<Edge> getResult();
		/**
		* Get the draw mutex of the algorithm visualizer. Allows the caller to lock and unlock within their own scope.
		* @returns reference to the draw mutex.
		*/
		std::mutex& getDrawMutex();
	private:
		//Sets the state of the algorithm.
		void setState(State state);
		//Clears all lines, points, and arcs generated by the visualizer.
		void clear();
		//Pauses the thread for a specific time depending on the speed and multiplier.
		void wait(float multiplier);
		//Called to finish the algorithm.
		void finish();
		//Convex Hull Algorithms
		static void convexHullGW(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f> points);
		static void convexHullGraham(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f> points);
		static void quickSortAngle(std::vector<Vector2f>& points, std::vector<float>& dots, size_t start, size_t end);
		static size_t partition(std::vector<Vector2f>& points, std::vector<float>& dots, size_t start, size_t end);
		static void convexHullQuickHull(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f> points);
		static std::vector<Vector2f> quickHullHelper(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f>& points, Vector2f left, Vector2f right);
		
		//Triangulation Algorithms
		//Ear Clipping
		static void triangulateEarClipping(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f> polygon);
		static bool diagonalCrossPolygon(std::vector<Vector2f>& polygon, Vector2f a, Vector2f b);
		static bool isEar(std::vector<Vector2f>& polygon, int idx, int prev, int next);

		//Sweep
		static void triangulateSweep(AlgorithmVisualizer* pVisualizer, std::unordered_map<Vector2f, std::vector<Vector2f>> edges);
		static void triangulateMonotoneMountain(AlgorithmVisualizer* pVisualizer, DCEL::Face* face);


		//Voronoi
		static void voronoiFortune(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f> points);
		static void arcsToBenzier(AlgorithmVisualizer* pVisualizer, BeachLineStatus& beachLine);
		static void drawVoronoiStatus(AlgorithmVisualizer* pVisualizer, BeachLineStatus& beachLine, std::vector<D2D::Line*>& lines, std::vector<VoronoiDiagram::HalfEdge*>& halfEdgePtrs);
	

	private:
		//Pointer to the application
		App* pApp = nullptr;
		//Flag that indicates whether or not to perform an algorithm step by step.
		bool mVisualize = true;
		//Flag that indicates whether or not to the algorithm visualizer should be paused.
		bool mShouldPause = false;
		//Mutex used to protect mShouldPause
		std::mutex mShouldPauseMutex;
		//Mutex used to protect mState
		std::mutex mStateMutex;
		//Mutex used to protect mSpeed
		std::mutex mSpeedMutex;
		//Mutex used to protect geometry(mLines, mPoints, mArcs)
		std::mutex mDrawMutex;
		//State of the algorithm visualizer.
		State mState = State::IDLE;
		//Pointer to the thread that this algorithm will run on.
		std::thread* mThread = nullptr;
		//Speed of the algorithm.
		float mSpeed = 5;
		//Stores the lines that are generated from the algorithm visualizer.
		std::vector<D2D::Line*> mLines;
		//Stores the points that are generated from the algorithm visualizer.
		std::vector<D2D::Point*> mPoints;
		//Stores the arcs that are generated from the algorithm visualizer.
		std::vector<D2D::QuadBezierCurve*> mArcs;
		//Result of the algorithm
		std::vector<Edge> mResult;
};

