#pragma once
#include <math.h>
struct Vector2f
{
	float x, y;
};

struct Line
{
	Vector2f v1, v2;
};

inline float length(const Vector2f v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}
inline float dot(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
inline float cross(const Vector2f& v1, const Vector2f& v2)
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

inline bool collinear(const Vector2f& a, const Vector2f& b, const Vector2f c)
{
    return cross(b - a, c - a) == 0;
}

inline bool between(const Vector2f& a, const Vector2f& b, const Vector2f c)
{
    if (!collinear(a, b, c))
        return false;
    if (a.x != b.x)
        return ((c.x >= a.x && c.x <= b.x) || (c.x >= b.x && c.x <= a.x));
    else
        return ((c.y >= a.y && c.y <= b.y) || (c.y >= b.y && c.y <= a.y));
}

//Returns true if point is left of the line.
inline bool leftOf(const Line& line, const Vector2f& point)
{
	return cross(line.v2 - line.v1, point - line.v1) > 0;
}
inline bool leftOf(const Vector2f& a, const Vector2f& b, const Vector2f& point)
{
	return cross(b - a, point - a) > 0;
}

//Returns true if point is left or on the line.
inline bool leftOn(const Line& line, const Vector2f& point)
{
	return cross(line.v2 - line.v1, point - line.v1) >= 0;
}
inline bool leftOn(const Vector2f& a, const Vector2f& b, const Vector2f& point)
{
	return cross(b - a, point - a) >= 0;
}

//Assume counter-clockwise direction
inline bool isConvex(const Vector2f& before, const Vector2f& current, const Vector2f& after)
{
	return !leftOf(before, after, current);
}

inline bool tIntersect(const Vector2f& a, const Vector2f& b, const Vector2f c, const Vector2f d)
{
    if (collinear(a, b, c) && collinear(a, b, d))
        return false;

    return (between(a, b, c) ||
			between(a, b, d) ||
			between(c, d, a) ||
			between(c, d, b)	);
}

//Return if lines ab and lines cd intersect at one point.
inline bool intersect(const Vector2f& a, const Vector2f& b, const Vector2f c, const Vector2f d)
{
	return (!leftOn(a, b, c) != !leftOf(a, b, d) && !leftOn(c, d, a) != !leftOf(c, d, b));
}

