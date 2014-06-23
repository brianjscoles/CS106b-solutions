/*
 * Project: Recursion exercises
 * Stanford CS106b Assignment 3
 * Written June 2014 by BScoles
 * Written for C++; compiled in MS Visual Studio
 */



#include <iostream>
#include "simpio.h"


/* Function Prototypes */ 

int CountWays(int numStairs);
void climbStairs(int numstairs, int &ways_to_climb);




int main ()
{


    // Test code for problem 1
    cout << "Problem 1: ways to climb stairs." << endl << endl;
    for(int i = -3; i <= 10; i++){
        cout << "There are " << CountWays(i) << " ways to climb a set of " << i << " stairs." << endl;
    }
    
    return 0;
}
    


/* Problem 1: stairclimbing.
 * This function determines how many different ways there are to climb a staircase of
 * a given length, if with each stride you can choose to advance up by one stair or two.
 * Returns an integer.  Returns 0 if the parameter numStairs is negative.
 */

// wrapper function
int CountWays(int numStairs) {
    int ways_to_climb = 0;
    climbStairs(numStairs, ways_to_climb);
    return ways_to_climb;
}

// recursive function
void climbStairs(int numstairs, int &ways_to_climb){
    if(numstairs == 0) ways_to_climb ++;
    else if (numstairs > 0){
        climbStairs(numstairs -1, ways_to_climb);
        climbStairs(numstairs -2, ways_to_climb);
    }

}