#ifndef SEGMENT 
#define SEGMENT
#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif
#include <cmath>
#include <iostream>

#include "Camera.h"


void drawTriangle()
{

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    }
    glEnd();
}

void drawCylinderSegment(float height, float radius, int segments)
{
    Point points[segments + 1];

    float offset = 70.5287794 * M_PI / 180.0;

    for (int i = 0; i < segments + 1; i++)
    {
        float theta = -offset / 2 + i * offset / segments;
        points[i].x = radius * cos(theta);
        points[i].y = radius * sin(theta);
    }

    glBegin(GL_QUADS);
    for (int i = 0; i < segments; i++)
    {
        glVertex3f(points[i].x, points[i].y, height / 2);
        glVertex3f(points[i].x, points[i].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, height / 2);
    }
    glEnd();
}

void drawSphereQuad(float radius, int segments)
{

    struct Point points[segments + 1][segments + 1];
    int i, j;
    float x, y;
    // generate points
    for (i = 0; i <= segments; i++)
    {

        for (j = 0; j <= segments; j++)
        {
            x = -1 + (float)i / segments * 2;
            y = -1 + (float)j / segments * 2;
            points[i][j].x = x;
            points[i][j].y = y;
            points[i][j].z = 1;

            points[i][j] = points[i][j].normalize();
            points[i][j] = points[i][j].scalarMultiply(radius);
        }
    }
    // draw quads using generated points
    for (i = 0; i < segments; i++)
    {
        for (j = 0; j < segments; j++)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
            }
            glEnd();
        }
    }
}
#endif