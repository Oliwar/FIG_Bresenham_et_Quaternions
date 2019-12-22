#include "grille.h"

Grille::Grille(){}

Grille::Grille(int largeur, int hauteur)
{
    this->largeur = largeur;
    this->hauteur = hauteur;
    this->nbPixels = hauteur * largeur;

    Pixel p;
    for(int y = 0; y < hauteur; ++y){
        for(int x = 0; x < largeur; ++x){
            p = Pixel(x, y, 0.5f, 0.5f, 0.5f);
            pixels.push_back(p);
        }
    }
}

void Grille::affiche(QOpenGLShaderProgram *sp){
    int start = 0;
    for(Pixel &p : pixels){
        p.affiche(sp, start);
        start += 6;
    }
}

