#pragma once
#include "DataStructs/Vector2f.h"

struct Edge
{
	Vector2f v1, v2;
	Edge() = default;
	Edge(const Vector2f v1, const Vector2f v2) : v1(v1), v2(v2) {}
};

inline bool operator==(const Edge& e1, const Edge& e2)
{
	return (e1.v1 == e2.v1 && e1.v2 == e2.v2);
}

inline bool operator!=(const Edge& e1, const Edge& e2)
{
	return !(e1 == e2);
}

inline bool operator<(const Edge& e1, const Edge& e2)
{
	return e1.v1 < e2.v1 || (e1.v1 == e2.v1 && e1.v2 < e2.v2);
}

inline bool operator>(const Edge& e1, const Edge& e2)
{
	return e1.v1 > e2.v1 || (e1.v1 == e2.v1 && e1.v2 > e2.v2);
}

namespace std
{
	template<>
	struct hash<Edge>
	{
		size_t operator()(Edge const& in) const
		{
			size_t seed = 0;
			seed ^= std::hash<Vector2f>()(in.v1) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<Vector2f>()(in.v2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}