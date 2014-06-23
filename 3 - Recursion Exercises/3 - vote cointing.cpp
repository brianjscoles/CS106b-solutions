/*
 * Project: Recursion exercises
 * Stanford CS106b Assignment 3
 * Written June 2014 by BScoles
 * Written for C++; compiled in MS Visual Studio
 * 
 */



#include <iostream>
#include "simpio.h"
#include "vector.h"
#include "graphics.h"
#include <cmath>
#include "lexicon.h"

/* Function Prototypes */ 


int CountCriticalVotes(Vector<int> & blocks, int blockIndex);
void findCriticalOutcomes(Vector<int> blocks, int keyblock, int Alice, int Bob, int &critical_outcomes, int &total_outcomes);


int main ()
{
    
    //Test code for problem 3
    cout << "Finding critical voting outcomes..." << endl << endl;
    Vector<int> blocks;
    blocks.add(9);
    blocks.add(9);
    blocks.add(7);
    blocks.add(3);
    blocks.add(1);
    blocks.add(1);
    for(int i = 0; i < blocks.size(); i++){
        int critical_outcomes = CountCriticalVotes(blocks, i);
    }

    return 0;
}



/* Problem 3: Every vote counts (or does it?).
 * This function examines a Vector of voting blocks, models all possible voting states (assuming the only
 * two candidates are Alice and Bob), and prints out the number of outcomes in which the specified voting block
 * will hold a critical or deciding vote.  Also tracks and prints out the total number of possible outcomes.
 */

//wrapper function
int CountCriticalVotes(Vector<int> & blocks, int blockIndex){
    int keyblock = blocks[blockIndex];
    Vector<int> blockscopy = blocks;
    blockscopy.removeAt(blockIndex);
    int Alice = 0;
    int Bob = 0;
    int critical_outcomes = 0;
    int total_outcomes = 0;

    findCriticalOutcomes(blockscopy, keyblock, Alice, Bob, critical_outcomes, total_outcomes);
    
    cout << "The block of " << keyblock << " is critical in " << critical_outcomes 
    << " of " << total_outcomes << " possible outcomes." << endl;
    
    return critical_outcomes;
}


//recursive function
void findCriticalOutcomes(Vector<int> blocks, int keyblock, int Alice, int Bob, int &critical_outcomes, int &total_outcomes){
    if(blocks.size()==0){
        if(abs(Alice-Bob)<=keyblock) critical_outcomes++;
        total_outcomes++;
    } else {
        int next_block = blocks[0];
        blocks.removeAt(0);
        findCriticalOutcomes(blocks, keyblock, Alice+next_block, Bob, critical_outcomes, total_outcomes);
        findCriticalOutcomes(blocks, keyblock, Alice, Bob+next_block, critical_outcomes, total_outcomes);
    }
}
