#ifndef PIXEL_H
#define PIXEL_H

#include <QOpenGLShaderProgram>



class Pixel
{
public:
    Pixel();
    Pixel(int x, int y, float r, float v, float b);

    void affiche(QOpenGLShaderProgram *sp, int start);

    int x;
    int y;
    float r;
    float v;
    float b;
    QMatrix4x4 modelMatrix;
};

#endif // PIXEL_H
