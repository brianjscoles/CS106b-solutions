/*
 * File: pqheap.cpp
 * ------------------
 * This file implements the priority queue class as a binary tree or "heap."  
 */



/* Implementation notes: PQueue class
 * ----------------------------------
 * The private section for the pqvector looks like this:
 *
 *	Vector<ElemType> entries;
 */

#include "pqueue.h"

template <typename ElemType>
PQueue<ElemType>::PQueue()
{
}

template <typename ElemType>
PQueue<ElemType>::~PQueue()
{
}

template <typename ElemType>
bool PQueue<ElemType>::isEmpty()
{
    return (entries.isEmpty());  
}

template <typename ElemType>
int PQueue<ElemType>::size()
{
	return (entries.size());
}


/* Implementation notes: enqueue
 * -----------------------------
 * Appends a new element to the end of the heap, then uses a "bubbling"
 * algorithm to adjust the heap so that no child value is larger than 
 * its parents.  Takes advantage of the fact that for the index+1 of any 
 * child value is 2x the index+1 of its parent.
 */
template <typename ElemType>
void PQueue<ElemType>::enqueue(ElemType newValue, int (cmp)(ElemType, ElemType) = OperatorCmp)
{
    //add new value to end.
	entries.add(newValue);

    
    
    //"bubble up" that new value to make sure it is no larger than its parent.
    int childIndex = entries.size()-1;
    int parentIndex = (childIndex+1)/2-1;
    while(parentIndex>=0 && cmp(entries[childIndex], entries[parentIndex])>0){
        swap(entries[childIndex], entries[parentIndex]);
        childIndex = parentIndex;
        parentIndex = (childIndex+1)/2-1;
    }
}


/* Implementation notes: dequeueMax
 * --------------------------------
 * The largest element is always the root of the heap, at index 0.
 * returns that value, and then adjusts the heap, by moving the very
 * last element into the root position and then "bubbling" it down.
 */
template <typename ElemType>
ElemType PQueue<ElemType>::dequeueMax(int (cmp)(ElemType, ElemType) = OperatorCmp) {	
	if (isEmpty())
		Error("Tried to dequeue max from an empty pqueue!");

	ElemType maxValue = entries[0];
    
    //fetch LAST element in vector and put it up front.
    entries[0] = entries[entries.size()-1];
    entries.removeAt(entries.size()-1);

    //now bubble down, or heapify!
    int current = 0;
    int leftChild;
    int rightChild;
    int maxChild;
    while(true){
        leftChild = (current+1)*2-1;
        rightChild = (current+1)*2;
        if(leftChild < entries.size()) maxChild = leftChild;
        //break immediately if current parent has no childen (it can't bubble down any more!)
        else break;     
        if(rightChild < entries.size() && cmp(entries[rightChild], entries[leftChild])>0) maxChild = rightChild;
        if(cmp(entries[current], entries[maxChild]) >=0) break;
        swap(entries[current], entries[maxChild]);
        current = maxChild;
    }

	return maxValue;
}

template <typename ElemType>
int PQueue<ElemType>::bytesUsed() 
{
	return sizeof(*this) + entries.bytesUsed();
}



