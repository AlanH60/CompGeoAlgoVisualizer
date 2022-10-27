#include "PCH.h"
#include "Algorithms.h"
#include "App.h"
#include "Direct2D/Color.h"

std::vector<Vector2f> AlgorithmVisualizer::convexHullGW(std::vector<Vector2f>& points)
{
	if (points.size() == 0)
		return std::vector<Vector2f>();
	int minY = 0;
	std::vector<Vector2f> convexHull = std::vector<Vector2f>();
	std::vector<bool> usedPoints = std::vector<bool>(points.size()); //True if point is used in convex hull(Wanted to use bitset, but size of points is unknown at compile time)
	convexHull.reserve(points.size());
	//Find point with smallest y value - O(n)
	for (int i = 0; i < points.size(); i++)
	{
		int compareResult = compareY(points[minY], points[i]);
		if (compareResult > 0)
			minY = i;
		else if (compareResult == 0) //If same y, find leftmost point (smallest x value)
			minY = (points[minY].x > points[i].x) ? i : minY;
	}
	convexHull.push_back(points[minY]);
	pApp->setPointColor(points[minY], Color{ 0.0f, 0.0f, 1.0f, 1.0f });
	usedPoints[minY] = true;
	int nextPoint = 0;
	//Find points on convex hull by "wrapping around" - O(nh)
	for (int i = 0; i < convexHull.size(); i++)
	{
		//Finds the rightmost point from the most recent vertex of convex hull. - O(n)
		for (int j = 0; j < points.size(); j++)
		{
			if (usedPoints[j])
				continue;
			if (!leftOf(convexHull[i], points[nextPoint], points[j]))
				nextPoint = j;
		}
		//Done if the first vertex of convex hull is to the right of the rightmost unused point. (Done after h iterations)
		if (i != 0 && !leftOf(convexHull[i], points[nextPoint], convexHull[0]))
			break;
		convexHull.push_back(points[nextPoint]);
		pApp->setPointColor(points[nextPoint], Color{ 0.0f, 0.0f, 1.0f, 1.0f });
		usedPoints[nextPoint] = true;
	}
	return convexHull;
}
