#include "PCH.h"
#include "TriSweepLineStatus.h"

TriSweepLineStatus::TriSweepLineStatus()
	:
	mTree(std::function<bool(const Edge&, const Edge&)>([this](const Edge& e1, const Edge& e2) { return edgeLess(e1, e2); }))
{
}

bool TriSweepLineStatus::insert(const Edge& edge)
{
	return mTree.insert(edge);
}

bool TriSweepLineStatus::remove(const Edge& edge)
{
	return mTree.remove(edge);
}

std::pair<bool, bool> TriSweepLineStatus::findNeighbors(const Edge& edge, Edge* pPre, Edge* pSuc)
{
	return mTree.findNeighbors(edge, pPre, pSuc);
}

void TriSweepLineStatus::setY(float y)
{
	this->y = y;
}