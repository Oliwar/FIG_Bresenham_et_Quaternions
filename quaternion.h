#ifndef QUATERNION_H
#define QUATERNION_H

#include <QMatrix4x4>



class Quaternion
{
public:
    Quaternion();
    Quaternion(float x, float y, float z, float w);

    void rotate(QMatrix4x4 &matrix, float angle, QVector3D axis);
    Quaternion angleAxis(float angle, QVector3D axis);
    QMatrix4x4 toMatrix();
    QMatrix4x4 mulMatrix(QMatrix4x4 mat1, QMatrix4x4 mat2);
    QVector3D normalize(QVector3D axis);
    void translate(float x, float y, float z);

    float x;
    float y;
    float z;
    float w;
};

#endif // QUATERNION_H
