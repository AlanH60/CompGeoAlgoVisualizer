#pragma once
#include "Direct2D/Types.h"

struct Vector2f
{
	float x = std::numeric_limits<float>::max(), y = std::numeric_limits<float>::max();
	Vector2f() = default;
	Vector2f invertY()
	{
		return { x, -y };
	}

	operator FLOAT2()
	{
		return FLOAT2{ x, y };
	}
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
	return abs(v1.x - v2.x) < EPSILONF && abs(v1.y - v2.y) < EPSILONF;
}
inline bool operator!=(const Vector2f& v1, const Vector2f& v2)
{
	return !(v1 == v2);
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