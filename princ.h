// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#define PRINC_H

#include "bresenham.h"
#include "ui_princ.h"

class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = nullptr);
    void remove_red_pixels();
private:
    void change_pixel_color(int valX, int valY, float r, float v, float b);
    void move_start_or_end_pixel(int valX, int valY);
    void remove_colored_pixels(float r, float v, float b);

    int pred_X_start_val = 0;
    int pred_Y_start_val = 0;
    int pred_X_arrive_val = 0;
    int pred_Y_arrive_val = 0;
    Bresenham bresenham;

protected:
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
protected slots:
private slots:
    void on_pushButton_reinit_clicked();
    void on_spinBox_X_start_valueChanged(int arg1);
    void on_spinBox_Y_start_valueChanged(int arg1);
    void on_spinBox_X_arrive_valueChanged(int arg1);
    void on_spinBox_Y_arrive_valueChanged(int arg1);
    void on_pushButton_next_step_clicked();
    void on_pushButton_prev_step_clicked();
    void on_pushButton_bresenham_clicked();
    void on_pushButton_quaternion_clicked();
};

#endif // PRINC_H
