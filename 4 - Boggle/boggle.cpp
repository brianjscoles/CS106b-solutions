/* File: boggle.cpp
 * ----------------
 * Your name here!
 */
 
#include "genlib.h"
#include "simpio.h"
#include <iostream>
#include "extgraph.h"
#include "lexicon.h"
#include "Gboggle.h"
#include "Random.h"
#include "Vector.h"
#include "Grid.h"
#include "Map.h"
#include "Set.h"


/*
 * Type: pointT
 * ------------
 * The type pointT is used to encapsulate a pair of integer
 * coordinates into a single value with row and col components.
 */

struct pointT { 
	int row; 
	int col; 
};

/* Function Prototypes */

void NewGameLoop();
void PlayGame(int size, string letters);
string GenerateRandomRoll(int size);
void PlayGame(int size, string letters);
Grid<char> SetupBoard(int size, string letters);
void FindWords(Vector<pointT> points, Lexicon lex, Grid<char> &board, Map<Vector<pointT> > &all_words);
string WordFromPoints(Vector<pointT> &points, Grid<char> &board);
bool VectorContainsPoint(Vector<pointT> points, pointT point);
bool PointInBounds(pointT point, Grid<char> board);
Vector<pointT>  FindValidOptions(Vector<pointT> &points, Grid<char> &board);
void FindWordsWrapper(Lexicon lex, Grid<char> &board, Map<Vector<pointT> > &all_words);
void DoHumanTurn(Grid<char> board, Set<string> &humwords, Map<Vector<pointT> > &all_words);
void CheckWord(string word, Grid<char> &board, Set<string> &humwords);
void SearchBoardForWord(Vector<pointT> points, string target, Grid<char> &board, Set<string> &humwords);
void HighlightWord(Vector<pointT> points, double pause, bool animate);
string StringToUpper(string input);
void DoComputerTurn(Grid<char>board, Map<Vector<pointT> > &all_words, Set<string> &humwords);
void GiveInstructions();
void Welcome();


/* Main Program */ 

int main()
{
	Randomize();
	Welcome();
	GiveInstructions();
        NewGameLoop();
	return 0;
}


void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
	 << "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
	 << "creating a grid of letters on which you try to form words. You go first, " 
	 << "entering all the words you can find that are formed by tracing adjoining " 
	 << "letters. Two letters adjoin if they are next to each other horizontally, " 
	 << "vertically, or diagonally. A letter can only be used once in the word. Words "
	 << "must be at least 4 letters long and can only be counted once. You score points "
	 << "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
	 << "points, and so on. After your puny brain is exhausted, I, the super computer, "
	 << "will find all the remaining words and double or triple your paltry score." << endl;
	
    cout << "\nHit return when you're ready...";
    GetLine();
}

void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
	 << "The good news is that you might improve your vocabulary a bit.  The "
	 << "bad news is that you're probably going to lose miserably to this little "
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/* Function: NewGameLoop
 * ----------------------
 * This function gets game parameters from the the user and then initializes a new Boggle game.
 * It will loop (starting new games) until the user declines to start a fresh game.
 */ 

void NewGameLoop(){
    while(true){
        cout << "Please say 'standard' for a 4x4 game, or 'big' for a 5x5 game." <<endl << "Or press ENTER to quit: ";
        string choice = GetLine();
        if(choice == "") break;
        while(choice != "standard" && choice != "big"){
            cout << "Sorry, I did not understand that. Please say 'standard' or 'big': ";
            choice = GetLine();
        }
        int board_size = 4;
        if(choice == "big") board_size = 5;
        
        cout << "Enter 'random' to generate new random board.  Or, for a custom board," 
            << endl << "enter a set of " << board_size*board_size << " letters: ";
        string board_choice = GetLine();
            while(board_choice != "random" && board_choice.length() != board_size*board_size){
                cout << "Sorry, bad input.  Please try again." << endl;
                board_choice = GetLine();
            }
        if(board_choice == "random"){
            PlayGame(board_size,GenerateRandomRoll(board_size));
        } else if(board_choice.length() == board_size*board_size) {
            PlayGame(board_size,StringToUpper(board_choice));
        }
    }

    cout << "Goodbye!";
}


/* Function: PlayGame
 * ----------------------
 * This function runs through one iteration of a Boggle game.
 * It also initializes the main objects: the board, the lexicon, the map of all valid
 * words on the board, and the set of words found by the human player.  These objects are 
 * then passed by reference from here to many other functions.
 */ 
void PlayGame(int size, string letters){
    cout << "Stand by, shaking the cubes..." << endl;
    Grid<char> board = SetupBoard(size, letters);
    Map<Vector<pointT> > all_words;
    Lexicon lex("lexicon.dat");
    FindWordsWrapper(lex, board, all_words);
    Set<string> humwords;
    DoHumanTurn(board, humwords, all_words);
    DoComputerTurn(board, all_words, humwords);
}

/* Function: GenerateRandomRoll
 * ----------------------
 * Returns a string of letters (length 16 or 25 depending on the size parameter) by randomly 
 * arranging and then randomly "rolling" the standard boggle cubes.
 */ 
string GenerateRandomRoll(int size){
   
   string standard_cubes[16]=  
        {"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
         "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};
   string big_boggle_cubes[25] = 
       {"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
        "BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
        "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};

   // feeds the array of boggle cubes into a Vector object for more control. 
   Vector<string> boggle_cubes(25);
    if(size==4){
        for(int i = 0; i < 16; i++){
            boggle_cubes.add(standard_cubes[i]);
        }
    } else if(size==5){
        for(int i = 0; i < 25; i++){
            boggle_cubes.add(big_boggle_cubes[i]);
        }
    }

    // shuffles the boggle cubes so they are in a random order
    string temp;
    for(int i = 0; i < boggle_cubes.size(); i++){
        int randomcube = RandomInteger(0,size*size-1);
        temp = boggle_cubes[i];
        boggle_cubes[i] = boggle_cubes[randomcube];
        boggle_cubes[randomcube] = temp;
    }

    // chooses a random side from each cube and adds it to the output string.
    string letters;
    for(int i = 0; i < size*size; i++){
        letters += boggle_cubes[i][RandomInteger(0,5)];
    }
    return letters;
}


/* Function: SetupBoard
 * ----------------------
 * Draws the initial game board, and displays the letter on each cube.
 * Also populates the "board" object, which is a Grid<char> that mirrors 
 * the visible Board object in the graphics display.
 */ 
Grid<char> SetupBoard(int size, string letters){
    SetWindowSize(9, 5);
	InitGraphics();
    DrawBoard(size, size);
    Grid<char> board(size,size);
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            LabelCube(row,col,letters[row*size+col]);
            board.setAt(row,col,letters[row*size+col]);
        }
    }
    return board;
}

/* Function: FindWordsWrapper
 * ----------------------
 * This is the wrapper function for the recursive function that finds all valid words
 * on the boggle board. It makes either 16 or 25 initial calls to the recursive function,
 * one for each cube on the board, as possible start locations for making a word.
 */ 
void FindWordsWrapper(Lexicon lex, Grid<char> &board, Map<Vector<pointT> > &all_words){
    Vector<pointT> points;
    pointT point;
    for(int row = 0; row < board.numRows(); row++){
        for(int col = 0; col < board.numCols(); col++){
            point.row = row;
            point.col = col;
            points.add(point);
            FindWords(points, lex, board, all_words);
            points.clear();
        }
    }
}

/* Function: FindWords
 * ----------------------
 * This is the recursive function that finds all valid words on the boggle board. 
 * It searches exhaustively, and every time it finds a valid word, it records that
 * to the all_words Map (where the key is the word, and the value is a Vector of pointTs
 * giving the location of cubes used to spell the word.  
 */ 
void FindWords(Vector<pointT> points, Lexicon lex, Grid<char> &board, Map<Vector<pointT> > &all_words){
    string word = WordFromPoints(points, board);
    if(lex.containsWord(word) && word.length() > 3 && !all_words.containsKey(word)){
        all_words[word] = points;
    }

    //begins new branches of the search, after checking that the word so far is a valid prefix in the 
    //lexicon (and thus not a dead end).
    if(lex.containsPrefix(word)) {
        Vector<pointT> next_letter_options = FindValidOptions(points, board);
        for(int i = 0; i < next_letter_options.size(); i++){
            points.add(next_letter_options[i]);
            FindWords(points, lex, board, all_words);
            points.removeAt(points.size()-1);
        }
    }
}

/* Function: FindValidOptions
 * ----------------------
 * This function takes a Vector of pointTs used so far, and returns a Vector of pointTs
 * that would be a valid choice for the next letter in the word.  It generates a set of
 * 9 possible options (a grid surrounding and including the last pointT in the Vector so 
 * far), and then removes any that would violate the bounds of the board, or that have
 * already been used in this word.
 */ 
Vector<pointT>  FindValidOptions(Vector<pointT> &points, Grid<char> &board){
    
    //generate vector of 9 surrounding cells
    Vector<pointT> neighbors;
    pointT point;
    for(int row = -1; row < 2; row ++){
        for(int col = -1; col < 2; col++){
            point.row = points[points.size()-1].row + row;
            point.col = points[points.size()-1].col + col;
            neighbors.add(point);
        }
    }

    //now "leave behind" any pointTs that violate board boundaries, or have already been used
    Vector<pointT> valid_neighbors;
    for(int i = 0; i < 9; i++){
        if(PointInBounds(neighbors[i], board) && !VectorContainsPoint(points, neighbors[i])){
            valid_neighbors.add(neighbors[i]);
        }
    }
    return valid_neighbors;
}

/* Function: DoHumanTurn
 * ----------------------
 * This function gets input from the user, checks if a word guess is valid, and records it
 * for their score if so.  Ends once the user submits an empty string.
 */ 
void DoHumanTurn(Grid<char> board, Set<string> &humwords, Map<Vector<pointT> > &all_words){
    cout << "Welcome to your turn!" << endl;
    while(true) {
        cout << "Input word, or ENTER to end turn: ";
        string word = GetLine();
        if(word == "") break;
        word = StringToUpper(word);
        if(word.length() < 4) cout << "Word is too short!" << endl;
        else if(humwords.contains(word)) cout << "You already got that word!" << endl;
        else if(all_words.containsKey(word)){
            HighlightWord(all_words[word], .4, true);
            humwords.add(word);
            RecordWordForPlayer(word,Human);
            cout << "Good find!" << endl;
        }else { cout << "Invalid guess, sorry!" << endl;
        }
    }
}

/* Function: DoComputerTurn
 * ----------------------
 * This function iterates through the all_words Map that was already generated,
 * and adds to the computer's score any words not found by the human player.
 * Highlights each word very briefly.
 */ 
void DoComputerTurn(Grid<char>board, Map<Vector<pointT> > &all_words, Set<string> &humwords){
    cout << "Now it's my turn, mwa ha ha!" << endl;
    Map<Vector<pointT> >::Iterator iter = all_words.iterator();
    while (iter.hasNext()) {
       string key = iter.next();
       if(!humwords.contains(key)){
            HighlightWord(all_words[key], .025, false);
            RecordWordForPlayer(key,Computer);
       }
    }
}

/* Function: WordFromPoints
 * ----------------------
 * This function loops up the given set of pointTs, and returns the string that those
 * points represent given the current Boggle board.
 */ 
string WordFromPoints(Vector<pointT> &points, Grid<char> &board){
    string word;
    for(int i = 0; i < points.size(); i++){
        word += board(points[i].row,points[i].col);
    }
    return word;
}

/* Function: DoHumanTurn
 * ----------------------
 * Returns false if the given point would be outside the bounds of the board.
 * Returns true if the point is valid.
 */ 
bool PointInBounds(pointT point, Grid<char> board){
    if (point.row >= 0 &&
        point.row < board.numRows() &&
        point.col >= 0 &&
        point.col < board.numCols() ){
            return true;
    }
    return false;
}

/* Function: VectorContainsPoint
 * ----------------------
 * Returns true if a given vector of pointTs already contains the given pointT.
 * Used by FindWords to check if a pointT has already been used in spelling the current word.
 */ 
bool VectorContainsPoint(Vector<pointT> points, pointT point){
	for(int i = 0; i < points.size(); i++){
		if(points[i].row == point.row &&
           points[i].col == point.col){
			return true;
		} 
	}
	return false;
}

/* Function: HighlightWord
 * ----------------------
 * Highlights the specified word on the board for the specified amount of time.
 * If the "animate" argument is true, the letters will light up in sequence to
 * represent the sequential spelling of the word.
 */ 
void HighlightWord(Vector<pointT> points, double pause, bool animate){
    for(int i = 0; i < points.size(); i++){
        HighlightCube(points[i].row, points[i].col, true);
        if(animate) Pause(pause/4);
    }
    Pause(pause);
        for(int i = 0; i < points.size(); i++){
        HighlightCube(points[i].row, points[i].col, false);
    }

}

/* Function: StringToUpper
 * ----------------------
 * Returns the uppercase version of a string.
 */ 
string StringToUpper(string input){
    for(int i = 0; i < input.length(); i++){
        input[i] = toupper(input[i]);
    }
    return input;
}

