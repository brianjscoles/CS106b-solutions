/*
 * Project: Markov_model
 * Stanford CS106b Assignment 2.1
 * Written June 2014 by BScoles
 * Written for C++; compiled in MS Visual Studio 2005
 * 
 * Markov random language generator.  Prompts the user for text file, and a Markov level 1-10.
 * Then generates a random set of language based on that source file.  The higher the Markov level,
 * the closer the random language will resemble the source text.
 *
 * For all test cases I've tried, runs about 25% faster than the demo program provided by the instructor!
 * On my machine, takes about 17 seconds to process 165,000 words at Markov level 10.
 */



/* Inclusions  */ 

#include "simpio.h"
#include "genlib.h"
#include "vector.h"
#include "map.h"
#include <iostream>
#include "random.h"
#include <fstream>


/*  Global declarations  */ 

const int OUTPUT_CHARS = 2000;

// This ifstream object is instantiated globally because I was not able to resolve errors 
// that occurred when creating it locally and passing it by value or reference between functions.
ifstream infile;                


/*  Function prototypes  */ 

void getFileName();
int getLevel();
void generateModel(Map<Vector<char> > &word_map, int markov_level);
string chooseInitialSeed(Map<Vector<char> > &word_map);
void printResult(Map<Vector<char> > &word_map, string seed);


/*  MAIN PROGRAM   */ 

int main ()
{
    Randomize();

    getFileName();
    int markov_level = getLevel();
    
    // constructs the main Map object, with an estimated initial 
    // size to aid system memory allocation (higher Markov levels 
    // require much larger Maps)
    Map<Vector<char> > word_map(50000*markov_level - 45000);    
    
    cout << "Please stand by, generating " << OUTPUT_CHARS << " characters of text";
    cout << " at Markov level " << markov_level << "." << endl;
    
    generateModel(word_map, markov_level);
    string seed = chooseInitialSeed(word_map);
    printResult(word_map, seed);
    
    cout << endl;
    return 0;
}

/*
* FUNCTION: getFileName()
* ----------------------
* Gets a filename from the user and attempts to open it.
* Will re-prompt the user until a valid filename is provided.
* Opens that file into the global ifstream object "infile".
*/

void getFileName(){
    while(true){
        cout << "Please input a file name: ";
        string filename = GetLine();
        infile.open(filename.c_str());
        if(infile.fail()){
            cout <<"Could not open file. Please try again." << endl;
            infile.clear();
        } else { 
            break; 
        }
    }
}

/*
* FUNCTION: getLevel()
* ----------------------
* Gets a Markov level from the user. Re-prompts the user until a valid 
* integer between 1-10 is provided, and then returns that int.
*/

int getLevel(){
    cout << "Please input a Markov level, 1-10: ";
    int markov_level = GetInteger();
    while(markov_level <= 0 || markov_level > 10){
        cout << "Sorry, evel must be between 1 and 10: ";
        markov_level = GetInteger();
    }
    return markov_level;
}

/*
* FUNCTION: generateModel()
* ----------------------
* Reads the source text file and fills in a Map object with a Markov model of the language in the source file.
* The Map object, named "word_map", contains key-value pairs.  
*   The keys are "seed strings", which are sequential strings pulled from the source file. Length of these strings
*     equals the given markov_level, e.g. if markov_level == 5, all seed strings will be of length 5.
*   The values are Vectors containing characters.  Every time a given seed string occurs in the source file,
*     the character that immediately follows that string will be added to the Vector for that seed string.  So 
*     the Vector contains one character for each occurence of the seed string in the source file.  Many of 
*     the characters are often duplicates, e.g. the seed string "begin" will probably be followed by many instances of " " and "n".
*     This is a simple way to create a probability map of which characters tend to follow each seed string.
*/

void generateModel(Map<Vector<char> > &word_map, int markov_level){
    char next_char;
    string seed;
    
    // generates the first possible seed string
    for(int i = 0; i<markov_level;i++){     
        if(infile.fail()) {
            cout << "Critical error.  Specified file is too short to process." << endl;
            break;
        }
        seed += infile.get();
    }


    while (true){
        next_char = infile.get();
        if(infile.fail()) break;            

        // adds the seed to the Map.  If a matching key does not already exist, the compiler defaults to
        // adding the seed as a new key, default-constructing a Vector, and adding next_char to that Vector.
        word_map[seed].add(next_char);      

        // "advances" the seed by one character in preparation for next iteration of the loop.
        seed = seed.substr(1) + next_char;  
    }
}

/*
* FUNCTION: chooseInitialSeed()
* ----------------------
* Iterates through every key in the Map object, and returns the most frequent key,
* which is the key that has the longest Vector as its value. We do this because our 
* instructions are to initiate the language generation with the most common seed.
*/

string chooseInitialSeed(Map<Vector<char> > &word_map){
    string most_frequent_seed;
    int max_frequency = 0;
    Map<Vector<char> >::Iterator itr = word_map.iterator();
    while(itr.hasNext()){
        string key = itr.next();
        if(word_map[key].size() > max_frequency){
            max_frequency = word_map[key].size();
            most_frequent_seed = key;
        }
    }
    return most_frequent_seed;
}

/*
* FUNCTION: printResult()
* ----------------------
* Generates and prints sequence of random language based on the model stored in the Map object.
*/

void printResult(Map<Vector<char> > &word_map, string seed){
    
    cout << seed;                                           
    string next_char;
    for(int i = 0; i < OUTPUT_CHARS; i++){
        if(!word_map.containsKey(seed)) break;              // Terminates the loop early if seed string not found

        next_char = word_map[seed][RandomInteger(0,word_map[seed].size()-1)];   // Grabs the next character from the Map object, randomly choosing from
                                                                                // the Vector associated with the present seed string.
        
        cout << next_char;                                                      // prints the chosen next character
        
        seed = seed.substr(1) + next_char;                  // "advances" the seed by one character in preparation for next iteration of loop.
    }
    cout << endl;
}
