#include <iostream>
#include <fstream>
#include <cstring>
#include "functions.h"
using namespace std;

/* John Plunk
   CSCE 121 */

// Normally you should not use global variables. However, the stack is
//   limited in size and will not allow arrays of this size. We'll learn
//   later how to put these arrays on the heap instead. Regardless, for now
//   we'll use these and treat them as if they were declared in main. So
//   when used in functions, we'll pass them as we would any other array.

static int elevations[MAX_ROWS][MAX_COLS];
static Pixel image[MAX_ROWS][MAX_COLS];

int main() {
	int rows = 0;
	int cols = 0;
	char infilename[100]="";  // initialize to empty string
    char outfilename[100]=""; // initialize to empty string
    
	cin >> rows >> cols;
	// check for negative values for rows cols
    //   stream state will be fail if non-integer is encountered
	if (!cin.good() || rows <=0 || cols <=0 || rows > MAX_ROWS || cols > MAX_COLS) {
		cout << "Error: Problem reading in rows and columns." << endl;
		return 1;
	}
    
	cin >> infilename;
	ifstream infs(infilename);
	if (!infs.is_open()) {
		cout << "Error: Unable to open file " << infilename << "." << endl;
		return 1;
	}
    
	// if we get this far then input stream is good
    
    loadData(elevations, rows, cols, infs);
        
    int max, min;
    findMaxMin(elevations, rows, cols, max, min);

    loadGreyscale(image, elevations, rows, cols, max, min);
    
    Pixel Red; // sets up the color red for all the paths
    Red.red = 252;
    Red.green = 25;
    Red.blue = 63;
    
    Pixel Green; // sets up the color green for the desired path
    Green.red = 31;
    Green.green = 253;
    Green.blue = 13;
    
    
    int distance = 0;
    int shortestDistance = 0;
    shortestDistance = colorPath(elevations,image,rows,cols,Red,0); // puts shortest distance in the same range as the other functions
    int index = 0;
    for (int i = 1; i < rows; i++) { //iterates through all the rows highlighting the desirable path
        distance = colorPath(elevations,image,rows,cols,Red,i);
        if ((distance < shortestDistance) && (distance != 0)) { // finds the index of the shortest path and the length
            shortestDistance = distance;
            index = i;
        }
    }
    shortestDistance = colorPath(elevations,image,rows,cols,Green,index); // highlights the shortest path
    
	strcpy(outfilename,infilename);
    strcat(outfilename, ".ppm");
    //cout << outfilename << endl;
    ofstream ofs(outfilename);
	if (!ofs.is_open()) {
		cout << "Error: Unable to open output file." << endl;
		return 1;
	}

    outputImage(image, rows, cols, ofs);
}