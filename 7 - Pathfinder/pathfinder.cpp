/* FILE: pathfinder.cpp
 * ---------------
 * Written August 2014 by Brian Scoles, for C++
 * This program identifies shortest-distance paths and minimal spanning trees for
 * fully-connected graphs that are supplied in properly formatted .txt files.
 * 
 * VERSION-SPECIFIC NOTES:
 * as of 8-19-2014, I am uploading this version because it works and I'm ready to move on!
 * Performance-wise, this build is quite stable, but not all of the code is pretty.
 * Notably, the readFile(), findPath(), findMinimalTree(), and main() functions need better decomposition.
 * Also, better unification is needed for the various calls to updating the graphics window 
 * by redrawing the background image and/or arcs and/or nodes.
 */
 
#include "genlib.h"
#include "extgraph.h"
#include <iostream>
#include <cmath>
#include "Vector.h"
#include "pqueue.h"
#include "cmpfn.h"
#include "simpio.h"
#include <fstream>
#include "map.h"
#include "stack.h"
#include "set.h"


/* Constants
 * --------
 * A few program-wide constants concerning the graphical display.
 */
const double CircleRadius =.05;     	// the radius of a node
const int LabelFontSize = 9;          // for node name labels


/* Type: coordT
 * ------------
 * Just a simple struct to handle a pair of x,y coordinate values.
 */
struct coordT {
	double x, y;
};

//forward reference 
struct arcT; 

/* Type: nodeT
 * ------------
 * A node on the graph.  Contains its own location and name, a vector
 * of pointers to its outgoing arcs, and a set of the nodes that it is
 * connected to (stored as strings - only used in findMinimalTree function).
 */
struct nodeT {
    coordT location;
    Vector<arcT*> arcs;
    Set<string> connections;
    string name;
};

/* Type: nodeT
 * ------------
 * An arc on the graph.  Contains its own distance, and pointers to the 
 * two nodes that it connects.
 */
struct arcT {
    nodeT * end1, * end2;
    double distance;
};

/* Function Prototypes */

void DrawFilledCircleWithLabel(coordT center, string color, string label = "");
void DrawLineBetween(coordT start, coordT end, string color);
coordT GetMouseClick();
bool WithinDistance(coordT pt1, coordT pt2, double maxDistance = CircleRadius*2);
double pathDistance(Stack<arcT*> path);
int pathCmp(Stack<arcT*> path1, Stack<arcT*> path2);
int arcPtrCmp(arcT * a, arcT * b);
string getNextWord(int & pos, string line);
string readFile(string fileName, Map<nodeT*> & nodes, Vector<arcT*> & arcs);
string getChoiceFromUser();
void printNodes(Map<nodeT*> nodes);
void printArcs(Vector<arcT*> & arcs, string color = "black", double pauseTime = 0);
nodeT* getNodeFromClick(Map<nodeT*> nodes);
void printPath( Stack<arcT *> arcs);
void enqueueArcs(nodeT* node, Stack<arcT*> & path, PQueue<Stack<arcT *> > & paths);
void findPath(nodeT *startNode, nodeT *endNode);
void findMinimalTree(Map<nodeT*> & nodes, Vector<arcT*> & arcs, string mapImage);
 

/* Main Function
 * This is longer than would be ideal, but it should be quite readable nonetheless. 
*/ 

int main() {
    InitGraphics();
    Map<nodeT*> nodes;
    Vector<arcT*> arcs;
	SetWindowTitle("CS106 Pathfinder");
    cout << "This masterful piece of work is a graph extravaganza!" << endl
        << "The main attractions include a lovely visual presentation of the graph" << endl
        << "along with an implementation of Dijkstra's shortest path algorithm and" << endl
        << "the computation of a minimal spanning tree.  Enjoy!" << endl;

    string mapImage;
    while(true){ 
        string choice = getChoiceFromUser();
        
        //read new text file
        if(choice=="1"){
            cout <<"Input filename: ";
            string fileName = GetLine();
            mapImage = readFile(fileName,nodes,arcs);
            printNodes(nodes);
            printArcs(arcs,"black",0);

        //find path between two points
        } else if(choice=="2"){
            if(nodes.isEmpty()){
                cout << "Error: must first choose a text file with at least two nodes." << endl;
            }else{
                printNodes(nodes);
                printArcs(arcs,"black");

                cout << "Please click on a starting node." << endl;
                nodeT * startNode = getNodeFromClick(nodes);
                DrawFilledCircleWithLabel(startNode->location,"red",startNode->name);
                
                cout << startNode->name << " selected.  Please click on an ending node." << endl;
                nodeT * endNode = getNodeFromClick(nodes);
                DrawFilledCircleWithLabel(endNode->location,"red",endNode->name);
                
                cout << endNode->name << " selected." << endl << endl;
                findPath(startNode, endNode);
            }

        //find minimal spanning tree
        }  else if(choice=="3"){
            if(nodes.isEmpty()){
                cout << "Error: must first choose a text file with at least two nodes." << endl;
            } else {
                MovePen(0,0);
                DrawNamedPicture(mapImage);
                printNodes(nodes);
                findMinimalTree(nodes, arcs, mapImage);
            }
        }
        
        //quit program
        else if(choice=="4"){
            cout << "Goodbye!" << endl;
            break;
        }
        
    }

    return 0;

}



/* Function: DrawFilledCircleWithLabel
 * Usage:  DrawFilledCircleWithLabel(center, "Green", "You are here");
 * -------------------------------------------------------------------
 * Uses facilities from extgraph to draw a circle filled with
 * color specified. The circle is centered at the given coord has the
 * specified radius.  A label is drawn to the right of the circle.
 * You can leave off the last argument if no label is desired.
 */
void DrawFilledCircleWithLabel(coordT center, string color, string label)
{
	MovePen(center.x + CircleRadius, center.y);
	SetPenColor(color);
	StartFilledRegion(1.0);
	DrawArc(CircleRadius, 0, 360);
	EndFilledRegion();
	if (!label.empty()) {
		MovePen(center.x + CircleRadius, center.y);
		SetFont("Helvetica");
		SetPointSize(LabelFontSize);
		DrawTextString(label);
	}
}

/* Function: DrawLineBetween
 * Usage:  DrawLineBetween(coord1, coord2, "Black");
 * -------------------------------------------------
 * Uses facilities from extgraph to draw a line of the
 * specified color between the two given coordinates.
 */
void DrawLineBetween(coordT start, coordT end, string color)
{
	SetPenColor(color);
	MovePen(start.x, start.y);
	DrawLine(end.x - start.x, end.y - start.y);
}


/* Function: GetMouseClick
 * Usage:  loc = GetMouseClick();
 * ------------------------------
 * Waits for the user to click somewhere on the graphics window
 * and returns the coordinate of where the click took place.
 */
coordT GetMouseClick()
{
	coordT where;
	WaitForMouseDown();
	WaitForMouseUp();
	where.x = GetMouseX();
	where.y = GetMouseY();
	return where;
}

/* Function: WithinDistance
 * Usage:  if (WithinDistance(click, pt))...
 * -----------------------------------------
 * Returns true if the Cartesian distance between the two coordinates
 * is <= the specified distance, false otherwise. If not specified,
 * the distance is assumed to be size of the node diameter on screen.
 */
bool WithinDistance(coordT pt1, coordT pt2, double maxDistance)
{
	double dx = pt2.x - pt1.x;
	double dy = pt2.y - pt1.y;
	double distance = sqrt(dx*dx + dy*dy);
	return (distance <= maxDistance);
}


/* Function: pathDistance
 * returns the sum of the distance of all arcs in the given path.
 */ 

double pathDistance(Stack<arcT*> path){
    double total_distance = 0;
    while(!path.isEmpty()) {
        total_distance += path.pop()->distance;
    }
    return total_distance;
}


/* Function: pathCmp
 * compares the total length of two paths.  Note that it is an inverse comparison,
 * so when used for sorting or PQueueing it will put elements in ASCENDING order.
 */

int pathCmp(Stack<arcT*> path1, Stack<arcT*> path2){
    if(pathDistance(path1)<pathDistance(path2)) return 1;
    else if(pathDistance(path1)>pathDistance(path2)) return -1;
    else return 0;
}


/* Function: arcPtrCmp
 * compares two arc pointers by their .distance property.
 */ 

int arcPtrCmp(arcT * a, arcT * b){
    if(a->distance < b->distance) return 1;
    else if(a->distance > b->distance) return -1;
    else return 0;
}


/* Function: getNextWord
 * iterates along a string and returns the next complete word, with " " as the delimiter.
 * accepts the "pos" iterator by reference and modifies it, so multiple calls to this function
 * will return successive words from the string.
 */ 
string getNextWord(int & pos, string line){
    string result = "";
    while(pos < line.length()){
        string nextChar = line.substr(pos++,1);
        if(nextChar==" ") break;
        result = result + nextChar;
    }
    return result;
}

/* Function: readFile
 * opens up a .txt file and interprets it.  populates the "nodes" and "arcs" data structures,
 * and returns the name of the background image (which is the only other useful data in the file).
 * File must be properly formatted or it will raise an error.
 */ 
string readFile(string fileName, Map<nodeT*> & nodes, Vector<arcT*> & arcs){
    

    //in case this is not the first time that readfile() has been called:
    //deletes all nodes and arcs currently in the heap before populating
    //them with fresh data from the file.
    Map<nodeT*>::Iterator iter = nodes.iterator();
    while(iter.hasNext()){
       delete nodes[iter.next()];
    }
    nodes.clear();
   
    for(int i = 0; i < arcs.size(); i++){
       delete arcs[i];
    }
    arcs.clear(); 

    string line;
    string mapImage;
    ifstream myfile (fileName.c_str());
    if (myfile.is_open()){

        //get first line and draw it as the background map image.
        getline(myfile,mapImage);
        MovePen(0,0);
        DrawNamedPicture(mapImage);

        //now read the NODES section.  use it to populate the nodes object.  
        getline(myfile,line);
        if(line != "NODES")
            Error("error - file does not contain properly-formatted nodes section.");

        while (getline(myfile,line) && line != "ARCS"){
            
            //construct a new node on the heap
            nodeT * nextNode;
            nextNode = new nodeT;                   
            
            //iterate through the line of text and populate this node's data fields,
            //assuming that the text follows the order [name] [x location] [y location]
            //also adds itself to the list of nodes it is connected to 
            // (this will beused for minimal spanning tree function)
            int i = 0;
            nextNode->name = getNextWord(i,line);
            nextNode->location.x = atof(getNextWord(i,line).c_str());               
            nextNode->location.y = atof(getNextWord(i,line).c_str());
            nextNode->connections.add(nextNode->name);

            // Sets up a pointer to this node in the map called "nodes," so that it may be looked up by name.
            nodes[nextNode->name] = nextNode;
        }

        //now read the ARCS section.
        if(line !="ARCS") 
            Error("file does not contain a properly formatted Arcs section.");


        while(getline(myfile,line)){
            
            arcT * nextArc;
            nextArc = new arcT;
            
            // populates the arc's data fields, assuming that the words in the line of text 
            // follow the order [end1] [end2] [distance].
            int i = 0;
            nextArc->end1 = nodes[getNextWord(i,line)];
            nextArc->end2 = nodes[getNextWord(i,line)];
            
            nextArc->distance = atof(getNextWord(i,line).c_str());

            nextArc->end1->arcs.add(nextArc);
            nextArc->end2->arcs.add(nextArc);
            arcs.add(nextArc);
        }
    } else cout << "Unable to open file"; 

    return mapImage;
}


/* Function: getChoiceFromUser
 * prompts and re-prompts the user until they enter a valid input ("1","2","3",or "4").
 */ 

string getChoiceFromUser(){
    cout << endl << endl << "Your options are:" << endl
        << "(1) Choose a new graph data file" << endl
        << "(2) Find shortest path using Dijkstra's algorithm" << endl
        << "(3) Compute the minimal spanning tree using Kruskal's algorithm" << endl
        << "(4) Quit" << endl
        << "Enter choice: ";
    string choice = GetLine();
    while(choice != "1" && choice != "2" && choice != "3" && choice != "4"){
        cout << "Invalid choice, please try again: ";
        choice = GetLine();
    }
    return choice;
}

/* Function: printNodes
 * iterates through a Map of nodes and prints all of them to the graphics window (in black).
 */ 

void printNodes(Map<nodeT*> nodes){
  Map<nodeT*>::Iterator iter = nodes.iterator();
  while(iter.hasNext()){
      string key = iter.next();
      DrawFilledCircleWithLabel(nodes[key]->location,"black",key);
  }
}

/* Function: printArcs
 * iterates through a Vector of arcss and prints all of them to the graphics window.  
 * Accepts optional arguments specifying color and a pause time between drawing each arc.
 */ 

void printArcs(Vector<arcT*> & arcs, string color, double pauseTime){
    for(int i = 0; i < arcs.size(); i++){
        DrawLineBetween(arcs[i]->end1->location, arcs[i]->end2->location,color);
        Pause(pauseTime);
    }
}

/* Function: getNodeFromClick
 * with the help of GetMouseClick(), this function returns a pointer to the node
 * that the user has clicked on.  Keeps waiting for more clicks until user clicks
 * close enough to a node.
 */ 
nodeT* getNodeFromClick(Map<nodeT*> nodes){
    while(true){
        coordT clicked = GetMouseClick();
        Map<nodeT*>::Iterator iter = nodes.iterator();
        while(iter.hasNext()){
            string key = iter.next();
            if(WithinDistance(nodes[key]->location,clicked)){
                return nodes[key];
            }
        }
    }
}

/* Function: printPath
 * highlights a path in red on the graphics window.  pauses between each "step"
 * for an animation effect.  Prints total path distance to the console.
 * Really this should be unified with printArcs and/or
 * printNodes, but that would make the animation step more difficult...
 */ 
void printPath( Stack<arcT *> arcs){
    double totalDistance = 0;
    Stack<arcT *> reversedArcs;
    while(!arcs.isEmpty()){
        reversedArcs.push(arcs.pop());
    }
    while(!reversedArcs.isEmpty()){
        arcT * arc = reversedArcs.pop();
        DrawFilledCircleWithLabel(arc->end1->location, "red", arc->end1->name);
        DrawFilledCircleWithLabel(arc->end2->location, "red", arc->end2->name);
        DrawLineBetween(arc->end1->location, arc->end2->location,"red");
        totalDistance += arc->distance;
        Pause(0.05);
    }
    cout << "Total distance is " << totalDistance << "." << endl;
}


/* Function: enqueueArcs
 * iterates through all the arcs connected to the given node.
 * generates and enqueues a new version of the path with each
 * arc as a possible next step.
 */

void enqueueArcs(nodeT* node, Stack<arcT*> & path, PQueue<Stack<arcT *> > & paths){
    for(int i = 0; i < node->arcs.size(); i++){
        path.push(node->arcs[i]);
        paths.enqueue(path,pathCmp);
        path.pop();
    }
}


/* Function: findPath
 * Uses a priority queue and Dijkstra's algorithm to find the shortest available path
 * between the two given nodes.  After identifying the path, the function will highlight
 * it in red and print out the total path distance.
 */

void findPath(nodeT *startNode, nodeT *endNode){
    Stack<arcT*> path;
    PQueue<Stack<arcT*> > paths;
    Set<nodeT*> includedNodes;      
    includedNodes.add(startNode);

    //enqueue a path for each possible step from the 1st node.
    enqueueArcs(startNode, path, paths);

    //main loop.  keeps generating and enqueing possible paths; breaks as soon as the first
    //valid path to the end node is found.  (Due to the nature of Dijkstra's algorithm, it can
    //be assumed that the first such path is the most efficient way to get there.)
    while(true){
        path = paths.dequeueMax(pathCmp);

        //check if the path currently reaches the end (goal) node.  
        //if so, stops the algorithm so result can be printed.
        //there's a bit of awkwardness in the next 5 lines because arcs are bidirectional
        //so an extra step is needed to check each end.
        if(path.peek()->end1->name==endNode->name || path.peek()->end2->name==endNode->name) 
            break;
        
        nodeT *nextNode = path.peek()->end1;
        if(includedNodes.contains(nextNode))
            nextNode = path.peek()->end2;
        
        //enqueues this path as a possible next step ONLY IF it leads to a node not already reached.
        if(!includedNodes.contains(nextNode)){
            includedNodes.add(nextNode);
            enqueueArcs(nextNode,path,paths);

        }
    }
    printPath(path);

}


/* Function: findMinimalTree
 * Identifies the minimal spanning tree using Kruskal's algorithm.
 * Redraws graphics window to show only the arcs included in the minimal tree,
 * and prints out some summary information to the console.
 */ 

void findMinimalTree(Map<nodeT*> & nodes, Vector<arcT*> & arcs, string mapImage){
    Set<string> includedNodes;
    Vector<arcT*> includedArcs;
    PQueue<arcT*> sortedArcs;
    int nConnections = 0;
    double totalDistance = 0.0;
    
    //feed arcs into a PQueue that will sort by distance and return them in ascending order.
    for(int i = 0; i < arcs.size(); i++){
        sortedArcs.enqueue(arcs[i],arcPtrCmp);
    }

    while(true){
        
        arcT * arc = sortedArcs.dequeueMax(arcPtrCmp);
        if(!arc->end1->connections.contains(arc->end2->name)){

            arc->end1->connections.unionWith(arc->end2->connections);
            Set<string>::Iterator iter = arc->end1->connections.iterator();
            while (iter.hasNext()) {
                nodes[iter.next()]->connections = arc->end1->connections;
            }

            nConnections++;
            includedArcs.add(arc);
            totalDistance += arc->distance;
        }
        if(arc->end1->connections.size()==nodes.size())
            break;

    }

    printArcs(includedArcs,"black",.05);
    cout << endl << "Minimal spanning tree requires " << nConnections << " connections" << endl 
        << " to join " << nodes.size() << " nodes. Total network distance is " << totalDistance << "." << endl;
}
