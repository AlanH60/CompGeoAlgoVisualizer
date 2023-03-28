#pragma once
#include "Direct2D/Types.h"
#include "Vector2f.h"

struct Vector2D
{
	double x = std::numeric_limits<double>::max(), y = std::numeric_limits<double>::max();
	Vector2D() = default;
	Vector2D(double x, double y) : x(x), y(y) {}

	operator FLOAT2()
	{
		return FLOAT2( (float)x, (float)y );
	}

	operator Vector2f()
	{
		return Vector2f((float)x, (float)y);
	}
};

inline Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
{
	return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
	return { v1.x - v2.x, v1.y - v2.y };
}
inline Vector2D operator/(const Vector2D& v1, float s)
{
	return { v1.x / s, v1.y / s };
}
inline Vector2D operator*(const Vector2D& v1, float s)
{
	return { v1.x * s, v1.y * s };
}
inline bool operator==(const Vector2D& v1, const Vector2D& v2)
{
	return EQUALD(v1.x, v2.x) && EQUALD(v1.y, v2.y);
}
inline bool operator!=(const Vector2D& v1, const Vector2D& v2)
{
	return !(v1 == v2);
}

inline bool operator<(const Vector2D& v1, const Vector2D v2)
{
	return LESSD(v1.x, v2.x) || (EQUALD(v1.x, v2.x) && LESSD(v1.y, v2.y));
}

inline bool operator>(const Vector2D& v1, const Vector2D v2)
{
	return GREATERD(v1.x, v2.x) || (EQUALD(v1.x, v2.x) && GREATERD(v1.y, v2.y));
}

namespace std
{
	template<>
	struct hash<Vector2D>
	{
		size_t operator()(Vector2D const& in) const
		{
			size_t seed = 0;
			seed ^= std::hash<long>()((long)in.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<long>()((long)in.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}