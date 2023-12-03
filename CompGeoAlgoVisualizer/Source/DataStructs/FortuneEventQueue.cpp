#include "PCH.h"
#include "FortuneEventQueue.h"
FortuneEventQueue::FortuneEventQueue()
{
	mHeap = new BeachLineStatus::Event*[INIT_FORT_EVENT_QUEUE_SIZE];
	mMaxSize = INIT_FORT_EVENT_QUEUE_SIZE;
}

FortuneEventQueue::~FortuneEventQueue()
{
	delete[] mHeap;
}

bool FortuneEventQueue::empty()
{
	return mSize == 0;
}

BeachLineStatus::Event* FortuneEventQueue::top()
{
	if (mSize == 0)
		return nullptr;
	return mHeap[0];
}

void FortuneEventQueue::pop()
{
	mHeap[0] = mHeap[--mSize];
	heapDown(0);
}

void FortuneEventQueue::push(BeachLineStatus::Event* e)
{
	if (mSize == mMaxSize)
		expandHeap();
	mHeap[mSize] = e;
	heapUp(mSize++);
}

void FortuneEventQueue::expandHeap()
{
	BeachLineStatus::Event** tmp = new BeachLineStatus::Event*[mMaxSize * 2];
	for (size_t i = 0; i < mMaxSize; i++)
		tmp[i] = mHeap[i];
	mMaxSize *= 2;
	delete[] mHeap;
	mHeap = tmp;
}

void FortuneEventQueue::heapDown(size_t index)
{
	size_t left = index * 2 + 1;
	size_t right = index * 2 + 2;
	size_t child = 0;

	while (left < mSize) {
		if (right >= mSize)
			child = left;
		else //Pick child with bigger y
			child = (mHeap[right]->point.y > mHeap[left]->point.y) ? right : left; 

		if (mHeap[child]->point.y > mHeap[index]->point.y) {
			swap(child, index);
			index = child;
			left = index * 2 + 1;
			right = index * 2 + 2;
		}
		else
			break;
	}

}

void FortuneEventQueue::heapUp(size_t index)
{
	size_t parent = 0;

	while (index != 0) {
		parent = (index - 1) / 2;
		if (mHeap[index]->point.y > mHeap[parent]->point.y) {
			swap(index, parent);
			index = parent;
		}
		else
			break;
	}
}

void FortuneEventQueue::swap(size_t one, size_t two)
{
	BeachLineStatus::Event* tmp = mHeap[one];
	mHeap[one] = mHeap[two];
	mHeap[two] = tmp;
}
