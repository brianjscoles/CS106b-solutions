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

/* Function Prototypes */ 

// Prototypes for warmup 1 
void PrintInBinary(int number);

// Prototypes for warmup 2
bool SumSubGroup(Vector<int> nums, int targetSum, int sum);
bool CanMakeSum(Vector<int> & nums, int targetSum);



int main ()
{
    
    //Test code for warmup 1.  
    cout << "Warmup 1: printing binary values for integers 1-50." << endl;
    for(int i = 0; i <= 50; i++){
        cout << i << ": ";
        PrintInBinary(i);
        cout << endl;
    }
    cout << endl << endl;

    
    //Test code for warmup 2.
    cout <<"Warmup 2: summing subgroups of a Vector." << endl;
    Vector<int> vec1;
    vec1.add(4);
    vec1.add(-2);
    vec1.add(5);
    for(int i = -5; i <= 10; i++){
        cout << "Checking for sum of " << i << " in subgroups of Vector [-2, 4, 5]: ";
        if(CanMakeSum(vec1, i)) cout << "true" << endl << endl;
        else cout << "false" << endl << endl;
    }
    cout << endl << endl;

    return 0;
}




/* Warmup #1: Print the binary version of a non-negative integer */ 

void PrintInBinary(int number){
    if(number/2 == 0)cout << number;
    else {
        PrintInBinary(number/2);
        cout << number%2;
    }

}



/* Warmup #2: Determine if a Vector<int> contains any subgroups that sum to a target integer.
 * This is a backtracking recursive function, so it will stop and "unstack" itself as soon as a valid subgroup is found.
 */ 

// wrapper function
bool CanMakeSum(Vector<int> & nums, int targetSum) {
    return SumSubGroup(nums, targetSum, 0);
}

// recursive backtracking function
bool SumSubGroup(Vector<int> nums, int targetSum, int sum) {

// base case 
    if(sum==targetSum) return true;

// recursive case.  choose a number from the available pool to use next, and generate a new branch for that choice.
    for(int i = 0; i < nums.size(); i++){
        int new_sum = nums[i] + sum;
        Vector<int> new_nums = nums;
        new_nums.removeAt(i);
        if(SumSubGroup(new_nums, targetSum, new_sum) return true;
    }
    return false;
}  

