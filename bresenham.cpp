#include "bresenham.h"

Bresenham::Bresenham(){}

Bresenham::Bresenham(int xStart, int yStart, int xEnd, int yEnd, std::vector<Pixel> &pixels){
    this->xStart = xStart;
    this->yStart = yStart;
    this->xEnd = xEnd;
    this->yEnd = yEnd;
    this->xCur = xStart;
    this->yCur = yStart;
    int deltaX = xEnd - xStart;
    int deltaY = yEnd - yStart;
    int d = 2*deltaY-deltaX;
    int i1 = 2*deltaY;
    int i2 = 2*deltaY - 2*deltaX;
    this->d = d;
    this->i1 = i1;
    this->i2 = i2;
    this->pixels = &pixels;
}

void Bresenham::editer(int x, int y, float r, float v, float b){
    for(Pixel &p : *pixels){
        if(p.x == x && p.y == y){
            p.r = r;
            p.v = v;
            p.b = b;
            break;
        }
    }
}

void Bresenham::bresenham_step(){
    if(xCur < xEnd-1){
        xCur++;
        if(d < 0){
            d += i1;
        } else {
            d += i2;
            yCur++;
        }
        editer(xCur, yCur, 1, 0, 0);
    }
}

void Bresenham::bresenham_reverse_step(){
    if(xCur > xStart){
        editer(xCur, yCur, 0.5f, 0.5f, 0.5f);
        xCur--;
        if(d >= i2+i1){
            d -= i1;
        } else {
            d -= i2;
            yCur--;
        }
    }
}
