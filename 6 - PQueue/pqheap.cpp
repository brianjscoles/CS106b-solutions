/*
 * File: pqvector.cpp
 * ------------------
 * This file implements the priority queue class as a vector
 * of integers stored in no particular order.  This makes it easy
 * enqueue new elements, but hard to dequeue the max (have to search for it).
 *
 * Julie Zelenski, CS106
 */
 
#include "pqueue.h"
#include "genlib.h"
#include <iostream>

/* Implementation notes: PQueue class
 * ----------------------------------
 * The private section for the pqvector looks like this:
 *	const static int INIT_SIZE = 10;
 *	int* entries;
 *  int capacity;
 *  int count;
 *  void swap(int & a, int & b);
 */


PQueue::PQueue()
{
	entries = new int[INIT_SIZE];
	entries[0] = -1;
	capacity = INIT_SIZE;
	count = 0;
}


PQueue::~PQueue()
{
}


bool PQueue::isEmpty()
{
	return count == 0;  
}

int PQueue::size()
{
	return count;
}


/* Implementation notes: enqueue
 * -----------------------------
 * Since we're keeping the vector in no particular order, we just append this
 * new element to the end.  It's the easiest/fastest thing to do.
 */
void PQueue::enqueue(int newValue)
{
	if(count == capacity - 1) {
		int* toBeDelete = entries;
		capacity *= 2;
		entries = new int[capacity];
		for(int i = 0; i <= count; i++) {
			entries[i] = toBeDelete[i]; 
		}
		delete[] toBeDelete;
	}

	count++;
	entries[count] = newValue;
	//heapify
	int curP, parentP;
	curP = count; //curP = current position
	parentP = curP/2;
	while(parentP >= 1 &&  entries[parentP] < entries[curP]) {
		//swap
		swap(entries[parentP], entries[curP]);
		curP = parentP;
		parentP = parentP/2;
	}

	

	//entries.add(newValue);
}


/* Implementation notes: dequeueMax
 * --------------------------------
 * Since we're keeping the vector in no particular order, we have to search to
 * find the largest element.  Once found, we remove it from the vector and
 * return that value.
 */
int PQueue::dequeueMax()
{	

	
	if (isEmpty())
		Error("Tried to dequeue max from an empty pqueue!");
	
	count--;
	int maxValue = entries[1];
	entries[1] = entries[count + 1];
	
	
	int curP = 1;
	while(true) {
		if(curP * 2 > count) break;// do not have any child
		if(curP * 2 + 1 > count) {
			//only have left child
			if(entries[curP] > entries[curP * 2 + 1]) 
				break; // right pos
			else {
				swap(entries[curP], entries[curP * 2 + 1]);
				curP = curP * 2 + 1;
			}
		} else {
			//have two child
			if(entries[curP] > entries[curP * 2] && entries[curP] <= entries[curP * 2 + 1]) {
				swap(entries[curP], entries[curP * 2 + 1]); 
				curP = curP * 2 + 1;
			} else if(entries[curP] <= entries[curP * 2] && entries[curP] > entries[curP * 2 + 1]) {
				swap(entries[curP], entries[curP * 2]); 
				curP = curP * 2;
			} else if(entries[curP] <= entries[curP * 2] && entries[curP] <= entries[curP * 2 + 1] && entries[curP * 2 + 1] <= entries[curP * 2]) {
				swap(entries[curP], entries[curP * 2]); 
				curP = curP * 2;
			} else if(entries[curP] <= entries[curP * 2] && entries[curP] <= entries[curP * 2 + 1] && entries[curP * 2 + 1] > entries[curP * 2]) {
				swap(entries[curP], entries[curP * 2 + 1]); 
				curP = curP * 2 + 1;
			} else {
				break;// right pos
			}

		}
	}

	return maxValue;

	//int maxIndex = 0;	// assume first element is largest until proven otherwise
	//int maxValue = entries[0];
	//for (int i = 1; i < entries.size(); i++) {
	//	if (entries[i] > maxValue) {
	//		maxValue = entries[i];
	//		maxIndex = i;
	//	}
	//}
	//entries.removeAt(maxIndex);	// remove entry from vector
	//return maxValue;
	
}
		

void PQueue::swap(int & a, int & b) {
	int temp = a;
	a = b;
	b = temp;
}


int PQueue::bytesUsed() 
{
	return sizeof(int) * capacity;
}


string PQueue::implementationName()
{
	return "Heap";
}

void PQueue::printDebuggingInfo()
{
	cout << "------------------ START DEBUG INFO ------------------" << endl;
	cout << "Pqueue contains " << count << " entries" << endl;
	if(count != 0) {
		//printHeap(1);
		for (int i = 1; i <= count; i++) 
			cout << entries[i] << " ";
		cout << endl;
	}
	
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}
