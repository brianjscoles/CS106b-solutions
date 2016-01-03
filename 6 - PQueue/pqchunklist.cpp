/*
 * File: pqlistchunklist.cpp
 * ----------------
 * This file implements the priority queue class as chunklist-linked queue with decreaseing order.
 * The length of this chunk is 4;
 * Kai Yao, CS106
 */
#include <cmath>
#include "pqueue.h"
#include "genlib.h"
#include <iostream>


/* Implementation notes: PQueue class
 * ----------------------------------
 * The private section for the pqlist looks like this:
	const int MaxElemsPerBlock = 4;
	struct Cell {
 	  int entries[MaxElemsPerBlock]; 
	  int count;
 	  Cell *next;
   };
   Cell *head;
 */



/* Implementation notes: constructor
 * ---------------------------------
 * We do not have a dummy cell, since it really won't help us much.
 * When the head is NULL, the pqueue is empty.
 */
PQueue::PQueue()
{
    head = NULL;
}

PQueue::~PQueue()
{
    while (head != NULL) {
    	Cell *next = head->next;
    	delete head;
    	head = next;
    }
}

bool PQueue::isEmpty()
{
    return (head == NULL);  
}

/* Implementation notes: size
 * --------------------------
 * This version doesn't cache the number of entries, so we must traverse the list to count.
 */
int PQueue::size()
{
    int count = 0;
	for (Cell *cur = head; cur != NULL; cur = cur->next) {
		count += cur->count;
	}
    return count;
}


/* Implementation notes: enqueue
 * -----------------------------
 * We have to search to find the proper position, which can be a bit tricky with
 * a chunkList. if insertPos is -1, then will need to insert this chunkList to the 
 * end of the list.
 * Other wise we have already known the insert position so we check if the chunkList
 * is full. If it is then we split the chunkList to two and insert new value to the 
 * right position. (Maybe is the first half of the chunkList or the second half.)
 * if the chunklist is not full, insert a new value is trivial.
 */
void PQueue::enqueue(int newValue)
{
 	Cell *cur, *prev, *newOne = new Cell;
 	
 	newOne->entries[0] = newValue;
	/*for(int i = 1; i < MaxElemsPerBlock; i++) {
		newOne->entries[i] = -1;
	}*/
	newOne->count = 1;
 	
	int insertPos = -1;
 	for (prev = NULL, cur = head; cur != NULL; prev=cur, cur = cur->next) {
		bool isfound = false;
		if(newValue > cur->entries[0] && cur->count == MaxElemsPerBlock) {
			break;
		} 

		for(int i = 0; i < cur->count; i++) {
			if(newValue > cur->entries[i]) {
				insertPos = i;
				isfound = true;
				break;
			}
		}
		if(isfound) break;
 	}
	
	if(insertPos == -1) {
		newOne->next = cur;
		if(prev)
			prev->next = newOne;
		else 
			head = newOne;
		return;
	} 
	if(cur->count == MaxElemsPerBlock) {
		//split
		newOne->next = cur->next;
		cur->next = newOne;
		
		int i, j;
		newOne->count--;
		for(i = floor(MaxElemsPerBlock/2.0), j = 0; i <= (MaxElemsPerBlock - 1); i++, j++) {
			newOne->entries[j] = cur->entries[i];
			cur->count--;
			newOne->count++;
		}
		//insert into new array
		if(insertPos >= floor(MaxElemsPerBlock/2.0)) {
			insertPos = insertPos - floor(MaxElemsPerBlock/2.0);
			for(int i = floor(MaxElemsPerBlock/2.0); i >= insertPos; i--) {
				newOne->entries[i + 1] = newOne->entries[i];
			}
			newOne->entries[insertPos] = newValue;
			newOne->count++;
		} else {
			for(int i = floor(MaxElemsPerBlock/2.0) - 1; i >= insertPos; i--) {
				cur->entries[i + 1] = cur->entries[i];
			}
			cur->entries[insertPos] = newValue;
			cur->count++;
		}


	} else {
		for(int i = cur->count - 1; i >= insertPos; i--) {
			cur->entries[i + 1] = cur->entries[i];
		}
		cur->entries[insertPos] = newValue;
		cur->count++;
	}

 	//Cell *cur, *prev, *newOne = new Cell;
 	//
 	//newOne->value = newValue;
 	//
 	//for (prev = NULL, cur = head; cur != NULL; prev=cur, cur = cur->next) {
 	//	if (newValue > cur->value) break;
 	//}
 	//newOne->next = cur;
 	//if (prev) 
 	//	prev->next = newOne;
 	//else 
 	//	head = newOne;
}


/* Implementation notes: dequeueMax
 * --------------------------------
 * Dequeue the max value and if this chunklist is empty, delete it.
 */
int PQueue::dequeueMax()
{
	if (isEmpty())
		Error("tried to dequeue max from an empty pqueue!");
	
	Cell *toBeDeleted = head;
	int value = head->entries[0];
	head->count--;
	if(head->count == 0) {
		head = head->next;
		delete toBeDeleted;
	} else {
		for(int i = 1; i <= head->count; i++) {
			head->entries[i - 1] = head->entries[i];
		}
	}
 //	Cell *toBeDeleted = head;
 //	int value = head->value;
 //	head = head->next; 	// splice head cell out
	//delete toBeDeleted;
	//return value;
	return value;
}


/* Implementation notes: bytesUsed
 * -------------------------------
 * The space needed is the sum of the object + the size of all the
 * allocated list cells.
 */
int PQueue::bytesUsed()
{
	int total = sizeof(*this);
 	for (Cell *cur = head; cur != NULL; cur = cur->next)
 		total += sizeof(*cur);
 	return total;
}
	

string PQueue::implementationName()
{
	return "sorted chunkList";
}

/*
 * Implementation notes: printDebuggingInfo
 * ----------------------------------------
 * The version for the singly-linked list prints the node structure
 * in order as a debugging aid to keeping track of the cell contents
 * and the pointers between them. It prints each cell in order
 */
void PQueue::printDebuggingInfo()
{
   int count = 0;

	cout << "------------------ START DEBUG INFO ------------------" << endl;
	for (Cell *cur = head; cur != NULL; cur = cur->next) {
		for(int i = 0; i < cur->count; i++) {
			cout << "Cell #" << count << " (at address " << cur->entries + i << ") val = " << cur->entries[i]
             << " next = " << cur->next << endl;
			count++;	
		}
       
	}
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}

