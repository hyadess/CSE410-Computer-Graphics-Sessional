#ifndef STAGE3
#define STAGE3

#include "MatrixClass.hpp"

Matrix projectionTransform(double fovY, double ratio, double near, double far)
{
    double fovX = fovY * ratio;
    double t = near * tanf64(fovY * M_PI / 360.0);
    double r = near * tanf64(fovX * M_PI / 360.0);

    double projectionArr[4][4];

    projectionArr[0][0] = near / r;
    projectionArr[0][1] = 0.0;
    projectionArr[0][2] = 0.0;
    projectionArr[0][3] = 0.0;

    projectionArr[1][0] = 0.0;
    projectionArr[1][1] = near / t;
    projectionArr[1][2] = 0.0;
    projectionArr[1][3] = 0.0;

    projectionArr[2][0] = 0.0;
    projectionArr[2][1] = 0.0;
    projectionArr[2][2] = -(far + near) / (far - near);
    projectionArr[2][3] = -(2 * far * near) / (far - near);

    projectionArr[3][0] = 0.0;
    projectionArr[3][1] = 0.0;
    projectionArr[3][2] = -1.0;
    projectionArr[3][3] = 0.0;

    Matrix resultMatrix = Matrix(projectionArr);

    return resultMatrix;
}

#endif