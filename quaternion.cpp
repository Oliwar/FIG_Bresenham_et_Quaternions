#include "quaternion.h"
#include <cmath>

Quaternion::Quaternion(){
    this->x = 1;
    this->y = 1;
    this->z = 1;
    this->w = 1;
}

Quaternion::Quaternion(float x, float y, float z, float w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quaternion::rotate(QMatrix4x4 &matrix, float angle, QVector3D axis)
{
    Quaternion q = angleAxis(angle, axis);
    matrix = mulMatrix(matrix, q.toMatrix());
}

Quaternion Quaternion::angleAxis(float angle, QVector3D axis) {

    QVector3D axisNormalized = normalize(axis);
    angle *= static_cast<float>(M_PI) / 180;
    angle *= 0.5f;
    float sinAngle = std::sin(angle);
    return Quaternion(std::cos(angle),
                      axisNormalized.x() * sinAngle,
                      axisNormalized.y() * sinAngle,
                      axisNormalized.z() * sinAngle);
}

QMatrix4x4 Quaternion::toMatrix()
{
    return QMatrix4x4(1-2*y*y-2*z*z,  2*x*y+2*w*z,    2*x*z-2*w*y,    0,
                      2*x*y-2*w*z,    1-2*x*x-2*z*z,  2*y*z+2*w*x,    0,
                      2*x*z+2*w*y,    2*y*z-2*w*x,    1-2*x*x-2*y*y,  0,
                      0,              0,              0,              1 );
}

QMatrix4x4 Quaternion::mulMatrix(QMatrix4x4 mat1, QMatrix4x4 mat2){
    QMatrix4x4 matResult = QMatrix4x4(0, 0, 0, 0,
                                      0, 0, 0, 0,
                                      0, 0, 0, 0,
                                      0, 0, 0, 0);

    int length = 4;
    int i = 0;
    int j = 0;
    for(int j1 = 0; j1 < length; j1++){
        for(int i2 = 0; i2 < length; i2++) {
            for(int i1 = 0, j2 = 0; j2 < length; i1++, j2++){
                matResult(j, i) += mat1(j1, i1) * mat2(j2, i2);
            }
            i++;
        }
        i = 0;
        j++;
    }

    return matResult;
}

QVector3D Quaternion::normalize(QVector3D axis){
    float val = std::sqrt(axis[0] * axis[0] +
                          axis[1] * axis[1] +
                          axis[2] * axis[2]);

    axis[0] = axis[0] / val;
    axis[1] = axis[1] / val;
    axis[2] = axis[2] / val;

    return axis;
}

void Quaternion::translate(float x, float y, float z){
    this->x += x;
    this->y += y;
    this->z += z;
}
