/*
 * Project: Recursion exercises
 * Stanford CS106b Assignment 3
 * Written June 2014 by BScoles
 * Written for C++; compiled in MS Visual Studio
 * 
 * My solutions to 6 recursion problems.  The prototype and desired output of each function was provided;
 * My assignment was to build the function and any helper functions that are required.  Many of the solutions
 * start with a wrapper function that calls a separate recursive function to do most of the heavy lifting.
 *
 * The "main" function contains test code that passes various inputs to the 6 functions.  All results should be 
 * easily readable as output in the console window.
 */



#include <iostream>
#include "simpio.h"
#include "vector.h"
#include "graphics.h"
#include <cmath>
#include "lexicon.h"

/* Function Prototypes */ 

// Prototypes for warmup 1 
void PrintInBinary(int number);

// Prototypes for warmup 2
bool SumSubGroup(Vector<int> nums, int targetSum, int sum);
bool CanMakeSum(Vector<int> & nums, int targetSum);

// Prototypes for problem 1
int CountWays(int numStairs);
void climbStairs(int numstairs, int &ways_to_climb);

// Prototypes for problem 2
void DrawRuler(double x, double y, double w, double h);
void DrawTickMark(double x, double y, double w, double h);

// Prototypes for problem 3
int CountCriticalVotes(Vector<int> & blocks, int blockIndex);
void findCriticalOutcomes(Vector<int> blocks, int keyblock, int Alice, int Bob, int &critical_outcomes, int &total_outcomes);

// Prototypes for problem 4
void ListCompletions(string digits, Lexicon & lex);
void examineDigits(string digits, Lexicon & lex, string so_far);
string digitToLetters(int digit);
void printValidWords(string prefix, Lexicon & lex);
string scrubDigits(string input);

// Prototypes for problem 5
bool Solvable(int start, Vector<int> & squares);
bool PuzzleSolvable(int start, Vector<int> & squares, Vector<bool> square_unused);

// Prototypes for problem 6
int CutStock(Vector<int> & requests, int stockLength);
void PlacePipe(Vector<int> requests, Vector<Vector<int> > assignments, Vector<Vector<Vector<int> > > & solutions, int stockLength);
bool pieceFits(int request, Vector<int> assignment, int stockLength);
Vector<Vector<int> > addPiece(int request, int i, Vector<Vector<int> > assignments);
int printSolution(Vector<Vector<Vector<int> > > & solutions);
bool bad_request(Vector<int> & requests, int stockLength);

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

    //Test code for problem 1: stair climbing
    cout << "Problem 1: ways to climb stairs." << endl << endl;
    for(int i = -3; i <= 10; i++){
        cout << "There are " << CountWays(i) << " ways to climb a set of " << i << " stairs." << endl;
    }
    
    
    //Test code for problem 2: drawing ruler
    cout << endl << endl << "Drawing a ruler now!" << endl << endl;
    DrawRuler(.5,1,6,1);

    
    //Test code for problem 3: election voting blocks
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
    cout << endl << endl;

    // Test code for problem 4: keypad word completion.
    Lexicon lex("lexicon.dat");
    string input = "25432";
    cout << "Finding valid words that begin with the dialed prefix!" << endl;
    ListCompletions(input,lex);
    cout << endl << endl << endl;
    

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
    cout << endl << endl;

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

    for(int i = 0; i < nums.size(); i++){
        int subsum = sum + nums[i];
        if(subsum==targetSum) return true;
        Vector<int> newnums = nums;
        newnums.removeAt(i);
        if(SumSubGroup(newnums, targetSum, subsum)) return true;
    }
    return false;
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

/* Problem 2: drawing near-infinite subdivisions on a ruler.
 * This function draws a ruler with a large number of subdividing tickmarks.
 */ 

// wrapper function
void DrawRuler(double x, double y, double w, double h){
    InitGraphics();
    MovePen(x,y);
    DrawLine(w,0);
    DrawTickMark(x, y, w, h);
}

// recursive function
void DrawTickMark(double x, double y, double w, double h){
        MovePen(x+w/2,y);
        DrawLine(0,h);
        if(w > .1){
            DrawTickMark(x,y,w/2,h/2);
            DrawTickMark(x+w/2,y,w/2,h/2);
        }
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


/* Problem 4: cell phone keypad word lookup.
 * The function simulates a T9-type keypad word completion algorithm.
 * It accepts a string of digits, generates a list of possible letter strings ("prefixes") based on
 * the layout of a standard alphanumeric keypad (though including Q on keypad 7 and Z on keypad 9),
 * and prints a list of valid english words that start with those prefixes.
 * The function will ignore any characters not in the valid digit range of 2 - 9 inclusive.
 */

// wrapper function
void ListCompletions(string digits, Lexicon & lex) {
    string clean_digits = scrubDigits(digits);
    examineDigits(clean_digits, lex, "");
}

// removes any characters from the string that are not in the valid range.
string scrubDigits(string input){
    string valid_digits = "23456789";
    string output;
    for(int i = 0; i < input.length(); i++){
        if(valid_digits.find(input.at(i)) != string::npos) output += input.at(i);
    }
    return output;
}

// recursive function
void examineDigits(string digits, Lexicon & lex, string so_far){
    
//base case: once string is completed, send it to the word-checker, which will print
//out any valid words starting with this prefix.
    if(digits.length()==0){
        printValidWords(so_far, lex);

// recursive case: convert next digit in the string to its possible letter equivalents,
// and feed each possible case back into the recursive loop.
    }else {
        string letters = digitToLetters(int(digits.at(0)));
        for (int j = 0; j < letters.length(); j++){
            examineDigits(digits.substr(1), lex, so_far + letters.at(j));
        };
    }
}


// returns the possible letter values for a given keypad digit. 
string digitToLetters(int digit){
    switch(digit){
        case '2': return "abc";
        case '3': return "def";
        case '4': return "ghi";
        case '5': return "jkl";
        case '6': return "mno";
        case '7': return "pqrs";
        case '8': return "tuv";
        case '9': return "wxyz";
        default: 
            return "";
    }
}

// checks lexicon and prints all words that begin with the given prefix
void printValidWords(string prefix, Lexicon &lex){
    string alpha = "abcdefghijklmnopqrstuvwxyz";
    if(lex.containsWord(prefix)) cout << prefix << " ";

    if(lex.containsPrefix(prefix)){
        for(int i = 0; i < alpha.length(); i++){
            printValidWords(prefix + alpha.at(i), lex);
        } 
    }
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

/* Problem 6: Pipe cutting.
 * This function accepts a Vector of ints that represents a set of desired pipe lengths, and an
 * int that represents the length of the stock pipes.  It returns the minimum number of stock pipes
 * required to obtain the desired set of pipe pieces, based on how the stock pieces might be cut up.
 */


// wrapper and setup function.
int CutStock(Vector<int> & requests, int stockLength) {
    if(bad_request(requests, stockLength)){
        cout << "Error - requested piece(s) larger than available stock length.";
        return 0;
    }
    Vector<Vector<Vector<int> > > solutions;
    Vector<Vector<int> > assignments;
    PlacePipe(requests,assignments,solutions,stockLength);
    int min = printSolution(solutions);
    return min;
}

bool bad_request(Vector<int> & requests, int stockLength){
    for(int i = 0; i< requests.size(); i++){
        if(requests[i] > stockLength) return true;
    }
    return false;
}


// identifies and prints optimal solution.  returns # of stock pieces required in optimal solution.
int printSolution(Vector<Vector<Vector<int> > > & solutions){
    int min = 99999;
    int index;
    for(int i = 0; i<solutions.size(); i++){
        if(solutions[i].size() < min){
            min = solutions[i].size();
            index = i;
        }
    }
    cout << "The optimal solution uses " << min << " pieces." << endl;
    for(int j = 0; j < solutions[index].size(); j++){
        cout << "Piece " << j+1 << ": ";
        for(int k = 0; k < solutions[index][j].size(); k++){
            cout << solutions[index][j][k] << "  ";
        }
        cout << endl;
    }
    return min;
}
    


// recursive function
void PlacePipe(Vector<int> requests, Vector<Vector<int> > assignments, Vector<Vector<Vector<int> > > & solutions, int stockLength){

// base case.  if all requested pieces have been assigned, add this possible solution to the vector of solutions.
    if(requests.size() == 0){
        solutions.add(assignments);
    } else {

// recursive case.
        int request = requests[0];
        requests.removeAt(0);

// tries adding the next requested piece onto each of the existing stock pipes in use.  generates a new
// branch for each such valid addition.
        for(int i = 0; i < assignments.size(); i++){
            if(pieceFits(request,assignments[i],stockLength)){
                PlacePipe(requests,addPiece(request,i,assignments), solutions, stockLength);
            }
        }

// generates a new branch in which the requested piece goes onto a new stock pipe of its own.
        PlacePipe(requests,addPiece(request,assignments.size(),assignments), solutions, stockLength);

    }
}


// checks whether a given requested piece would fit on the remaining length of a given stock piece.
bool pieceFits(int request, Vector<int> assignment, int stockLength){
    int sum_so_far = 0;
    for(int i = 0; i < assignment.size(); i++){
        sum_so_far += assignment[i];
    }
    if(request + sum_so_far <= stockLength) return true;
    else return false;
}


// this function modifies and returns the "assignments" 2-dimensional vector.  it will add
// the requested piece onto the specified stock piece, or put the request onto a new stock piece,
// depending on the index given.
Vector<Vector<int> > addPiece(int request, int i, Vector<Vector<int> > assignments){
    if(i == assignments.size()){
        Vector<int> empty_vec;
        assignments.add(empty_vec);
        assignments[i].add(request);
    } else {
    assignments[i].add(request);
    }
    return assignments;
}