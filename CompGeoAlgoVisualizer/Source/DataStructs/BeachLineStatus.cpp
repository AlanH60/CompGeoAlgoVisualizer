#include "PCH.h"
#include "BeachLineStatus.h"

BeachLineStatus::BeachLineStatus()
	:
	mTree(std::function<bool(const Vector2f&, const Vector2f&)>([this](const Vector2f& v1, const Vector2f& v2) { return v1.y < v2.y || (v1.y == v2.y && v1.x > v2.x); }))
{
}

bool BeachLineStatus::insert(const Vector2f& point)
{
	return mTree.insert(point);
}

bool BeachLineStatus::remove(const Vector2f& point)
{
	return mTree.remove(point);
}

std::pair<bool, bool> BeachLineStatus::findNeighbors(const Vector2f& point, Vector2f* pPre, Vector2f* pSuc)
{
	return mTree.findNeighbors(point, pPre, pSuc);
}

void BeachLineStatus::setY(float y)
{
	this->y = y;
}