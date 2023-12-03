#pragma once
#include "BeachLineStatus.h"

#define INIT_FORT_EVENT_QUEUE_SIZE 128ul

/**
* Max heap that orders the events based on their y-values.
*/
class FortuneEventQueue
{
	public:
		FortuneEventQueue();
		~FortuneEventQueue();

		bool empty();
		BeachLineStatus::Event* top();
		void pop();
		void push(BeachLineStatus::Event* e);
	private:
		//Doubles the size of the heap using malloc
		void expandHeap();
		void heapDown(size_t index);
		void heapUp(size_t index);
		void swap(size_t one, size_t two);

	private:
		//Max-Heap
		BeachLineStatus::Event** mHeap;
		//Size
		size_t mSize = 0;
		//Max size
		size_t mMaxSize = 0;
};
