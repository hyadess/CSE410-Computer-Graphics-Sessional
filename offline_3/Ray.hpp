#ifndef RAY
#define RAY
#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "Point.hpp"

class Ray
{
public:
    Point origin;
    Point direction;
    Ray()
    {
        origin = Point();
        direction = Point();
    }
    Ray(Point origin, Point direction)
    {
        this->origin = origin;
        this->direction = direction;
        this->direction.normalize();
        direction.normalize();
    }
};

#endif