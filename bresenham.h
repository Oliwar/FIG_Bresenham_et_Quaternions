#ifndef BRESENHAM_H
#define BRESENHAM_H

#include "pixel.h"

class Bresenham
{
public:
    Bresenham();
    Bresenham(int xStart, int yStart, int xEnd, int yEnd, std::vector<Pixel> &pixels);

    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    int xCur;
    int yCur;
    int d;
    int i1;
    int i2;
    std::vector<Pixel> *pixels;
    void editer(int x, int y, float r, float v, float b);
    void bresenham_step();
    void bresenham_reverse_step();
};

#endif // BRESENHAM_H
