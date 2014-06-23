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

int CutStock(Vector<int> & requests, int stockLength);
void PlacePipe(Vector<int> requests, Vector<Vector<int> > assignments, Vector<Vector<int> > & solution, int stockLength);
bool pieceFits(int request, Vector<int> assignment, int stockLength);
int printSolution(Vector<Vector<int> > & solution);
bool bad_request(Vector<int> & requests, int stockLength);



int main ()
{

    // Test code for assignment 6: stock cutting!
    int stockLength = 10;
    Vector<int> requests;
    requests.add(4);
    requests.add(3);
    requests.add(4);
    requests.add(1);
    requests.add(7);
    requests.add(8);
    requests.add(1);
    requests.add(9);
    requests.add(9);
    requests.add(2);
    requests.add(2);
    
    cout << "Finding optimal solution for stock-cutting request!" << endl << endl;
    CutStock(requests,stockLength);

    return 0;
}



/* Problem 6: Pipe cutting.
 * This function accepts a Vector of ints that represents a set of desired pipe lengths, and an
 * int that represents the length of the stock pipes.  It returns the minimum number of stock pipes
 * required to obtain the desired set of pipe pieces, based on how the stock pieces might be cut up.
 */


// wrapper and setup function.
int CutStock(Vector<int> & requests, int stockLength) {
    if(bad_request(requests, stockLength)){
        cout << "Error - requested piece size exceeds stock length.";
        return 0;
    }
    Vector<Vector<int> > solution;
    Vector<Vector<int> > assignments;
    PlacePipe(requests,assignments,solution,stockLength);
    int min = printSolution(solution);
    return min;
}



// checks if any requested piece exceeds the maximum size allowed (which is stockLength).
bool bad_request(Vector<int> & requests, int stockLength){
    for(int i = 0; i< requests.size(); i++){
        if(requests[i] > stockLength) return true;
    }
    return false;
}



// recursive function
void PlacePipe(Vector<int> requests, Vector<Vector<int> > assignments, Vector<Vector<int> > & solution, int stockLength){

    // base case.  if all requested pieces have been assigned, checks if this solution is the shortest yet,
    // and saves it if so
    if(requests.size() == 0){
        if(assignments.size() < solution.size() || solution.size()==0){
            solution = assignments;
        }
    } else {

    // recursive case. begins by popping the next requested piece from the vector of requests.
        int request = requests[0];
        requests.removeAt(0);

    // tries adding the requested piece onto each of the existing stock pipes in use.  generates a new
    // branch for each such valid addition.
        for(int i = 0; i < assignments.size(); i++){
            if(pieceFits(request,assignments[i],stockLength)){
                assignments[i].add(request);
                PlacePipe(requests,assignments,solution,stockLength);
                assignments[i].removeAt(assignments[i].size()-1);
            }
        }
    // generates a new branch in which the requested piece goes onto a new stock pipe of its own.
        Vector<int> new_vec;
        new_vec.add(request);
        assignments.add(new_vec);
        PlacePipe(requests,assignments,solution,stockLength);
    }
}


// checks whether a given requested piece would fit on the remaining length of a given stock piece.
bool pieceFits(int request, Vector<int> assignment, int stockLength){
    //cout << "checking if a piece fits!" << endl;
    int sum_so_far = 0;
    for(int i = 0; i < assignment.size(); i++){
        sum_so_far += assignment[i];
    }
    if(request + sum_so_far <= stockLength) return true;
    else return false;
}


//prints the solution.  returns # of stock pieces required in optimal solution.
int printSolution(Vector<Vector<int> > & solution){
    cout << "The optimal solution uses " << solution.size() << " pieces." << endl;
    for(int i = 0; i < solution.size(); i++){
        cout << "Piece " << i+1 << ": ";
        for(int j = 0; j < solution[i].size(); j++){
            cout << solution[i][j] << "  ";
        }
        cout << endl;
    }
    return solution.size();
}
    
