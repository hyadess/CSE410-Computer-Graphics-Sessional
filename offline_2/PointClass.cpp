#ifndef POINT
#define POINT

#include <bits/stdc++.h>
using namespace std;

class Point
{

public:
    double x, y, z,scale;
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->scale=1;
    }
    double value()
    {
        return sqrt(x*x+y*y+z*z);
    }
    void normalize()
    {
        double value=this->value();
        x=x/value;
        y=y/value;
        z=z/value;

    }
    void setScale(double scale)
    {
        this->scale=scale;
    }

    double dotProduct(Point q)
    {
        return x*q.x+y*q.y+z*q.z;
    }
    Point crossProduct(Point q)
    {
        double X = y * q.z - z * q.y;
        double Y = z * q.x - x * q.z;
        double Z = x * q.y - y * q.x;

        return Point(X,Y,Z);
    }

    Point subtract(Point q)
    {
        return Point(x-q.x,y-q.y,z-q.z);
    }


};

#endif