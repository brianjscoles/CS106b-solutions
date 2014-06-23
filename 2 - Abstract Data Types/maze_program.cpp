/*
 * Project: Maze_program
 * Stanford CS106b Assignment 2.2
 * Written June 2014 by BScoles
 * Written for C++; compiled in MS Visual Studio 2005
 * 
 * This program generates and solves "perfect" mazes (which by definition have only one possible
 * path from entrance to exit, as long as no back-tracking is allowed).
 * 
 * Prompts the user for maze dimensions before generating.
 * Then generates a perfect maze using the Aldous-Broder "random walk" algorithm.
 * Then prompts the user to hit "ENTER" before it finds and displays the solution.
 * Then asks the user if they'd like to generate and solve another maze.  Continues to loop until
 * user responds with "N" or "n".
 *
 * Makes extensive use of the Stanford-provided Maze class, which stores the maze properties 
 * and handles all graphics/display calls.
 */


/*	INCLUSIONS  */ 

#include "maze.h"
#include "simpio.h"
#include <iostream>
#include "vector.h" 
#include "random.h"
#include "queue.h"
#include "stack.h"
#include "extgraph.h"


/* PRIVATE FUNCTION PROTOTYPES */

pointT getParameters();
Vector<pointT> findNeighbors(Maze m, pointT position); 
bool pointsEqual(pointT p1, pointT p2);
bool vectorContainsPoint(Vector<pointT> vec1, pointT p1);
Maze generateMaze(pointT maze_size);
Stack<pointT> solveMaze(Maze &m, pointT maze_size);
void drawSolution(Stack<pointT> path, Maze &m);



/*	MAIN PROGRAM   */

int main() {
    Randomize();
	cout << "WELCOME TO MAZEMASTER 9000!!!" <<endl;
	while(true){
		pointT maze_size = getParameters();
		cout << "Please wait while maze is generated." << endl;

		// this constructs the main Maze object, which will be
		// passed by reference to the functions that operate on it.
		Maze m = generateMaze(maze_size);							
		m.draw();
		cout << "Maze generated... hit ENTER to solve!";
		GetLine();
		Stack<pointT> solution = solveMaze(m, maze_size);
		drawSolution(solution,m);
		cout << "Whew, that was a fun one!  Want me to solve another maze, Y/N? ";
		string choice = GetLine();
		if(choice == "N" || choice == "n") {
				cout << "Okay, bye!" << endl;
                break;
		}
	}
}

/*
 * FUNCTION: getParameters
 * -------------------------
 * This function gets from the user the number of rows and cols for the maze.
 * It recommends certain value ranges and will re-prompt the user if they specify anything unacceptable.
 * It returns the values as a pointT - which is slightly overloading this type, but it is an efficient
 * way to store and retrieve two int values.
 */ 


pointT getParameters() {         
	pointT result = {0,0};
	while(result.row <= 0 || result.row > 30){
		cout << endl << "Input number of rows (recommended range 5-30): ";
		result.row = GetInteger();
	}

	while(result.col <= 0 || result.col > 50){
		cout << "Input number of columns (recommended range 5-50): ";
		result.col = GetInteger();
	}

	return result;
}

/*
 * FUNCTION: findNeighbors
 * -------------------------
 * This function finds and returns valid "neighbor" cells for a given cell.  
 * (NOTE: cells are referred to by their pointT, which specifies a row and column location.)
 * It first generates 4 possible neighbor coordinates, to the N, E, S, and W of the original,
 * and then eliminates any of those that would be outside of the maze's bounds.
 */ 

Vector<pointT> findNeighbors(Maze m, pointT position) {
	Vector<pointT> neighbors;
	for(int i = 0; i < 4; i++){
		
	// populates "neighbors" with 4 copies of the current position.  
	// it will then edit each position to generate the possible neighbor coordinates.
		neighbors.add(position);	
	}
	neighbors[0].row += 1;			// this defines the "north neighbor"
	neighbors[1].col += 1;			// east neighbor
	neighbors[2].row -= 1;			// south neighbor
	neighbors[3].col -= 1;			// west neighbor
	
	// collects only the neighbors that are within the legal bounds of the maze
	Vector<pointT> in_bounds_neighbors;		
	for(int i = 0; i < 4; i++){
		if(m.pointInBounds(neighbors[i])) {
			in_bounds_neighbors.add(neighbors[i]);
		}
	}
return in_bounds_neighbors;
}


/*
 * FUNCTION: pointsEqual
 * -------------------------
 * This helper function compares two pointT objects and returns true if they contain the 
 * same coordinates.
 */ 

bool pointsEqual(pointT p1, pointT p2){
	if (p1.row == p2.row && p1.col == p2.col) { 
		return true;
	} else { 
		return false;
	}
}


/*
 * FUNCTION: findNeighbors
 * -------------------------
 * This helper function iterates through a Vector, and uses pointsEqual to check if
 * the given pointT already exists inside that Vector.
 * This function exists because the built-in Vector::contains(elem) function is not
 * capable of comparing two pointT objects to determine equality.
 */ 

bool vectorContainsPoint(Vector<pointT> vec1, pointT p1){
	for(int i = 0; i < vec1.size(); i++){
		if(pointsEqual(vec1[i],p1)){
			return true;
		} 
	}
	return false;
}

/*
 * FUNCTION: generateMaze
 * -------------------------
 * This function builds and returns a Maze object for a given maze_size.  Uses the Aldous-Broder algorithm,
 * which "walks" randomly around the maze, and if it steps into a cell that is not already "included",
 * it knocks down the wall between that cell and the previous one, thereby including the cell.  Continues
 * until all cells in the Maze are included (can take a long time for larger mazes!).
 */ 

Maze generateMaze(pointT maze_size) {
			Maze m(maze_size.row, maze_size.col, true);									// constructs the Maze, with all walls existing.
			m.draw();																	// draws initial Maze to graphics window.
			
			Vector<pointT> is_included;													// this Vector will track which cells have been included.
			pointT current_pos = {RandomInteger(0, maze_size.row-1),RandomInteger(0, maze_size.col-1)};		// chooses a random starting point.     
			is_included.add(current_pos);
			
			while (is_included.size() < maze_size.row*maze_size.col) {
                        Vector<pointT> neighbors = findNeighbors(m, current_pos);		// get valid neighbor cells based on current position.
						pointT next_pos;
						next_pos = neighbors[RandomInteger(0, neighbors.size() - 1)];	// choose a random valid neighbor to "walk to" next
						if (!vectorContainsPoint(is_included, next_pos)) {              // if neighbor not yet included,                 
                                    is_included.add(next_pos);							// record it as included 
									m.setWall(current_pos, next_pos, false);			// and knock down wall!
                        }
                        current_pos = next_pos;											// "step into" neighbor cell, whether or not it was already included.
            }
            return m;			
}

/*
 * FUNCTION: solveMaze
 * -------------------------
 * This function solves the maze, and returns its solution as a Stack of pointTs which represent the "steps" from
 * maze entrance to maze exit.
 * It performs a breadth-first search by looking at its current point, generating all possible legal next steps from that point,
 * and enqueueing each of those as a possible partial path.
 */ 


Stack<pointT> solveMaze(Maze &m, pointT maze_size){
 Queue<Stack<pointT> > possible_paths;                      //This queue holds many possible paths, each of which is a stack of pointTs.
 
 pointT goal = {maze_size.row-1, maze_size.col-1};			//defining the "goal" cell as the top-right corner

 pointT origin = {0,0};										//defining the "origin" cell as the bottom-left corner

 Stack<pointT> path_0;										//Generates the first path, which starts at the origin.
 path_0.push(origin);										//Pushes two copies of the origin cell onto the stack to guard against an error 
 path_0.push(origin);										//in the anti-backtracking line during the very first iteration of the loop when the Stack size is 1.
															
 possible_paths.enqueue(path_0);							//Pushes our first stack into the queue!
 
 Stack<pointT> current_path;
 while(true) {
    current_path = possible_paths.dequeue();				// get the next stack from our queue
    if(pointsEqual(current_path.peek(), goal)) { break;}	// if the topmost cell in the stack is the goal cell, this path is complete! maze solved.
	pointT current_pos = current_path.pop();				// get the topmost point from our stack of points - this is the current endpoint of the path in progress.
    
	Vector<pointT> neighbors = findNeighbors(m, current_pos);	// get possible neighbor cells for the current position.

		for(int i = 0; i < neighbors.size(); i++){			// this loop generates an "offshoot" path for each valid possible next step, 
															// and then adds these offshoot paths back to the queue.

			if(!m.isWall(current_pos, neighbors[i]) &&		// checks 2 conditions for a valid move.  must be no wall present, and 
			!pointsEqual(neighbors[i],current_path.peek())) // the next step must not be the same as the previous step (to prevent backtracking).
			{
				Stack<pointT> next_path = current_path;		// makes a copy of the current path
				next_path.push(current_pos);				// puts the current position back onto the "next path" stack
				next_path.push(neighbors[i]);				// puts the newly identified valid next move onto the "next path" stack
				possible_paths.enqueue(next_path);			// puts the "next path" into the main queue.
			}
		}
 }
 return current_path;										// returns the current_path after the while loop breaks - this is the solution to the maze.
}

/*
 * FUNCTION: drawSolution
 * -------------------------
 * This function takes the solution path (a Stack of pointTs), inverts it,
 * and then asks the Maze object to display the solution cell-by-cell, with a brief pause
 * between each mark so that the user can see the "progress" from start cell to finish cell.
 */ 


void drawSolution(Stack<pointT> path, Maze &m){
	
	Stack<pointT> inverted_path;                    // inverting the stack so that the maze path will print out from start to finish.
	while(!path.isEmpty()) {
		inverted_path.push(path.pop());
	}
	
	while(!inverted_path.isEmpty()){
		m.drawMark(inverted_path.pop(), "red");		// draw a mark at each of the cells along the solution path.
		Pause(.01);									// slight Pause included so that user can see "motion" or "progress"
	}
}

