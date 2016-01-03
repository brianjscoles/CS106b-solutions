/*
 * File: pqlist.cpp
 * ----------------
 * This file implements the priority queue class as a singly-linked
 * list of integers stored in descreasing order.  This makes it easy
 * dequeue the max (it's right in front), but hard to enqueue since we
 * have to search for the proper position to insert the new cell.
 *
 * Julie Zelenski, CS106
 */
 
#include "pqueue.h"
#include "genlib.h"
#include <iostream>


/* Implementation notes: PQueue class
 * ----------------------------------
 * The private section for the pqlist looks like this:
 	struct Cell {
 	  int value;
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
    for (Cell *cur = head; cur != NULL; cur = cur->next)
    	count++;
    return count;
}


/* Implementation notes: enqueue
 * -----------------------------
 * We have to search to find the proper position, which can be a bit tricky with
 * a singly-linked list.  We walk two parallel pointers, one a step behind the other,
 * until we find the correct position to insert the new cell, which we then splice
 * into place. Note the special case of inserting at the head. Alternatively, this
 * operation could work recursively.
 */
void PQueue::enqueue(int newValue)
{
 	Cell *cur, *prev, *newOne = new Cell;
 	
 	newOne->value = newValue;
 	
 	for (prev = NULL, cur = head; cur != NULL; prev=cur, cur = cur->next) {
 		if (newValue > cur->value) break;
 	}
 	newOne->next = cur;
 	if (prev) 
 		prev->next = newOne;
 	else 
 		head = newOne;
}


/* Implementation notes: dequeueMax
 * --------------------------------
 * The maximum value is kept at the head of the list, so it's easy to find and
 * remove. Note we take care to free the memory for the deleted cell.
 */
int PQueue::dequeueMax()
{
	if (isEmpty())
		Error("Tried to dequeue max from an empty pqueue!");
	
 	Cell *toBeDeleted = head;
 	int value = head->value;
 	head = head->next; 	// splice head cell out
	delete toBeDeleted;
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
	return "sorted linked list";
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
       cout << "Cell #" << count << " (at address " << cur << ") val = " << cur->value
             << " next = " << cur->next << endl;
       count++;
	}
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}

