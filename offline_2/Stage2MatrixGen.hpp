#ifndef STAGE2
#define STAGE2

#include "MatrixClass.hpp"
#include "PointClass.hpp"

Matrix translationPart(Point eye)
{
    double translationArr[4][4];

    translationArr[0][0] = 1.0;
    translationArr[0][1] = 0.0;
    translationArr[0][2] = 0.0;
    translationArr[0][3] = -eye.x;

    translationArr[1][0] = 0.0;
    translationArr[1][1] = 1.0;
    translationArr[1][2] = 0.0;
    translationArr[1][3] = -eye.y;

    translationArr[2][0] = 0.0;
    translationArr[2][1] = 0.0;
    translationArr[2][2] = 1.0;
    translationArr[2][3] = -eye.z;

    translationArr[3][0] = 0.0;
    translationArr[3][1] = 0.0;
    translationArr[3][2] = 0.0;
    translationArr[3][3] = 1.0;

    Matrix resultMatrix = Matrix(translationArr);

    return resultMatrix;
}

Matrix viewTransform(Point eye, Point look, Point up)
{
    Point l = look.subtract(eye);
    l.normalize();
    Point r = l.crossProduct(up);
    r.normalize();
    Point u = r.crossProduct(l);

    Matrix translationMatrix = translationPart(eye);

    double rotationArr[4][4];

    rotationArr[0][0] = r.x;
    rotationArr[0][1] = r.y;
    rotationArr[0][2] = r.z;
    rotationArr[0][3] = 0.0;

    rotationArr[1][0] = u.x;
    rotationArr[1][1] = u.y;
    rotationArr[1][2] = u.z;
    rotationArr[1][3] = 0.0;

    rotationArr[2][0] = -l.x;
    rotationArr[2][1] = -l.y;
    rotationArr[2][2] = -l.z;
    rotationArr[2][3] = 0.0;

    rotationArr[3][0] = 0.0;
    rotationArr[3][1] = 0.0;
    rotationArr[3][2] = 0.0;
    rotationArr[3][3] = 1.0;

    Matrix rotationMatrix = Matrix(rotationArr);

    Matrix resultMatrix = rotationMatrix.multiplication(translationMatrix);
    return resultMatrix;
}

#endif

#
