#pragma once
#include "Direct2D/Types.h"
struct Vector2D;
struct Vector2f
{
	float x = std::numeric_limits<float>::max(), y = std::numeric_limits<float>::max();
	Vector2f() = default;
	Vector2f(float x, float y) : x(x), y(y) {}

	operator FLOAT2()
	{
		return FLOAT2( x, y );
	}

	operator Vector2D();
};

inline Vector2f operator+(const Vector2f& v1, const Vector2f& v2)
{
	return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2f operator-(const Vector2f& v1, const Vector2f& v2)
{
	return { v1.x - v2.x, v1.y - v2.y };
}
inline Vector2f operator/(const Vector2f& v1, float s)
{
	return { v1.x / s, v1.y / s };
}
inline Vector2f operator*(const Vector2f& v1, float s)
{
	return { v1.x * s, v1.y * s };
}
inline bool operator==(const Vector2f& v1, const Vector2f& v2)
{
	return EQUALF(v1.x, v2.x) && EQUALF(v1.y, v2.y);
}
inline bool operator!=(const Vector2f& v1, const Vector2f& v2)
{
	return !(v1 == v2);
}

inline bool operator<(const Vector2f& v1, const Vector2f v2)
{
	return LESSF(v1.x, v2.x) || (EQUALF(v1.x, v2.x) && LESSF(v1.y, v2.y));
}

inline bool operator>(const Vector2f& v1, const Vector2f v2)
{
	return GREATERF(v1.x, v2.x) || (EQUALF(v1.x, v2.x) && GREATERF(v1.y, v2.y));
}

namespace std
{
	template<>
	struct hash<Vector2f>
	{
		size_t operator()(Vector2f const& in) const
		{
			size_t seed = 0;
			seed ^= std::hash<int>()((int)in.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<int>()((int)in.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}