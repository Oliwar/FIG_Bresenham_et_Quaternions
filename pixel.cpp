#include "pixel.h"

Pixel::Pixel(){}

Pixel::Pixel(int x, int y, float r, float v, float b)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->v = v;
    this->b = b;
}

void Pixel::affiche(QOpenGLShaderProgram *sp, int start){
    this->modelMatrix.setToIdentity();

    sp->setUniformValue("modelMatrix", this->modelMatrix);

    glDrawArrays(GL_TRIANGLES, start, 6);
}
