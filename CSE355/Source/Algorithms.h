#pragma once
#include "Primatives.h"

//O(nh) where n = num of points, h = num of outputs
inline std::vector<Vector2f> convexHullGW(std::vector<Vector2f>& points)
{
	if (points.size() == 0)
		return std::vector<Vector2f>();
	int minY = 0;
	std::vector<Vector2f> convexHull = std::vector<Vector2f>();
	std::vector<bool> usedPoints = std::vector<bool>(points.size()); //True if point is used in convex hull(Wanted to use bitset, but size of points is unknown at compile time)
	convexHull.reserve(points.size());
	//Find point with smallest y value - O(n)
	for (int i = 0; i < points.size(); i ++)
	{
		int compareResult = compareY(points[minY], points[i]);
		if (compareResult > 0)
			minY = i;
		else if (compareResult == 0) //If same y, find leftmost point (smallest x value)
			minY = (points[minY].x > points[i].x) ? i : minY;
	}
	convexHull.push_back(points[minY]);
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
			if (!leftOf(convexHull[i], points[nextPoint] , points[j]))
				nextPoint = j;
		}
		//Done if the first vertex of convex hull is to the right of the rightmost unused point. (Done after h iterations)
		if (i != 0 && !leftOf(convexHull[i], points[nextPoint], convexHull[0])) 
			break;
		convexHull.push_back(points[nextPoint]);
		usedPoints[nextPoint] = true;
	}
	return convexHull;
}


inline size_t partition(std::vector<int>& indices, std::vector<float>& dots, size_t start, size_t end)
{
	size_t i = start;
	for (int j = start; j < end; j++)
	{
		if (dots[j] >= dots[end - 1])
		{
			int temp = indices[i];
			indices[i] = indices[j];
			indices[j] = temp;
			float tempf = dots[i];
			dots[i] = dots[j];
			dots[j] = tempf;
			i++;
		}
	}
	return --i;
}

inline void quickSort(std::vector<int>& indices, std::vector<float>& dots, size_t start, size_t end)
{
	if (end - start <= 1)
		return;

	size_t p = partition(indices, dots, start, end);
	quickSort(indices, dots, start, p);
	quickSort(indices, dots, p, end);
}

inline void merge(std::vector<int>& indices, std::vector<float>& dots, size_t start, size_t middle, size_t end, 
		std::vector<int>& cpyIdx, std::vector<float>& cpyDots)
{
	int i = start;
	int j = middle;
	for (int k = start; k < end; k++)
	{
		cpyIdx[k] = indices[k];
		cpyDots[k] = dots[k];
	}
	for (int k = start; k < end; k++)
	{
		if (i != middle && (j == end || cpyDots[i] >= cpyDots[j]))
		{
			indices[k] = cpyIdx[i];
			dots[k] = cpyDots[i++];
		}
		else
		{
			indices[k] = cpyIdx[j];
			dots[k] = cpyDots[j++];
		}
	}
}

inline void split(std::vector<int>& indices, std::vector<float>& dots, size_t start, size_t end, 
		std::vector<int>& cpyIdx, std::vector<float>& cpyDots)
{
	if (end - start <= 1)
		return;
	size_t middle = (end - start) / 2 + start;
	
	split(indices, dots, start, middle, cpyIdx, cpyDots);
	split(indices, dots, middle, end, cpyIdx, cpyDots);
	merge(indices, dots, start, middle, end, cpyIdx, cpyDots);
}

//Graham Scan - O(nlogn) - we choose an extreme point, pivot, that is guaranteed to be on the convex hull.
//Then we sort the points by angle from the pivot point, using dot product. 
//We can then use left tests and "wrap" the points in sorted counter-clockwise order.
inline std::vector<Vector2f> convexHullGraham(std::vector<Vector2f>& points)
{
	std::vector<int> indices = std::vector<int>(points.size());
	//Find the pivot point(Point with lowest y value, higher x value is used to break ties).  Rightmost of the bottommost
	int pivot = 0;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].y < points[pivot].y)
			pivot = i;
		else if (points[i].y == points[pivot].y)
			pivot = (points[i].x > points[pivot].x) ? i : pivot;
		indices[i] = i;
	}

	//Get the dot products between the vector horizontal from pivot and the vector from pivot to any given point.
	//This dot product will be used to sort the points (1 -> -1).  
	std::vector<float> dots = std::vector<float>(points.size());
	for (int i = 0; i < dots.size(); i++)
	{
		dots[i] = dot({ 1, 0 }, normalize(Vector2f{ points[i] - points[pivot] }));
	}
	//Merge sort used to sort points in O(nlogn)
	std::vector<int> cpyIdx = std::vector<int>(indices.size());
	std::vector<float> cpyDots = std::vector<float>(dots.size());
	split(indices, dots, 0, points.size(), cpyIdx, cpyDots);

	//"Stack", but I use an array.
	std::vector<int> hullI = std::vector<int>();
	hullI.push_back(pivot);
	//Keep a record of 
	int start = 0;
	//The pivot point may or may not be in the front of the points array.  We want to make sure that it isn't added again.
	while (indices[start] == pivot)
		start++;
	hullI.push_back(indices[start]);
	for (int i = start + 1; i < points.size(); i ++)
	{
		if (indices[i] == pivot) //Ignore the pivot point
			continue;
		if (leftOf(points[hullI[hullI.size() - 2]], points[hullI[hullI.size() - 1]], points[indices[i]]))
			hullI.push_back(indices[i]);
		else
		{
			//Remove from the end of the hull "stack" until the point satisfies the left test.
			do
			{
				hullI.erase(--hullI.end());
			} while (hullI.size() >= 2 && !leftOf(points[hullI[hullI.size() - 2]], points[hullI[hullI.size() - 1]], points[indices[i]]));
			hullI.push_back(indices[i]);
		}
	}
	std::vector<Vector2f> hullPoints = std::vector<Vector2f>();
	for (int i : hullI)
		hullPoints.push_back(points[i]);
	return hullPoints;
}

inline std::vector<Vector2f> quickHull(std::vector<Vector2f>& points, Vector2f left, Vector2f right)
{
	if (points.empty())
		return std::vector<Vector2f>();
	Vector2f top;
	Vector2f leftRightUnit = normalize(Vector2f(right - left));
	float maxDist = 0;
	for (Vector2f& p : points)
	{
		Vector2f leftToP = p - left;
		Vector2f projection = leftRightUnit * dot(leftToP, leftRightUnit);
		float dist = abs((leftToP - projection).y);
		if (dist > maxDist)
		{
			maxDist = dist;
			top = p;
		}
	}
	std::vector<Vector2f> leftPoints;
	std::vector<Vector2f> rightPoints;
	for (Vector2f& p : points)
	{
		if (leftOf(left, top, p))
			leftPoints.push_back(p);
		else if (leftOf(top, right, p))
			rightPoints.push_back(p);
	}
	std::vector<Vector2f> leftHull = quickHull(leftPoints, left, top);
	std::vector<Vector2f> rightHull = quickHull(rightPoints, top, right);
	std::vector<Vector2f> topHull;
	for (Vector2f& p : rightHull)
		topHull.push_back(p);
	topHull.push_back(top);
	for (Vector2f& p : leftHull)
		topHull.push_back(p);
	return topHull;
}

inline std::vector<Vector2f> quickHull(std::vector<Vector2f>& points)
{
	Vector2f left = points[0];
	Vector2f right = points[0];
	for (Vector2f& p : points)
	{
		if (p.x < left.x)
			left = p;
		else if (p.x == left.x)
			left = (p.y > left.y) ? p : left;
		if (p.x > right.x)
			right = p;
		else if (p.x == right.x)
			right = (p.y > right.y) ? p : right;
	}
	std::vector<Vector2f> topPoints;
	std::vector<Vector2f> bottomPoints;
	for (Vector2f& p : points)
	{
		if (leftOf(left, right, p))
			topPoints.push_back(p);
		else if (p != left && p != right)
			bottomPoints.push_back(p);
	}
	std::vector<Vector2f> topHull = quickHull(topPoints, left, right);
	std::vector<Vector2f> bottomHull = quickHull(bottomPoints, right, left);

	std::vector<Vector2f> hull;
	hull.push_back(right);
	for (Vector2f& v : topHull)
		hull.push_back(v);
	hull.push_back(left);
	for (Vector2f& v : bottomHull)
		hull.push_back(v);
	return hull;
}