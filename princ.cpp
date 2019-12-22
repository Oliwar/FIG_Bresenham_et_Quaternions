// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>
#include <chrono>
#include <thread>

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    spinBox_X_start->setMaximum(glarea->grille.largeur-1);
    spinBox_X_arrive->setMaximum(glarea->grille.hauteur-1);
    spinBox_Y_arrive->setMaximum(spinBox_X_arrive->value() + spinBox_Y_start->value() - spinBox_X_start->value());
    change_pixel_color(spinBox_X_start->value(), spinBox_Y_start->value(), 0, 1, 0);
    change_pixel_color(spinBox_X_arrive->value(), spinBox_Y_arrive->value(), 0, 1, 0);
    bresenham = Bresenham(spinBox_X_start->value(), spinBox_Y_start->value(), spinBox_X_arrive->value(), spinBox_Y_arrive->value(), glarea->grille.pixels);
}

void Princ::remove_red_pixels(){
    while(bresenham.xCur > bresenham.xStart)
        bresenham.bresenham_reverse_step();
}

void Princ::on_pushButton_reinit_clicked()
{
    remove_red_pixels();
    glarea->setFocus();
    glarea->makeGLObjects();
}

void Princ::on_pushButton_bresenham_clicked()
{
    remove_red_pixels();
    while(bresenham.xCur < bresenham.xEnd - 1)
        bresenham.bresenham_step();
    glarea->setFocus();
    glarea->makeGLObjects();
}

void Princ::move_start_or_end_pixel(int valX, int valY){
    for(Pixel &p : glarea->grille.pixels){
        if(p.x == valX && p.y == valY &&
           !(p.x == spinBox_X_start->value() && p.y == spinBox_Y_start->value()) &&
           !(p.x == spinBox_X_arrive->value() && p.y == spinBox_Y_arrive->value())){
            p.r = 0.5f;
            p.v = 0.5f;
            p.b = 0.5f;
            break;
        }
    }

    glarea->makeGLObjects();
}

void Princ::change_pixel_color(int valX, int valY, float r, float v, float b){
    for(Pixel &p : glarea->grille.pixels){
        if(p.x == valX && p.y == valY){
            p.r = r;
            p.v = v;
            p.b = b;
            break;
        }
    }

    glarea->makeGLObjects();
    glarea->update();
}

void Princ::on_spinBox_X_start_valueChanged(int arg1)
{
    remove_red_pixels();
    move_start_or_end_pixel(pred_X_start_val, pred_Y_start_val);
    pred_X_start_val = arg1;
    int valX = arg1;
    int valY = spinBox_Y_start->value();
    change_pixel_color(valX, valY, 0, 1, 0);
    spinBox_X_arrive->setMinimum(arg1);
    spinBox_Y_arrive->setMaximum(spinBox_X_arrive->value() + spinBox_Y_start->value() - arg1);
    bresenham = Bresenham(spinBox_X_start->value(), spinBox_Y_start->value(), spinBox_X_arrive->value(), spinBox_Y_arrive->value(), glarea->grille.pixels);
}

void Princ::on_spinBox_Y_start_valueChanged(int arg1)
{
    remove_red_pixels();
    move_start_or_end_pixel(pred_X_start_val, pred_Y_start_val);
    pred_Y_start_val = arg1;
    int valX = spinBox_X_start->value();
    int valY = arg1;
    change_pixel_color(valX, valY,0, 1, 0);
    spinBox_Y_arrive->setMinimum(arg1);
    spinBox_Y_arrive->setMaximum(spinBox_X_arrive->value() + arg1 - spinBox_X_start->value());
    bresenham = Bresenham(spinBox_X_start->value(), spinBox_Y_start->value(), spinBox_X_arrive->value(), spinBox_Y_arrive->value(), glarea->grille.pixels);
}

void Princ::on_spinBox_X_arrive_valueChanged(int arg1)
{
    remove_red_pixels();
    move_start_or_end_pixel(pred_X_arrive_val, pred_Y_arrive_val);
    pred_X_arrive_val = arg1;
    int valX = arg1;
    int valY = spinBox_Y_arrive->value();
    change_pixel_color(valX, valY, 0, 1, 0);
    spinBox_Y_arrive->setMaximum(arg1 + spinBox_Y_start->value() - spinBox_X_start->value());
    bresenham = Bresenham(spinBox_X_start->value(), spinBox_Y_start->value(), spinBox_X_arrive->value(), spinBox_Y_arrive->value(), glarea->grille.pixels);
}

void Princ::on_spinBox_Y_arrive_valueChanged(int arg1)
{
    remove_red_pixels();
    move_start_or_end_pixel(pred_X_arrive_val, pred_Y_arrive_val);
    pred_Y_arrive_val = arg1;
    int valX = spinBox_X_arrive->value();
    int valY = arg1;
    change_pixel_color(valX, valY, 0, 1, 0);
    bresenham = Bresenham(spinBox_X_start->value(), spinBox_Y_start->value(), spinBox_X_arrive->value(), spinBox_Y_arrive->value(), glarea->grille.pixels);
}

void Princ::on_pushButton_next_step_clicked()
{
    bresenham.bresenham_step();
    glarea->makeGLObjects();
    glarea->setFocus();
}

void Princ::on_pushButton_prev_step_clicked()
{
    bresenham.bresenham_reverse_step();
    glarea->makeGLObjects();
    glarea->setFocus();
}

void Princ::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
        case Qt::Key_Left :
            qDebug() << __FUNCTION__ << "Key_Left";
            bresenham.bresenham_reverse_step();
            glarea->makeGLObjects();
            break;
        case Qt::Key_Right :
            qDebug() << __FUNCTION__ << "Key_Right";
            bresenham.bresenham_step();
            glarea->makeGLObjects();
            break;
        case Qt::Key_Return :
        case Qt::Key_Enter :
            glarea->setFocus();
            break;
    }
}

void Princ::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void Princ::on_pushButton_quaternion_clicked()
{
    glarea->quaternion = !glarea->quaternion;
    if(glarea->quaternion) qDebug() << "quaternion activé";
    else qDebug() << "quaternion déactivé";
}
