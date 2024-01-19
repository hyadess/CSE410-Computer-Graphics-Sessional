#ifndef STAGE1
#define STAGE1
#include <bits/stdc++.h>
using namespace std;

#include "MatrixClass.cpp"
#include "PointClass.cpp"

// for rotation matrix generation........................

// for converting i,j,k into c1,c2,c3.........................
Matrix rodriguezMatrixGen(Point a, double angle)
{
    a.normalize();
    double arr[4][4];
    double c = cosf64(angle * M_PI / 180.0);
    double s = sinf64(angle * M_PI / 180.0);

    arr[0][0] = c + a.x * a.x * (1 - c);
    arr[0][1] = a.x * a.y * (1 - c) - a.z * s;
    arr[0][2] = a.x * a.z * (1 - c) + a.y * s;
    arr[0][3] = 0.0;

    arr[1][0] = a.y * a.x * (1 - c) + a.z * s;
    arr[1][1] = c + a.y * a.y * (1 - c);
    arr[1][2] = a.y * a.z * (1 - c) - a.x * s;
    arr[1][3] = 0.0;

    arr[2][0] = a.z * a.x * (1 - c) - a.y * s;
    arr[2][1] = a.z * a.y * (1 - c) + a.x * s;
    arr[2][2] = c + a.z * a.z * (1 - c);
    arr[2][3] = 0.0;

    arr[3][0] = 0.0;
    arr[3][1] = 0.0;
    arr[3][2] = 0.0;
    arr[3][3] = 1.0;

    Matrix m = Matrix(arr);
    return m;
}

Matrix rotationMatrixGen(double ax, double ay, double az, double angle)
{
    Point a = Point(ax, ay, az);
    Matrix rodriguezMatrix = rodriguezMatrixGen(a, angle);
    Point i = Point(1, 0, 0);
    Point j = Point(0, 1, 0);
    Point k = Point(0, 0, 1);
    Point c1 = rodriguezMatrix.pointMultiplication(i);
    Point c2 = rodriguezMatrix.pointMultiplication(j);
    Point c3 = rodriguezMatrix.pointMultiplication(k);

    double rotationArr[4][4];

    rotationArr[0][0] = c1.x;
    rotationArr[0][1] = c2.x;
    rotationArr[0][2] = c3.x;
    rotationArr[0][3] = 0.0;

    rotationArr[1][0] = c1.y;
    rotationArr[1][1] = c2.y;
    rotationArr[1][2] = c3.y;
    rotationArr[1][3] = 0.0;

    rotationArr[2][0] = c1.z;
    rotationArr[2][1] = c2.z;
    rotationArr[2][2] = c3.z;
    rotationArr[2][3] = 0.0;

    rotationArr[3][0] = 0.0;
    rotationArr[3][1] = 0.0;
    rotationArr[3][2] = 0.0;
    rotationArr[3][3] = 1.0;

    Matrix resultMatrix = Matrix(rotationArr);

    return resultMatrix;
}

// translation.....................

Matrix translationMatrixGen(double tx, double ty, double tz)
{
    double translateArr[4][4];

    translateArr[0][0] = 1.0;
    translateArr[0][1] = 0.0;
    translateArr[0][2] = 0.0;
    translateArr[0][3] = tx;

    translateArr[1][0] = 0.0;
    translateArr[1][1] = 1.0;
    translateArr[1][2] = 0.0;
    translateArr[1][3] = ty;

    translateArr[2][0] = 0.0;
    translateArr[2][1] = 0.0;
    translateArr[2][2] = 1.0;
    translateArr[2][3] = tz;

    translateArr[3][0] = 0.0;
    translateArr[3][1] = 0.0;
    translateArr[3][2] = 0.0;
    translateArr[3][3] = 1.0;

    Matrix resultMatrix = Matrix(translateArr);

    return resultMatrix;
}

// scaling....................
Matrix scalingMatrixGen(double sx, double sy, double sz)
{
    double scalingArr[4][4];

    scalingArr[0][0] = sx;
    scalingArr[0][1] = 0.0;
    scalingArr[0][2] = 0.0;
    scalingArr[0][3] = 0.0;

    scalingArr[1][0] = 0.0;
    scalingArr[1][1] = sy;
    scalingArr[1][2] = 0.0;
    scalingArr[1][3] = 0.0;

    scalingArr[2][0] = 0.0;
    scalingArr[2][1] = 0.0;
    scalingArr[2][2] = sz;
    scalingArr[2][3] = 0.0;

    scalingArr[3][0] = 0.0;
    scalingArr[3][1] = 0.0;
    scalingArr[3][2] = 0.0;
    scalingArr[3][3] = 1.0;

    Matrix resultMatrix = Matrix(scalingArr);

    return resultMatrix;
}





#endif
