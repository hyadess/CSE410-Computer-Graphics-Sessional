#ifndef COMMONOP
#define COMMONOP

#include "1905064_Point.hpp"
#include "1905064_Color.hpp"

// r=a-2(a.n)n
Point Reflection(Point n, Point a)

{
    n.normalize();
    double dot = a.dotProduct(n);
    Point temp = n.scalarMultiply(2 * dot);
    Point reflected = a.subtract(temp);
    reflected.normalize();
    return reflected;
}

double determinant(double ara[3][3])
{
    double v1 = ara[0][0] * (ara[1][1] * ara[2][2] - ara[1][2] * ara[2][1]);
    double v2 = ara[0][1] * (ara[1][0] * ara[2][2] - ara[1][2] * ara[2][0]);
    double v3 = ara[0][2] * (ara[1][0] * ara[2][1] - ara[1][1] * ara[2][0]);
    return v1 - v2 + v3;
}

void colorNormallize(Color &color)
{
    if (color.r > 1)
        color.r = 1;
    if (color.g > 1)
        color.g = 1;
    if (color.b > 1)
        color.b = 1;

    if (color.r < 0)
        color.r = 0;
    if (color.g < 0)
        color.g = 0;
    if (color.b < 0)
        color.b = 0;
}

#endif