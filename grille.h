#ifndef GRILLE_H
#define GRILLE_H

#include "pixel.h"

using namespace std;

class Grille
{
public:
    Grille();
    Grille(int largeur, int hauteur);

    void affiche(QOpenGLShaderProgram *sp);

    int largeur;
    int hauteur;
    int nbPixels;
    std::vector<Pixel> pixels;
    QMatrix4x4 modelMatrix;
};

#endif // GRILLE_H
