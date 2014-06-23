/*
 * Project: Recursion exercises
 * Stanford CS106b Assignment 3
 * Written June 2014 by BScoles
 * Written for C++; compiled in MS Visual Studio
 * 
 */



#include <iostream>
#include "simpio.h"
#include "lexicon.h"

/* Function Prototypes */ 


void ListCompletions(string digits, Lexicon & lex);
void examineDigits(string digits, Lexicon & lex, string so_far);
string digitToLetters(int digit);
void printValidWords(string prefix, Lexicon & lex);
string scrubDigits(string input);


int main ()
{

    // Test code for problem 4: keypad word completion.
    Lexicon lex("lexicon.dat");
    string input = "25432";
    cout << "Finding valid words that begin with the dialed prefix " << input << "!" << endl;
    ListCompletions(input,lex);

    return 0;
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