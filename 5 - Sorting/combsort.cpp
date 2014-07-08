/*
 * Project: 5B - generic sort
 * Created by CS106 C++ Assignment Wizard 0.1
 *
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * [TODO: Describe assignment]
 */

#include <iostream>
#include "Vector.h"
#include "Random.h"
#include "Set.h"
#include <ctime>

/* Function prototypes */ 

void PrintVectorInt(Vector<int> arr);
void PrintVectorSetInt(Vector<Set<int> > arr);
void PrintVectorString(Vector<string> arr);
Vector<int> RandomArray(int size, int min, int max);
string RandomString(Vector<char> &chars, int size);
int SetSum(Set<int> set1);
int SetIntCmp(Set<int> set1, Set<int> set2);
int StringCmp(string string1, string string2);



/* Template functions */


/* Template: Swap
 * This performs an in-place swap of two elements that were passed by reference.
 */

template <typename Type>    
    void Swap(Type &a, Type &b){
        Type temp = a;
        a = b;
        b = temp;
    }

/* Template: CombSort
 * This sorts a vector of any type using the CombSort algorithm.  Accepts an optional 
 * callback function to replace the default comparison function.
 */

template <typename Type>
    void CombSort(Vector<Type> &nums, int (cmp)(Type one, Type two) = OperatorCmp){
        cout << "Sorting...";
        clock_t start = clock();
        bool sorted = false;
        double shrink_factor = 1.3;
        int gap = nums.size()/shrink_factor;

        while(true){
            sorted = true;
            int pos = 0;
            while (pos + gap < nums.size()){
                if(cmp(nums[pos],nums[pos+gap])>0){
                    Swap(nums[pos],nums[pos+gap]);
                    sorted = false;
                }
                pos++;
            }
            if(gap > 1){
                gap--;
            }else if(sorted){
                double runtime = (clock() - start);
                cout << "finished in " << runtime << " milliseconds." << endl << endl;
                break;
            }
        }
    }




/* 
 * Main Program
 * Contains some test code to put our CombSort through its paces.
 */ 

int main ()
{
    Randomize();
    
    /* Test code for sorting a vector of ints */ 

    Vector<int> testarr = RandomArray(250,0,500);
    cout << "Testing our CombSort on a vector of random integers..." << endl << endl;
    cout << "Here is the unsorted vector of integers:" << endl << endl;
    PrintVectorInt(testarr);
    CombSort(testarr);
    cout << "Here is the sorted version" << endl << endl;
    PrintVectorInt(testarr);
    

    /* Test code for sorting a vector of sets of ints. */ 

    // Populating a vector with randomized sets.
    Vector<Set<int> > testsets;
    for(int i = 0; i < 40; i++){
        Set<int> newset;
        Vector<int> nums = RandomArray(RandomInteger(4,12),0,50);
        for(int i = 0; i < nums.size(); i++){
            newset.add(nums[i]);
        }
        testsets.add(newset);
    }

    // Putting the vector of sets through the sort!
    cout << "Testing our CombSort on a vector of sets of integers." << endl << endl;
    cout << "Here is the unsorted vector of sets:" << endl << endl;
    PrintVectorSetInt(testsets);
    CombSort(testsets,SetIntCmp);
    cout << "Here is the sorted version." << endl << endl;
    PrintVectorSetInt(testsets);
   

    /* Test code for sorting strings */

    //building a list of valid characters for random strings. alphabetical + spaces.
    Vector<char> chars;
    for(int i = 65; i < 91; i++){
        chars.add((char)i);
    }
    for(int i = 97; i < 123; i++){
        chars.add((char)i);
    }  
    chars.add(' ');

    // populating a vector with some random strings
    Vector<string> teststrings;
    for(int i = 0; i < 50; i++){
        teststrings.add(RandomString(chars,RandomInteger(3,10)));
    }
    
    // running the vector through the sort algorithm.
    cout << "Testing our CombSort on a vector of strings..." << endl << endl;
    cout << "Here is the unsorted vector of strings:" << endl << endl;
    PrintVectorString(teststrings);
    CombSort(teststrings,StringCmp);
    cout << "Here is the sorted version." << endl << endl;
    PrintVectorString(teststrings);


	return 0;
}


/* 
 * Function: PrintVectorInt
 * Prints a vector of ints to the console in a slick readable format.
 */ 

void PrintVectorInt(Vector<int> arr){
    for(int i = 0; i < arr.size(); i++){
        cout << arr[i] << " ";
    }
    cout << endl << endl << endl;
}


/* 
 * Function: PrintVectorSetInt
 * Prints a vector of sets of ints to the console in a slick readable format.
 * Puts each set on a separate line.
 */ 

void PrintVectorSetInt(Vector<Set<int> > arr){

    for(int i = 0; i < arr.size(); i++){
        Set<int>::Iterator iter = arr[i].iterator();
        cout << "[";
        while(iter.hasNext()){
           cout << iter.next();
           if(iter.hasNext()) cout << ", ";
        }
        cout <<  "]    (Sum=" << SetSum(arr[i]) << ")" << endl;
    }
    cout << endl << endl << endl;
}


/* 
 * Function: PrintVectorString
 * Prints a vector of strings to the console in a slick readable format.
 * Each string gets its own line.
 */ 

void PrintVectorString(Vector<string> arr){
    for(int i = 0; i < arr.size(); i++){
        cout << arr[i] << endl;
    }
    cout << endl << endl << endl;
}


/* 
 * Function: RandomArray
 * Returns a Vector populated with random ints, based on the parameters passed to it.
 */ 
            
Vector<int> RandomArray(int size, int min, int max){
    Vector<int> arr(size);
    for(int i = 0; i < size; i++){
        arr.add(RandomInteger(min,max));
    }
    return arr;
}


/* 
 * Function: RandomString
 * Returns a randomly generated string, based on the parameters passed to it.
 */ 
            
string RandomString(Vector<char> &chars, int size){
    string output = "";
    for(int i = 0; i < size; i++){
        output+= chars[RandomInteger(0,chars.size()-1)];
    }
    return output;
}
    


/* 
 * Function: SetSum
 * Returns the sum of all integers in a given Set of integers.
 */ 

int SetSum(Set<int> set1){
    int sum = 0;
    Set<int>::Iterator iter = set1.iterator();
    while(iter.hasNext()){
        sum += iter.next();
    }
    return sum;
}
 

/* 
 * Function: SetIntCmp
 * A comparison function for Sets of integers.
 * Returns 1 if the first set has a larger sum,
 * returns -1 if the second set has a larger sum,
 * returns 0 if they are equal or if something strange happened.
 */ 

int SetIntCmp(Set<int> set1, Set<int> set2){
    
    int sum1 = SetSum(set1);
    int sum2 = SetSum(set2);

    if(sum1 == sum2) return 0;
    else if(sum1 < sum2) return -1;
    else if(sum1 > sum2) return 1;
    else return 0;
}


/* 
 * Function: StringCmp
 * A comparison function for strings.
 * Checks to see if string1 is longer than string2.
 * If they are equal length, compares them alphabetically.
 * Returns 0 if strings are identical.
 */ 

int StringCmp(string string1, string string2){
    if(string1.length() > string2.length()) return 1;
    if(string1.length() < string2.length()) return -1;
    else {
        for(int i = 0; i < string1.length(); i++){
            char char1 = tolower(string1.at(i));
            char char2 = tolower(string2.at(i));
            if(char1 > char2) return 1;
            if(char1 < char2) return -1;
        }
        return 0;
    }
}
