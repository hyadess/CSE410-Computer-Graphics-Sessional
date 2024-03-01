#ifndef SPOTLIGHT
#define SPOTLIGHT
#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "1905064_Point.hpp"
#include "1905064_Light.hpp"
#include "1905064_Color.hpp"

class SpotLight
{
public:
    Point direction;
    Light light;
    double cutoff;
    SpotLight()
    {
        direction = Point();
        light = Light();
        cutoff = 0;
    }
    SpotLight(Point direction, Light light, double cutoff)
    {
        this->direction = direction;
        this->light = light;
        this->cutoff = cutoff;
    }
    void draw()
    {
        Color color = light.color;
        Point pos = light.position;

        glPointSize(15);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }
};

#endif