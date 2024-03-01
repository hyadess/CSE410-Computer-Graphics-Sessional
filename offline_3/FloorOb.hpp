#ifndef FLOOROB
#define FLOOROB
#include "Object.hpp"

class Floor : public Object
{
public:
    int tiles;
    Floor()
    {
        tiles = 1;
    }

    Floor(int floorWidth, int tileWidth)
    {
        tiles = floorWidth / tileWidth;
        //start of the floor
        reference_point = Point(-floorWidth / 2, -floorWidth / 2, 0);
        length = tileWidth;
    }

    virtual Color getColorAt(Point point)
    {

        // x and y values...................

        int tileX = (point.x - reference_point.x) / length;
        int tileY = (point.y - reference_point.y) / length;

        if (tileX < 0 || tileX >= tiles || tileY < 0 || tileY >= tiles)
        {
            return Color(0, 0, 0);
        }

        if (((tileX + tileY) % 2) == 0)
        {
            return Color(1, 1, 1);
        }
        else
        {
            // cout<<"Black"<<endl;
            return Color(0, 0, 0);
        }
    }

    virtual Ray getNormalAt(Point point, Ray incidentRay)
    {
        //floor normal is always z axis...................
        if (incidentRay.direction.z > 0)
            return Ray(point, Point(0, 0, 1));
        else
            return Ray(point, Point(0, 0, -1));
    }

    virtual void draw()
    {
        for (int i = 0; i < tiles; i++)
        {
            for (int j = 0; j < tiles; j++)
            {
                if (((i + j) % 2) == 0)
                    glColor3f(1, 1, 1);
                else
                    glColor3f(0, 0, 0);

                glBegin(GL_QUADS);
                {
                    glVertex3f(reference_point.x + i * length, reference_point.y + j * length, 0);
                    glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + j * length, 0);
                    glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + (j + 1) * length, 0);
                    glVertex3f(reference_point.x + i * length, reference_point.y + (j + 1) * length, 0);
                }
                glEnd();
            }
        }
    }
    
    
    string returnType()
    {
        return "floor";
    }
    
    virtual double getIntersectingT(Ray ray, Color &color, int level)
    {
        Point normal = Point(0, 0, 1);
        double dotP = normal.dotProduct(ray.direction);

        //normal on that plane...............................
        if (round(dotP * 100) == 0)
            return -1;

        double t = -normal.dotProduct(ray.origin) / dotP;

        Point p = ray.origin.add(ray.direction.scalarMultiply(t));


        // out of range!!!!!!!!!!!!!!!
        if (p.x <= reference_point.x || p.x >= abs(reference_point.x) && p.y <= reference_point.y && p.y >= abs(reference_point.y))
        {
            return -1;
        }

        return t;
    }
};

#endif