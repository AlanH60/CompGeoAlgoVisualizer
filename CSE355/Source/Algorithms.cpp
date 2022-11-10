#include "PCH.h"
#include "Algorithms.h"
#include "App.h"
#include "Direct2D/Color.h"
#include "Direct2D/Drawable/Point.h"
#include "Direct2D/Drawable/Line.h"

AlgorithmVisualizer::AlgorithmVisualizer()
{
	mThread = new std::thread();
}

AlgorithmVisualizer::~AlgorithmVisualizer()
{
	clear();
	if (mThread->joinable())
		mThread->join();
	delete mThread;
}

bool AlgorithmVisualizer::isIdle()
{
	return mState == IDLE;
}


bool AlgorithmVisualizer::isRunning()
{
	return mState == RUNNING;
}

bool AlgorithmVisualizer::isFinished()
{
	return mState == FINISHED;
}

bool AlgorithmVisualizer::shouldVisualize()
{
	return mVisualize;
}

void AlgorithmVisualizer::setVisualization(bool visualize)
{
	mVisualize = visualize;
}

void AlgorithmVisualizer::setSpeed(float speed)
{
	mSpeed = speed;
}

std::vector<Drawable*> AlgorithmVisualizer::getDrawables()
{
	std::vector<Drawable*> drawables;
	for (Line* l : mLines)
		drawables.push_back(l);
	for (Point* p : mPoints)
		drawables.push_back(p);
	return drawables;
}

void AlgorithmVisualizer::computeConvexHull(std::vector<Vector2f>& points, ConvexHullAlgorithm algorithm)
{
	if (points.size() == 0)
		return;
	clear();
	switch (algorithm)
	{
		case GIFT_WRAPPING:
			if (mThread->joinable())
				mThread->join();
			mThread->swap(std::thread(convexHullGW, this, points));
	}
}

std::vector<std::pair<Vector2f, Vector2f>> AlgorithmVisualizer::getResult()
{
	if (mState == FINISHED)
	{
		mState = IDLE;
		std::vector<std::pair<Vector2f, Vector2f>> ret = std::move(mResult);
		mResult.clear();
		return ret;
	}
	return std::vector<std::pair<Vector2f, Vector2f>>();
}

void AlgorithmVisualizer::clear()
{
	for (Line* l : mLines)
		delete l;
	for (Point* p : mPoints)
		delete p;
	mLines.clear();
	mPoints.clear();
}

void AlgorithmVisualizer::wait()
{
	std::this_thread::sleep_for(std::chrono::milliseconds((long)(1000 / mSpeed)));
}

void AlgorithmVisualizer::convexHullGW(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f>& points)
{
	pVisualizer->mState = RUNNING;
	int minY = 0;
	std::vector<Vector2f> convexHull = std::vector<Vector2f>();
	std::vector<bool> usedPoints = std::vector<bool>(points.size()); //True if point is used in convex hull(Wanted to use bitset, but size of points is unknown at compile time)
	convexHull.reserve(points.size());

	Point* pCurrPoint = new Point({ 0, 0 }, Color{ 1, 1, 0, 1 });
	pVisualizer->mPoints.push_back(pCurrPoint);
	Point* pLowest = new Point({ 0, 0 }, Color{0, 1, 0, 1});
	pVisualizer->mPoints.push_back(pLowest);
	//Find point with smallest y value - O(n)
	for (int i = 0; i < points.size(); i++)
	{
		pCurrPoint->setPos(points[i]);
		int compareResult = compareY(points[minY], points[i]);
		if (compareResult > 0)
			minY = i;
		else if (compareResult == 0) //If same y, find leftmost point (smallest x value)
			minY = (points[minY].x > points[i].x) ? i : minY;
		pLowest->setPos(points[minY]);
		pVisualizer->wait();
	}
	convexHull.push_back(points[minY]);
	int nextPoint = 0;
	//Find points on convex hull by "wrapping around" - O(nh)
	for (int i = 0; i < convexHull.size(); i++)
	{
		Line* pNextLine = new Line(convexHull[i], {0, 0}, Color{0, 1, 0, 1});
		Point* pNextPoint = new Point({ 0, 0 }, Color{ 0, 1, 0, 1 });
		pVisualizer->mLines.push_back(pNextLine);
		pVisualizer->mPoints.push_back(pNextPoint);
		//Finds the rightmost point from the most recent vertex of convex hull. - O(n)
		for (int j = 0; j < points.size(); j++)
		{
			if (usedPoints[j] || (i == 0 && j == minY))
				continue;
			pCurrPoint->setPos(points[j]);
			if (!leftOf(convexHull[i], points[nextPoint], points[j]))
			{
				nextPoint = j;
				pNextPoint->setPos(points[j]);
				pNextLine->setPoints(convexHull[i], points[j]);
			}
			pVisualizer->wait();
		}
		convexHull.push_back(points[nextPoint]);
		usedPoints[nextPoint] = true;
		//Done if next point is the first point
		if (nextPoint == minY)
			break;
	}
	for (int i = 0; i < convexHull.size() - 1; i++)
		pVisualizer->mResult.push_back({ convexHull[i], convexHull[i + 1] });
	pVisualizer->mState = FINISHED;
}
