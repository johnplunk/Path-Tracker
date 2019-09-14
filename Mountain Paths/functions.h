#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Do not add using namespace

#include <iostream>

const int MAX_ROWS = 1000;
const int MAX_COLS = 1000;

struct Pixel {
    int red;
    int green;
    int blue;
};

void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, 
                int& max, int& min);
void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, 
                std::istream& inData);
void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], 
                    const int elevations[MAX_ROWS][MAX_COLS], 
                    int rows, int cols, int max, int min);
void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, 
                    std::ostream& outData);
int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row);
int scaleValue(int value, int max, int min);

#endif