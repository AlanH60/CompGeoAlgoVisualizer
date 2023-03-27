#include "PCH.h"
#include "Algorithms.h"
#include "App.h"
#include "DataStructs/TriSweepLineStatus.h"
#include "Direct2D/Color.h"
#include "Direct2D/Drawable/Point.h"
#include "Direct2D/Drawable/Line.h"
#include "Direct2D/Drawable/QuadBezierCurve.h"
#include "DataStructs/BeachLineStatus.h"


using D2D::Drawable;
using D2D::Point;
using D2D::Line;
using D2D::QuadBezierCurve;

AlgorithmVisualizer::AlgorithmVisualizer(App* pApp)
	:
	pApp(pApp)
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
	return mState == State::IDLE;
}


bool AlgorithmVisualizer::isRunning()
{
	return mState == State::RUNNING;
}

bool AlgorithmVisualizer::isFinished()
{
	return mState == State::FINISHED;
}

bool AlgorithmVisualizer::isSleeping()
{
	return mState == State::SLEEPING;
}

bool AlgorithmVisualizer::shouldVisualize()
{
	return mVisualize;
}

bool AlgorithmVisualizer::shouldPause()
{
	return mShouldPause;
}

float AlgorithmVisualizer::getSpeed()
{
	return mSpeed;
}

void AlgorithmVisualizer::setVisualization(bool visualize)
{
	mVisualize = visualize;
}

void AlgorithmVisualizer::setShouldPause(bool shouldPause)
{
	mShouldPause = shouldPause;
}


void AlgorithmVisualizer::setSpeed(float speed)
{
	mSpeed = speed;
}

void AlgorithmVisualizer::addSpeed(float modifier)
{
	mSpeed += modifier;
	if (mSpeed < 1)
		mSpeed = 1;
	if (mSpeed > 10)
		mSpeed = 10;
}

std::vector<D2D::Line*>& AlgorithmVisualizer::getLines()
{
	return mLines;
}

std::vector<D2D::Point*>& AlgorithmVisualizer::getPoints()
{
	return mPoints;
}

std::vector<D2D::QuadBezierCurve*>& AlgorithmVisualizer::getArcs()
{
	return mArcs;
}


void AlgorithmVisualizer::computeConvexHull(std::vector<Vector2f>& points, ConvexHullAlgorithm algorithm)
{
	if (points.size() == 0)
		return;
	clear();
	if (mThread->joinable())
		mThread->join();
	switch (algorithm)
	{
		case ConvexHullAlgorithm::GIFT_WRAPPING:
			mThread->swap(std::thread(convexHullGW, this, points));
			break;
		case ConvexHullAlgorithm::GRAHAM_SCAN:
			mThread->swap(std::thread(convexHullGraham, this, points));
			break;
		case ConvexHullAlgorithm::QUICK_HULL:
			mThread->swap(std::thread(convexHullQuickHull, this, points));
			break;
	}
}

void AlgorithmVisualizer::computeTriangulation(std::vector<Vector2f>& polygon, std::unordered_map<Vector2f, std::vector<Vector2f>>& edges, TriangulationAlgorithm algorithm)
{
	if (polygon.size() <= 3)
		return;
	clear();
	if (mThread->joinable())
		mThread->join();
	switch (algorithm)
	{
		case TriangulationAlgorithm::EAR_CLIPPING:
			mThread->swap(std::thread(triangulateEarClipping, this, polygon));
			break;
		case TriangulationAlgorithm::SWEEP:
			mThread->swap(std::thread(triangulateSweep, this, edges));
			break;
	}
}

void AlgorithmVisualizer::computeVoronoiDiagram(std::vector<Vector2f>& points, VoronoiDiagramAlgorithm algorithm)
{
	clear();
	if (mThread->joinable())
		mThread->join();
	switch (algorithm)
	{
		case VoronoiDiagramAlgorithm::FORTUNE:
			mThread->swap(std::thread(voronoiFortune, this, points));
			break;
	}
}

std::vector<Edge> AlgorithmVisualizer::getResult()
{
	if (mState == State::FINISHED)
	{
		mState = State::IDLE;
		std::vector<Edge> ret = std::move(mResult);
		mResult.clear();
		return ret;
	}
	return std::vector<Edge>();
}

void AlgorithmVisualizer::clear()
{
	for (int i = 0; i < mLines.size(); i++)
		delete mLines[i];
	for (int i = 0; i < mPoints.size(); i++)
		delete mPoints[i];
	for (int i = 0; i < mArcs.size(); i++)
		delete mArcs[i];
	mLines.clear();
	mPoints.clear();
	mArcs.clear();
}

void AlgorithmVisualizer::wait()
{
	wait(1);
}

void AlgorithmVisualizer::wait(float multiplier)
{
	mState = State::SLEEPING;
	//std::cout << "[Visualizer] --- Sleeping" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds((long)(multiplier * 1000 / mSpeed)));
	while (mShouldPause) {} //If it is pausing, continue sleeping.
	while (pApp->isDrawingAlgorithmVisualize()) {} //Wait until app is done drawing before continuing to run.
	mState = State::RUNNING;
	//std::cout << "[Visualizer] --- Running" << std::endl;
}

void AlgorithmVisualizer::finish()
{
	this->mState = State::SLEEPING;
	while (pApp->isDrawingAlgorithmVisualize()) {};
	this->mState = State::FINISHED;
}

void AlgorithmVisualizer::convexHullGW(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f>& points)
{
	pVisualizer->mState = State::RUNNING;

	int minY = 0;
	std::vector<Vector2f> convexHull = std::vector<Vector2f>();
	std::vector<bool> usedPoints = std::vector<bool>(points.size()); //True if point is used in convex hull(Wanted to use bitset, but size of points is unknown at compile time)
	convexHull.reserve(points.size());

	//Current lines and points that the algorithm is evaluating
	Line* pCurrLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });
	Point* pCurrPoint = new Point({ 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });
	//Lowest point and line.  Line is a horizontal line that all other points should be above.
	Line* pLowestLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 1, 0, 1 });
	Point* pLowestPoint = new Point({ 0, 0 }, Color{0, 1, 0, 1});
	pVisualizer->mLines.push_back(pCurrLine);
	pVisualizer->mPoints.push_back(pCurrPoint);
	pVisualizer->mLines.push_back(pLowestLine);
	pVisualizer->mPoints.push_back(pLowestPoint);

	//Find point with smallest y value - O(n)
	for (int i = 0; i < points.size(); i++)
	{
		pCurrPoint->setPos(points[i]);
		pCurrLine->setPoints({ 0, points[i].y }, { 10000, points[i].y });
		
		//Compare the y values,  if points[minY] is below points[i], compareResult = -1, otherwise if it is above, compareResult = 1.
		//compareResult = 0 if they are equal.
		int compareResult = compareY(points[minY], points[i]);
		if (compareResult > 0)
			minY = i;
		else if (compareResult == 0) //If same y, find leftmost point (smallest x value)
			minY = (points[minY].x > points[i].x) ? i : minY;

		//Update the lowest point and line.
		pLowestPoint->setPos(points[minY]);
		pLowestLine->setPoints({ 0, points[minY].y }, { 10000, points[minY].y });
		pVisualizer->wait();
	}
	//Lowest line no longer needed.
	pLowestLine->setVisibility(false);

	convexHull.push_back(points[minY]);
	int nextPoint = 0;
	//Find points on convex hull by "wrapping around" - O(nh)
	for (int i = 0; i < convexHull.size(); i++)
	{
		//Current right-most edge
		Line* pNextLine = new Line(convexHull[i], {0, 0}, Color{0, 1, 0, 1});
		//Current point of the right-most edge
		Point* pNextPoint = new Point({ 0, 0 }, Color{ 0, 1, 0, 1 });
		pVisualizer->mLines.push_back(pNextLine);
		pVisualizer->mPoints.push_back(pNextPoint);

		//Finds the rightmost point from the most recent vertex of convex hull. - O(n)
		for (int j = 0; j < points.size(); j++)
		{
			if (usedPoints[j] || (i == 0 && j == minY))
				continue;
			pCurrLine->setPoints(convexHull[i], points[j]);
			pCurrPoint->setPos(points[j]);
			pVisualizer->wait(0.5f);
			//If we find a point that is to the right of our current right-most edge, 
			//we set the edge from that point to the most recent convex hull vertex as the new right-most edge.
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
	pVisualizer->finish();
}

//Graham Scan- O(nlogn) - we choose an extreme point, pivot, that is guaranteed to be on the convex hull.
//Then we sort the points by angle from the pivot point, using dot product. 
//We can then use left tests and "wrap" the points in sorted counter-clockwise order.
void AlgorithmVisualizer::convexHullGraham(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f>& points)
{
	pVisualizer->mState = State::RUNNING;
	//Current lines and points that the algorithm is evaluating
	Line* pCurrLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });
	Point* pCurrPoint = new Point({ 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });

	//Lowest point and line.  Line is a horizontal line that all other points should be above.
	Line* pLowestLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 1, 0, 1 });
	Point* pLowestPoint = new Point({ 0, 0 }, Color{ 0, 1, 0, 1 });
	pVisualizer->mLines.push_back(pCurrLine);
	pVisualizer->mPoints.push_back(pCurrPoint);
	pVisualizer->mLines.push_back(pLowestLine);
	pVisualizer->mPoints.push_back(pLowestPoint);

	//Find the pivot point(Point with lowest y value, higher x value is used to break ties).  Rightmost of the bottommost
	int pivot = 0;
	for (int i = 0; i < points.size(); i++)
	{
		pCurrPoint->setPos(points[i]);
		pCurrLine->setPoints({ 0, points[i].y }, { 10000, points[i].y });

		if (points[i].y < points[pivot].y)
			pivot = i;
		else if (points[i].y == points[pivot].y)
			pivot = (points[i].x > points[pivot].x) ? i : pivot;

		//Update the lowest point and line.
		pLowestPoint->setPos(points[pivot]);
		pLowestLine->setPoints({ 0, points[pivot].y }, { 10000, points[pivot].y });
		pVisualizer->wait();
	}
	//Lowest line no longer needed.
	pLowestLine->setVisibility(false);

	//Get the dot products between the vector horizontal from pivot and the vector from pivot to any given point.
	//This dot product will be used to sort the points (1 -> -1).  
	std::vector<float> dots = std::vector<float>();
	for (int i = 0; i < points.size(); i++)
	{
		if (i == pivot) //Exclude the pivot point
			continue;
		dots.push_back(dot({ 1, 0 }, normalize(Vector2f{ points[i] - points[pivot] })));
	}
	std::vector<Vector2f> sortedPoints = points;
	sortedPoints.erase((pivot == 0) ? sortedPoints.begin() : std::next(sortedPoints.begin(), pivot));
	//Quick sort to sort vertices by dot product. Note: Average: O(nlogn), Worse: O(n^2), so this graham scan has worse case O(n^2).
	quickSortAngle(sortedPoints, dots, 0, sortedPoints.size());

	//"Stack", but I use an vector.
	std::vector<Vector2f> hull = std::vector<Vector2f>();
	hull.push_back(points[pivot]);
	hull.push_back(sortedPoints[0]);
	pVisualizer->mLines.push_back(new Line(points[pivot], sortedPoints[0], Color{ 0, 1, 0, 1 }));
	pVisualizer->mPoints.push_back(new Point(points[pivot], Color{ 0, 1, 0, 1 }));
	pVisualizer->mPoints.push_back(new Point(sortedPoints[0], Color{ 0, 1, 0, 1 }));

	for (int i = 1; i < sortedPoints.size(); i++)
	{
		pCurrPoint->setPos(sortedPoints[i]);
		pCurrLine->setPoints(hull[hull.size() - 1], sortedPoints[i]);
		pVisualizer->wait();
		if (!leftOf(hull[hull.size() - 2], hull[hull.size() - 1], sortedPoints[i]))
		{
			//Remove from the end of the hull "stack" until the point satisfies the left test.
			do
			{
				hull.erase(--hull.end());
				pCurrLine->setPoints(hull[hull.size() - 1], sortedPoints[i]);
				Point* p = *--pVisualizer->mPoints.end();
				Line* l = *--pVisualizer->mLines.end();
				pVisualizer->mLines.erase(--pVisualizer->mLines.end());
				pVisualizer->mPoints.erase(--pVisualizer->mPoints.end());
				pVisualizer->wait(); //Wait to delete the lines bc main thread might be drawing.
				delete p;
				delete l;
			} while (hull.size() > 2 && !leftOf(hull[hull.size() - 2], hull[hull.size() - 1], sortedPoints[i]));
		}		
		pVisualizer->mLines.push_back(new Line(hull[hull.size() - 1], sortedPoints[i], Color{ 0, 1, 0, 1 }));
		pVisualizer->mPoints.push_back(new Point(sortedPoints[i], Color{ 0, 1, 0, 1 }));
		hull.push_back(sortedPoints[i]);
	}
	hull.push_back(points[pivot]);
	for (int i = 0; i < hull.size() - 1; i++)
		pVisualizer->mResult.push_back({ hull[i], hull[i + 1] });
	pVisualizer->finish();
}

void AlgorithmVisualizer::quickSortAngle(std::vector<Vector2f>& points, std::vector<float>& dots, size_t start, size_t end)
{
	if (end - start <= 1)
		return;

	size_t p = partition(points, dots, start, end);
	quickSortAngle(points, dots, start, p);
	quickSortAngle(points, dots, p + 1, end);
}

size_t AlgorithmVisualizer::partition(std::vector<Vector2f>& points, std::vector<float>& dots, size_t start, size_t end)
{
	size_t i = start;
	for (size_t j = start; j < end; j++)
	{
		//Sort dot products from largest to smallest.  Larger dot product means that it comes first.  
		//Theta only goes from 0-pi because all the angles are relative to the lowest y point. 
		if (dots[j] >= dots[end - 1])
		{
			Vector2f temp = points[i];
			points[i] = points[j];
			points[j] = temp;
			float tempf = dots[i];
			dots[i] = dots[j];
			dots[j] = tempf;
			i++;
		}
	}
	return --i;
}

//Recursive function for quickHull
std::vector<Vector2f> AlgorithmVisualizer::quickHullHelper(AlgorithmVisualizer* pVisualizer, 
	std::vector<Vector2f>& points, Vector2f left, Vector2f right)
{
	//Base case, no points remaining.
	if (points.empty())
		return std::vector<Vector2f>();
	Point* pTopPoint = new Point({ 0, 0 }, Color{ 0, 1, 0, 1 });
	Line* pTopLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 1, 0, 1 });
	Point* pCurrPoint = new Point({ 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });
	Line* pCurrLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });
	pVisualizer->mLines.push_back(pTopLine);
	pVisualizer->mLines.push_back(pCurrLine);
	pVisualizer->mPoints.push_back(pTopPoint);
	pVisualizer->mPoints.push_back(pCurrPoint);

	Vector2f top;
	Vector2f leftRightUnit = normalize(Vector2f(right - left));
	float maxDist = 0;
	for (Vector2f& p : points)
	{
		Vector2f leftToP = p - left;
		//Projects the point p onto line from left to right.
		Vector2f projection = leftRightUnit * dot(leftToP, leftRightUnit);
		//From this projection, we take the y component from the vector from projection to point.
		float dist = abs((leftToP - projection).y);

		pCurrLine->setPoints(Vector2f(projection + left), p);
		pCurrPoint->setPos(p);
		pVisualizer->wait();

		//Maximize this y component to find the next vertex of the hull.
		if (dist > maxDist)
		{
			maxDist = dist;
			top = p;
			pTopLine->setPoints(Vector2f(projection + left), p);
			pTopPoint->setPos(p);
		}
		pVisualizer->wait();
	}
	pCurrPoint->setVisibility(false);
	pCurrLine->setColor({ 0, 1, 0, 1 });
	pTopLine->setPoints(left, top);
	pCurrLine->setPoints(top, right);
	//Once found we find the next vertex of the hull, top, we divide the points that are left of vector(left->c) and vector(c->right).
	std::vector<Vector2f> leftPoints;
	std::vector<Vector2f> rightPoints;
	for (Vector2f& p : points)
	{
		if (leftOf(left, top, p))
			leftPoints.push_back(p);
		else if (leftOf(top, right, p))
			rightPoints.push_back(p);
	}
	//Recursively find the hull of the points between left and top, and top and right.
	std::vector<Vector2f> leftHull = quickHullHelper(pVisualizer, leftPoints, left, top);
	std::vector<Vector2f> rightHull = quickHullHelper(pVisualizer, rightPoints, top, right);

	if (leftHull.empty())
		pVisualizer->mLines.push_back(new Line(top, left, Color{ 0, 1, 0, 1 }));
	if (rightHull.empty())
		pVisualizer->mLines.push_back(new Line(top, right, Color{ 0, 1, 0, 1 }));

	//Once we find the left and right hulls.  We combine them accordingly.
	std::vector<Vector2f> topHull;
	for (Vector2f& p : rightHull)
		topHull.push_back(p);
	topHull.push_back(top);
	for (Vector2f& p : leftHull)
		topHull.push_back(p);
	return topHull;
}

//Quick Hull- Average - O(nlogn), Worse - O(n^2)
void AlgorithmVisualizer::convexHullQuickHull(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f>& points)
{
	pVisualizer->mState = State::RUNNING;
	Vector2f left = points[0];
	Vector2f right = points[0];
	Point* pCurrPoint = new Point({ 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });
	Line* pCurrLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 0.5f, 0.5f, 1 });
	Point* pRightPoint = new Point({ 0, 0 }, Color{ 0, 1, 0, 1 });
	Line* pRightLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 1, 0, 1 });
	Point* pLeftPoint = new Point({ 0, 0 }, Color{ 0, 1, 0, 1 });
	Line* pLeftLine = new Line({ 0, 0 }, { 0, 0 }, Color{ 0, 1, 0, 1 });

	pVisualizer->mLines.push_back(pCurrLine);
	pVisualizer->mLines.push_back(pRightLine);
	pVisualizer->mLines.push_back(pLeftLine);

	pVisualizer->mPoints.push_back(pCurrPoint);
	pVisualizer->mPoints.push_back(pRightPoint);
	pVisualizer->mPoints.push_back(pLeftPoint);

	//Find the top-leftmost point and the top-rightmost point.
	for (Vector2f& p : points)
	{
		pCurrPoint->setPos(p);
		pCurrLine->setPoints({ p.x, 0 }, { p.x, 10000 });

		if (p.x < left.x)
			left = p;
		else if (p.x == left.x)
			left = (p.y > left.y) ? p : left;
		if (p.x > right.x)
			right = p;
		else if (p.x == right.x)
			right = (p.y > right.y) ? p : right;

		pLeftPoint->setPos(left);
		pRightPoint->setPos(right);
		pLeftLine->setPoints({ left.x, 0 }, { left.x, 10000 });
		pRightLine->setPoints({ right.x, 0 }, { right.x, 10000 });

		pVisualizer->wait();
	}
	pCurrPoint->setVisibility(false);
	pLeftLine->setVisibility(false);
	pCurrLine->setVisibility(false);
	pRightLine->setPoints(left, right);

	//Divide the points into a set of points above and below the line from the left to right points.
	std::vector<Vector2f> topPoints;
	std::vector<Vector2f> bottomPoints;
	for (Vector2f& p : points)
	{
		if (leftOf(left, right, p))
			topPoints.push_back(p);
		else if (p != left && p != right)
			bottomPoints.push_back(p);
	}
	//Find the top convex hull and bottom convex hull.
	std::vector<Vector2f> topHull = quickHullHelper(pVisualizer, topPoints, left, right);
	std::vector<Vector2f> bottomHull = quickHullHelper(pVisualizer, bottomPoints, right, left);

	//Combine the results of the top hull and bottom hull accordingly.
	std::vector<Vector2f> hull;
	hull.push_back(right);
	for (Vector2f& v : topHull)
		hull.push_back(v);
	hull.push_back(left);
	for (Vector2f& v : bottomHull)
		hull.push_back(v);
	hull.push_back(right);

	for (int i = 0; i < hull.size() - 1; i++)
		pVisualizer->mResult.push_back({ hull[i], hull[i + 1] });

	pVisualizer->finish();
}

//Ear-Clipping triangulation algorithm.  Cut off ears and update ear status of adjacent vertices.  Loop around the polygon until triangulation is done.
void AlgorithmVisualizer::triangulateEarClipping(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f>& polygon)
{
	pVisualizer->mState = State::RUNNING;
	//Vector of VertexStatus of each vertex of the polygon
	std::vector<VertexStatus> vertexStatus = std::vector<VertexStatus>(polygon.size());
	//Intialize the VertexStatus of each vertex - O(n^2)
	for (int i = 0; i < polygon.size(); i++)
		vertexStatus[i] = { (i == 0) ? (polygon.size() - 1) : i - 1, (i + 1) % polygon.size(), isEar(polygon, i, (i == 0) ? (polygon.size() - 1) : i - 1, (i + 1) % polygon.size()), false };

	Point* pCurrPoint = new Point({ 0, 0 }, { 0, 0.5f, 0.5f, 1 });
	Line* pCurrLine = new Line({ 0, 0 }, { 0, 0 }, { 0, 0.5f, 0.5f, 1 });

	pVisualizer->mPoints.push_back(pCurrPoint);
	pVisualizer->mLines.push_back(pCurrLine);

	int i = 0;
	while (pVisualizer->mResult.size() != (polygon.size() - 3)) //There are always n - 3 non-crossing diagonals.
	{
		i %= polygon.size();
		//If the vertex is an ear and is not already clipped, clip it.  Then update its prev and next vertices' ear status and their next and prev vertex respectively.
		if (!vertexStatus[i].isClipped)
		{
			pCurrPoint->setPos(polygon[i]);
			pCurrLine->setPoints(polygon[vertexStatus[i].prev], polygon[vertexStatus[i].next]);
			if (vertexStatus[i].isEar)
			{
				pCurrLine->setColor({ 0, 0.5f, 0.5f, 1 });
				pVisualizer->wait(2);
				VertexStatus& v = vertexStatus[i];
				pVisualizer->mResult.push_back({ polygon[v.prev], polygon[v.next] });
				pVisualizer->mLines.push_back(new Line(polygon[v.prev], polygon[v.next], {0, 1, 0, 1}));
				pVisualizer->mPoints.push_back(new Point(polygon[i], {0, 1, 0, 1}));

				vertexStatus[v.prev].next = v.next;
				vertexStatus[v.next].prev = v.prev;
				vertexStatus[v.prev].isEar = isEar(polygon, v.prev, vertexStatus[v.prev].prev, v.next);
				vertexStatus[v.next].isEar = isEar(polygon, v.next, v.prev, vertexStatus[v.next].next);
				vertexStatus[i].isClipped = true;
			}
			else
				pCurrLine->setColor({ 1, 0, 0, 1 });

			pVisualizer->wait(2);
		}
		i++;
	}

	pVisualizer->finish();
}

bool AlgorithmVisualizer::diagonalCrossPolygon(std::vector<Vector2f>& polygon, Vector2f a, Vector2f b)
{
	for (int i = 1; i < polygon.size(); i++)
	{
		if (intersectProp(a, b, polygon[i - 1], polygon[i]))
		{
			return false;
		}
	}
	return true;
}

bool AlgorithmVisualizer::isEar(std::vector<Vector2f>& polygon, int idx, int prev, int next)
{
	if (!isConvex(polygon[prev], polygon[idx], polygon[next]))
		return false;
	return diagonalCrossPolygon(polygon, polygon[prev], polygon[next]);
}

void AlgorithmVisualizer::triangulateSweep(AlgorithmVisualizer* pVisualizer, std::unordered_map<Vector2f, std::vector<Vector2f>>& edges)
{
	pVisualizer->mState = State::RUNNING;
	TriSweepLineStatus sweepLineStatus;
	//Event queue will sort the points in order of greatest y.
	std::priority_queue<Vector2f, std::vector<Vector2f>, VertexCompareY> eventQueue;
	//Push all points into events queue.
	for (auto& pair : edges)
		eventQueue.push(pair.first);
	//Map from edge to its bottom most left and right trapezoids(the defining vertices of the trapezoids).
	std::unordered_map<Edge, std::pair<Vector2f, Vector2f>> trapezoids;
	std::vector<Edge> edgesToRemove;
	//**************VISUALIZER*********************
	Line* line = new Line({ 0, 0 }, { 10000, 0 }, { 0, 0, 0, 1.0f });
	pVisualizer->mLines.push_back(line);
	//*****************************************

	while (!eventQueue.empty())
	{
		Vector2f vertex = eventQueue.top();
		sweepLineStatus.setY(vertex.y);
		eventQueue.pop();
		edgesToRemove.clear();

		//Update sweep line
		line->setPoints({ 0, vertex.y }, { 10000, vertex.y });
		pVisualizer->wait();

		//Remove all edges above vertex.
		for (Vector2f& v : edges[vertex])
		{
			if (VertexCompareY()(vertex, v))
			{
				Edge e = { v, vertex };
				if (trapezoids.find(e) != trapezoids.end())
					edgesToRemove.push_back(e); //Mark edges for removal, we still need to keep the info about their trapezoids for now.
				sweepLineStatus.remove(e); //Remove the edges from sweepline. This will allow us to find neighbors of the new vertex.
			}
		}
		//Find the edges directly to the left and right of the vertex.
		Edge leftEdge;
		Edge rightEdge;
		bool leftFound = false;
		bool rightFound = false;
		std::pair<bool, bool> res = sweepLineStatus.findNeighbors({ vertex, { vertex.x + 1, vertex.y + 1 } }, &leftEdge, &rightEdge);
		if (res.first)
		{
			//If left edge isn't the end at infinity, 
			if (trapezoids[leftEdge].second != Vector2f())
			{
				//Connect the left edge's right trapezoid vertex with this vertex if an edge doesn't already exist.
				if (trapezoids.find({ trapezoids[leftEdge].second, vertex }) == trapezoids.end())
				{
					pVisualizer->mResult.push_back({ trapezoids[leftEdge].second, vertex });
					pVisualizer->mLines.push_back(new Line{ trapezoids[leftEdge].second, vertex, {0, 0, 0, 1.0f} });
					edges[vertex].push_back(trapezoids[leftEdge].second);
					edges[trapezoids[leftEdge].second].push_back(vertex);
				}
				leftFound = true;
				pVisualizer->wait();
			}
		}
		if (res.second)
		{
			if (trapezoids[rightEdge].first != Vector2f())
			{
				//Connect the right edge's left trapezoid vertex with this vertex if an edge doesn't already exist.
				if (trapezoids.find({ trapezoids[rightEdge].first, vertex }) == trapezoids.end() && !(leftFound && trapezoids[rightEdge].first == trapezoids[leftEdge].second))
				{
					pVisualizer->mResult.push_back({ trapezoids[rightEdge].first, vertex });
					pVisualizer->mLines.push_back(new Line{ trapezoids[rightEdge].first, vertex, {0, 0, 0, 1.0f} });
					edges[vertex].push_back(trapezoids[rightEdge].first);
					edges[trapezoids[rightEdge].first].push_back(vertex);
				}
				rightFound = true;
				pVisualizer->wait();
			}
		}
		//Update trapezoids map after checking both edges(In case right edge's trapezoid is defined by the same vertex as left edge)
		if (leftFound)
			trapezoids[leftEdge].second = vertex;
		if (rightFound)
			trapezoids[rightEdge].first = vertex;
		//Remove edges from trapezoids
		for (Edge& e : edgesToRemove)
			trapezoids.erase(e);

		//Add new edges below vertex into SLS.
		for (int i = 0; i < edges[vertex].size(); i++)
		{
			if (VertexCompareY()(edges[vertex][i], vertex))
			{
				Edge e = { vertex, edges[vertex][i] };
				sweepLineStatus.insert(e);
				if (i % 2 == 0) //Vertex is predecessor
					trapezoids[e] = { vertex, Vector2f() };
				else
					trapezoids[e] = { Vector2f() , vertex };
			}
		}

	}

	//Create a doublely connected edge list to store our monotone mountains.
	DCEL dcel(edges);
	std::unordered_map<Edge, DCEL::HalfEdge*>& halfEdges = dcel.getHalfEdges();
	std::unordered_map<Vector2f, DCEL::Vertex*>& vertices = dcel.getVertices();
	std::vector<DCEL::Face*>& faces = dcel.getFaces();
	//Triangulate each monotone mountain.
	for (DCEL::Face* face : faces)
		triangulateMonotoneMountain(pVisualizer, face);

	pVisualizer->finish();
}

void AlgorithmVisualizer::triangulateMonotoneMountain(AlgorithmVisualizer* pVisualizer, DCEL::Face* face)
{
	DCEL::HalfEdge* bottom = face->rep;
	DCEL::HalfEdge* top = face->rep;
	DCEL::HalfEdge* e = face->rep->next;
	//Find the top and bottom of the monotone mountain.
	while (e != face->rep)
	{
		if (e->tail->y < bottom->tail->y || (e->tail->y == bottom->tail->y && e->tail->x < bottom->tail->x))
			bottom = e;
		if (e->tail->y > top->tail->y || (e->tail->y == top->tail->y && e->tail->x < top->tail->x))
			top = e;
		e = e->next;
	}
	//If CW(Face at infinity)
	if (!leftOf(bottom->tail->v, bottom->next->tail->v, bottom->prev->tail->v)) 
		return;
	std::vector<Vector2f> stack;
	e = bottom;
	DCEL::HalfEdge* start = bottom;
	//If move the start point such that we start at either top or bottom and end at bottom or top, respectively.
	if (e->next == top)
	{
		e = e->next;
		start = top;
	}
	stack.push_back(start->tail->v);
	Vector2f curr;
	Vector2f next;
	//At each iteration, we check if e->next->tail is convex.
	while (e->next->next != start && !(stack.size() == 1 && e->next->next->next == start))
	{
		curr = e->next->tail->v;
		stack.push_back(curr); //Push current vertex onto stack
		next = e->next->next->tail->v;
		//We continue popping from the stack if the top two vertices and e->next->next->tail(vertex after curr) form a convex angle.
		while (stack.size() >= 2 && isConvex(*----stack.end(), *--stack.end(), next))
		{
			stack.erase(--stack.end());
			pVisualizer->mLines.push_back(new Line{ *--stack.end(), next, {0, 1, 0, 1} });
			pVisualizer->mResult.push_back({ *--stack.end(), next });
			pVisualizer->wait();
		}
		e = e->next;
	}

}

void AlgorithmVisualizer::voronoiFortune(AlgorithmVisualizer* pVisualizer, std::vector<Vector2f>& points)
{
	pVisualizer->mState = State::RUNNING;
	BeachLineStatus beachLine(100000);
	//Event queue will sort the points in order of greatest y.
	std::priority_queue<BeachLineStatus::Event*, std::vector<BeachLineStatus::Event*>, BeachLineStatus::EventCompare> eventQueue;
	//Push all points into events queue.
	for (auto& p : points)
		eventQueue.push(new BeachLineStatus::Event(p));
	//Sweepline
	Line* sweepLine = new Line({ 0, 0 }, { 2000, 0 });
	pVisualizer->mLines.push_back(sweepLine);

	//Map from half edges of the voronoi diagram to lines to draw and update.
	std::unordered_map<VoronoiDiagram::HalfEdge*, Line*> edgesToLines;
	//Pointers to half edges, used to query into edgesToLines map and add lines when new half edges appear.
	std::vector<VoronoiDiagram::HalfEdge*>& halfEdgePtrs = beachLine.getHalfEdgePtrs();

	while (!eventQueue.empty())
	{
		BeachLineStatus::Event* e = eventQueue.top();
		eventQueue.pop();
		//If an event is no longer valid, delete it and continue
		if (!e->isValid) { delete e; continue; }
		//Move the beachline directrix to the event point
		beachLine.setDirectrix(e->point.y);

		//Visually move the sweepline
		sweepLine->setPoints({ 0, e->point.y }, { 2000, e->point.y });
		//Draw the current status of the algorithm(All arcs and edges)
		drawVoronoiStatus(pVisualizer, beachLine, edgesToLines, halfEdgePtrs, e->point.y);

		//Flag used to determine whether a circle event was successfully(Arc removed).
		bool success = true;
		if (!e->isCircle)
			BeachLineStatus::Arc* arc = beachLine.addArc(e->point, eventQueue);
		else
			success = beachLine.remove(e->arc, eventQueue);
		if (success) //If an arc was removed, we redraw the scene.
			drawVoronoiStatus(pVisualizer, beachLine, edgesToLines, halfEdgePtrs, e->point.y);

		delete e;
	}
	//Resolve the edges of the arcs that go off to infinity.
	beachLine.resolveLastArcs();
	//Move the edges from the voronoi diagram to the result of the visualizer.
	pVisualizer->mResult = std::move(beachLine.getEdges());
	pVisualizer->finish();
}

void AlgorithmVisualizer::arcsToBenzier(AlgorithmVisualizer* pVisualizer, BeachLineStatus::Arc* leftmostArc, float directrix)
{
	for (QuadBezierCurve* a : pVisualizer->mArcs)
		delete a;
	pVisualizer->mArcs.clear();
	Vector2f start = {};
	Vector2f control = {};
	Vector2f end = {};
	float p = 0;
	float a = 0;
	float b = 0;
	float c = 0;
	BeachLineStatus::Arc* arc = leftmostArc;
	while (arc)
	{
		if (arc->face->site.y == directrix)
		{
			start = arc->face->site;
			control = arc->face->site;
			if (!arc->prev)
				end = { arc->face->site.x, 10000 };
			pVisualizer->mArcs.push_back(new QuadBezierCurve(start, control, end));
			arc = arc->next;
			continue;
		}
		p = (arc->face->site.y - directrix) / 2;
		a = 1 / (4 * p);
		b = -arc->face->site.x / (2 * p);
		c = (arc->face->site.x * arc->face->site.x) / (4 * p) + arc->face->site.y - p;

		if (arc->prev)
			start = end;
		else
			start = { 0, c };
		if (arc->next)
		{
			if (arc->next->face->site.y == directrix)
			{
				float x = arc->next->face->site.x;
				end = { x, x * x * a + x * b + c };
			}
			else
			{
				Vector2f intersection = getArcIntersection(arc, arc->next, directrix);
				if (isnan(intersection.y))
					end = { intersection.x, intersection.x * intersection.x * a + intersection.x * b + c };
				else
				{
					float right;
					if (arc->face->site.y < arc->next->face->site.y)
						right = std::max(intersection.x, intersection.y);
					else
						right = std::min(intersection.x, intersection.y);
					end = { right, right * right * a + right * b + c };
				}
			}
		}
		else
			end = { 2000, 2000 * 2000 * a + 2000 * b + c };
		control = { (start.x + end.x) / 2, start.y + (2 * a * start.x + b) * (end.x - start.x) / 2 };
		pVisualizer->mArcs.push_back(new QuadBezierCurve(start, control, end));

		arc = arc->next;
	}
}

Vector2f AlgorithmVisualizer::getArcIntersection(BeachLineStatus::Arc* arc1, BeachLineStatus::Arc* arc2, float directrix)
{
	Vector2f site1 = arc1->face->site;
	Vector2f site2 = arc2->face->site;
	if (site1.y == site2.y) //If the two sites have the same y, their intersection is the middle of their xs'.
		return { (site1.x + site2.x) / 2, std::numeric_limits<float>::quiet_NaN() };

	if (directrix == site1.y) //If the sites have different y but one site is at the directrix.
		return { site1.x, std::numeric_limits<float>::quiet_NaN() };
	if (directrix == site2.y)
		return { site2.x, std::numeric_limits<float>::quiet_NaN() };

	float a1 = 1 / (2 * (site1.y - directrix));
	float a2 = 1 / (2 * (site2.y - directrix));

	float a = (a1 - a2);
	float b = -2 * (a1 * site1.x - a2 * site2.x);
	float c = (site1.x * site1.x * a1 - site2.x * site2.x * a2 + (site1.y - site2.y) / 2);

	if (a == 0) //Linear(1 intersection)
		return { -c / b, std::numeric_limits<float>::quiet_NaN() };

	float sqrtTerm = b * b - 4 * a * c;
	if (sqrtTerm < 0)
		return Vector2f();
	float pmTerm = sqrt(sqrtTerm);
	float x1 = (-b + pmTerm) / (2 * a);
	float x2 = (-b - pmTerm) / (2 * a);

	return { x1, x2 };
}

void AlgorithmVisualizer::drawVoronoiStatus(AlgorithmVisualizer* pVisualizer, BeachLineStatus& beachLine, std::unordered_map<VoronoiDiagram::HalfEdge*, Line*>& edgesToLines, std::vector<VoronoiDiagram::HalfEdge*>& halfEdgePtrs, float directrix)
{
	beachLine.updateEdges();
	for (int i = 0 ; i < halfEdgePtrs.size(); i ++)
	{
		VoronoiDiagram::HalfEdge* pHalfEdge = halfEdgePtrs[i];
		if (edgesToLines.find(pHalfEdge) == edgesToLines.end())
		{
			Line* l = new Line(pHalfEdge->v1, pHalfEdge->v2);
			pVisualizer->mLines.push_back(l);
			edgesToLines[pHalfEdge] = l;
		}
		edgesToLines[pHalfEdge]->setPoints(pHalfEdge->v1, pHalfEdge->v2);
	}
	arcsToBenzier(pVisualizer, beachLine.getLeftmostArc(), directrix);
	pVisualizer->wait(0.01f);
}
