#ifndef LIGHT
#define LIGHT
#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "1905064_Point.hpp"
#include "1905064_Color.hpp"

class Light
{
public:
    Point position;
    Color color;
    Light()
    {
        position = Point();
        color = Color();
    }
    Light(Point position, Color color)
    {
        this->position = position;
        this->color = color;
    }

    void draw()
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(position.x, position.y, position.z);
        glEnd();
    }
};

#endif