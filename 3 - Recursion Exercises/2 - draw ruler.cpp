/*
 * Project: Recursion exercises
 * Stanford CS106b Assignment 3
 * Written June 2014 by BScoles
 * Written for C++; compiled in MS Visual Studio
 * 
 */


/* Problem 2: drawing subdivisions on a ruler.
 * This function draws a ruler with smaller and smaller subdividing tickmarks.
 */ 


#include <iostream>
#include "simpio.h"
#include "graphics.h"


/* Function Prototypes */ 

void DrawRuler(double x, double y, double w, double h);
void DrawTickMark(double x, double y, double w, double h);




int main ()
{
    
    //Test code for problem 2: drawing ruler
    cout << endl << endl << "Drawing a ruler now!" << endl << endl;
    DrawRuler(.5,1,6,1);

    return 0;
}




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

