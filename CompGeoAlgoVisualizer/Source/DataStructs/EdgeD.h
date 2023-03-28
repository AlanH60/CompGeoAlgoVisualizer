#pragma once
#include "DataStructs/Vector2D.h"
#include "DataStructs/Edge.h"
#include "DataStructs/Vector2f.h"

struct EdgeD
{
	Vector2D v1, v2;
	EdgeD() = default;
	EdgeD(const Vector2D v1, const Vector2D v2) : v1(v1), v2(v2) {}
	operator Edge() { return Edge((Vector2f)v1, (Vector2f)v2); }
};

inline bool operator==(const EdgeD& e1, const EdgeD& e2)
{
	return (e1.v1 == e2.v1 && e1.v2 == e2.v2);
}

inline bool operator!=(const EdgeD& e1, const EdgeD& e2)
{
	return !(e1 == e2);
}

inline bool operator<(const EdgeD& e1, const EdgeD& e2)
{
	return e1.v1 < e2.v1 || (e1.v1 == e2.v1 && e1.v2 < e2.v2);
}

inline bool operator>(const EdgeD& e1, const EdgeD& e2)
{
	return e1.v1 > e2.v1 || (e1.v1 == e2.v1 && e1.v2 > e2.v2);
}

namespace std
{
	template<>
	struct hash<EdgeD>
	{
		size_t operator()(EdgeD const& in) const
		{
			size_t seed = 0;
			seed ^= std::hash<Vector2D>()(in.v1) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<Vector2D>()(in.v2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}