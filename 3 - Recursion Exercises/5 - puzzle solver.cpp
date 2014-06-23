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

bool Solvable(int start, Vector<int> & squares);
bool PuzzleSolvable(int start, Vector<int> & squares, Vector<bool> square_unused);



int main ()
{
    
    

    // Test code for problem 5: vector puzzle solver.
    cout << "Now trying to solve some \"square\" vector puzzles!" << endl << endl;
    Vector<int> puzzle;
    string puzzle_digits = "3641342530";
    puzzle.add(3);
    puzzle.add(6);
    puzzle.add(4);
    puzzle.add(1);
    puzzle.add(3);
    puzzle.add(4);
    puzzle.add(2);
    puzzle.add(5);
    puzzle.add(3);
    puzzle.add(0);
    if(Solvable(1, puzzle)) cout << "Yes, puzzle " << puzzle_digits << " has a solution." << endl;
    else cout << "No solution found for puzzle " << puzzle_digits << "." << endl;

    Vector<int> puzzle2;
    string puzzle2_digits = "31230";
    puzzle2.add(3);
    puzzle2.add(1);
    puzzle2.add(2);
    puzzle2.add(3);
    puzzle2.add(0);
    if(Solvable(1, puzzle2)) cout << "Yes, puzzle " << puzzle2_digits << " has a solution." << endl;
    else cout << "No solution found for puzzle " << puzzle2_digits << "." << endl;

    Vector<int> puzzle3;
    string puzzle3_digits = "32123441320";
    puzzle3.add(3);
    puzzle3.add(2);
    puzzle3.add(1);
    puzzle3.add(2);
    puzzle3.add(3);
    puzzle3.add(4);
    puzzle3.add(4);
    puzzle3.add(1);
    puzzle3.add(3);
    puzzle3.add(2);
    puzzle3.add(0);
    if(Solvable(1, puzzle3)) cout << "Yes, puzzle " << puzzle3_digits << " has a solution." << endl;
    else cout << "No solution found for puzzle " << puzzle3_digits << "." << endl;

    return 0;
}




/* Problem 5: Vector puzzle solver.
 * This function accepts a Vector of ints and a start position on that vector.  It attempts to 
 * "solve the puzzle" by moving back and forth along the vector.  The distance of each move is determined
 * by the value of the current position in the vector.  The puzzle is solved when the last item in the 
 * vector is reached.  Function returns True if a solution exists, or False otherwise.
 * To prevent backtracking, the function logs which squares it has "stepped on" so far, and will never
 * step on the same square twice for a given solution.
 */


//wrapper function
bool Solvable(int start, Vector<int> & squares){
    Vector<bool> square_unused;
    for(int i = 0; i < squares.size(); i++){
        square_unused.add(true);
    }
    return(PuzzleSolvable(start, squares, square_unused));
}


// backtracking recursive function
bool PuzzleSolvable(int start, Vector<int> & squares, Vector<bool> square_unused){
    
// base case: the current position equals the goal position, which is the last position in the array.
    if(start == squares.size()) return true;


// recursive case: try moving left or right, unless that would exceed the 
// vector bounds, or go back to an already-stepped-on square.

// try moving to left.  start new branch if move is valid.
    int left_dest = start - squares[start-1];
    if(left_dest > 0 && square_unused[left_dest-1]) {
        square_unused[left_dest-1] = false;
        if(PuzzleSolvable(left_dest,squares, square_unused)) return true;
    }

// try moving to right. start new branch if move is valid.
    int right_dest = start + squares[start-1];
    if(right_dest <= squares.size() && square_unused[right_dest-1]) {
        square_unused[right_dest-1] = false;
        if(PuzzleSolvable(right_dest, squares, square_unused)) return true;
    }

    return false;

}
