#pragma once
#include <math.h>
struct Vector2f
{
	float x, y;
};
inline float length(const Vector2f v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}
inline float dot(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
inline float crossMag(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}
inline Vector2f operator+(const Vector2f& v1, const Vector2f& v2)
{
	return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2f operator-(const Vector2f& v1, const Vector2f& v2)
{
	return { v1.x - v2.x, v1.y - v2.y };
}
inline bool operator==(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x == v2.x && v1.y == v2.y;
}
inline bool operator!=(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x != v2.x || v1.y != v2.y;
}
inline int compareX(const Vector2f& v1, const Vector2f& v2)
{
	return (v1.x < v2.x) ? -1 : (v1.x > v2.x) ? 1 : 0;
}
inline int compareY(const Vector2f& v1, const Vector2f& v2)
{
	return (v1.y < v2.y) ? -1 : (v1.x > v2.x) ? 1 : 0;
}


struct Line
{
	Vector2f v1, v2;
};

//Returns true if point is left of points on line.
inline bool leftOf(const Line& line, const Vector2f& point)
{
	Vector2f lineV = line.v2 - line.v1;
	Vector2f toPointV = point - line.v1;
	return crossMag(lineV, toPointV) > 0;
}

//Assume counter-clockwise direction
inline bool isConvex(const Vector2f& before, const Vector2f& current, const Vector2f& after)
{
	Vector2f bToA = after - before;
	Vector2f bToC = current - before;
	float cross = crossMag(bToA, bToC);
	return cross <= 0;
}


