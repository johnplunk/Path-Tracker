#include <istream>
#include <fstream>
#include <cmath>
#include "functions.h"
using namespace std;

void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int& max, int& min) {
	// initialize max and min to first element in array
	min = elevations[0][0];
	max = elevations[0][0];

	for (int row=0; row<rows; ++row) {
		for (int col=0; col<cols; ++col) {
			if (elevations[row][col]<min)
				min = elevations[row][col];
			if (elevations[row][col]>max)
				max = elevations[row][col];
		}
	}
}

void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, istream& inData) {
	for (int row=0; row<rows; ++row) {
		for (int col=0; col<cols; ++col) {
			int elev;
			inData >> elev;
			if (inData.eof() && inData.fail()) {
                cout << "Error: End of file reached prior to getting all required data.";
                cout << endl;
                exit(1);
			}
			else if (inData.fail()) {
				cout << "Error: Read a non-integer value." << endl;
				exit(1);
			}
			else if (inData.bad()) {
				cout << "Error: Problem reading the file." << endl;
				exit(1);
			}
            else {
                elevations[row][col] = elev;
            }
		}
	}
    
    // check for too many data points
	int extraData = 0;
    inData >> extraData;
	if (!inData.fail()) { // if I can successfully read in an extra number
        if (!inData.bad()) { // ensure it is failbit and not badbit
            cout << "Error: Too many data points." << endl;
            exit(1);
        }
        else {
            cout << "Error: Problem reading the file." << endl;
			exit(1);
        }
	}
}

int scaleValue(int value, int max, int min) {
    return round(static_cast<double>((value-min)*255)/(max - min));
}

void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], const int elevations[MAX_ROWS][MAX_COLS], 
                    int rows, int cols, int max, int min) {
	for (int row=0; row<rows; ++row) {
		for (int col=0; col<cols; ++col) {
			int rgbVal = scaleValue(elevations[row][col], max, min);
			image[row][col] = {rgbVal, rgbVal, rgbVal};
		}
	}
}

void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, ostream& outData) {
	outData << "P3" << endl;
	outData << cols << " " << rows << endl;
	outData << 255 << endl;
	for (int row=0; row<rows; ++row) {
		for (int col=0; col<cols; ++col) {
			outData << image[row][col].red << " ";
			outData << image[row][col].green << " ";
			outData << image[row][col].blue << " ";
		}
		outData << endl;
	}
}

int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row) {
    int i = start_row;
    int newI = 0;
    int length = 0;
    int shortestDistance = 0;
    int distanceAcross = 0;
    int distanceDown = 0;
    int distanceUp = 0;
    for (int j = 0; j < cols; j++) {
        bool botDist = false;
        image[i][j].red = color.red; // highlights the path in the desired color
        image[i][j].blue = color.blue;
        image[i][j].green = color.green;
        newI = i;
        distanceAcross = 0;
        distanceDown = 0;
        distanceUp = 0;
        distanceAcross = abs(elevations[i][j] - elevations[i][j+1]); // calculates distance from current elevation to elevation directly to the right
        shortestDistance = distanceAcross;
        if (i != rows) {
            distanceDown = abs(elevations[i][j] - elevations[i+1][j+1]); // calculates distance from current elevation to elevation directly down and to the right
            if (distanceDown < distanceAcross) {
                shortestDistance = distanceDown;
                newI++;
                botDist = true; // there's a certain case where top is larger than bottom that i must account for
            }
        }
        if (i != 0) {
            distanceUp = abs(elevations[i][j] - elevations[i-1][j+1]); // calculates distance from current elevation to elevation directly up and to the right
            if (distanceUp < shortestDistance) {
                shortestDistance = distanceUp;
                if (botDist == true) {
                    if (newI - 2 < 0) {
                        newI = 0; // so we don't leave array bounds
                    }
                    else {
                        newI = newI - 2; // if top is biggest and bottom is second highest, we've already increased i by 1, so we must lower it by two to compensate
                    }
                }
                if (botDist == false) {
                    newI--;
                }
            }
        }
        i = newI;
        length = length + shortestDistance;
        if (j == cols - 1) {
            length = length - shortestDistance;
        }
    }
    return length;
}

