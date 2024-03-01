#ifndef SPHEREOB
#define SPHEREOB
#include "Object.hpp"

class Sphere : public Object
{
public:
    Sphere()
    {
    }

    Sphere(Point center, double radius)
    {
        reference_point = center;
        length = radius;
    }

    virtual Ray getNormalAt(Point point, Ray incidentRay)
    {
        return Ray(point, point.subtract(reference_point));
    }

    virtual void draw()
    {
        int stacks = 30;
        int slices = 20;

        Point points[100][100];
        int i, j;
        double h, r;
        // generate points
        for (i = 0; i <= stacks; i++)
        {
            h = length * sin(((double)i / (double)stacks) * (pi / 2));
            r = length * cos(((double)i / (double)stacks) * (pi / 2));
            for (j = 0; j <= slices; j++)
            {
                points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
                points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
                points[i][j].z = h;
            }
        }
        // draw quads using generated points
        for (i = 0; i < stacks; i++)
        {
            glPushMatrix();
            glTranslatef(reference_point.x, reference_point.y, reference_point.z);
            glColor3f(color.r, color.g, color.b);
            for (j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    // upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    // lower hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                }
                glEnd();
            }
            glPopMatrix();
        }
    }
    string returnType()
    {
        return "circle";
    }
    virtual double getIntersectingT(Ray ray, Color &color, int level)
    {

        ray.origin = ray.origin.subtract(reference_point); // adjust ray origin

        double a = 1;
        double b = 2 * ray.origin.dotProduct(ray.direction);
        double c = ray.origin.dotProduct(ray.origin) - (length * length);

        double discriminant = pow(b, 2) - 4 * a * c;
        double t = -1;
        if (discriminant < 0)
        {
            t = -1;
        }
        else
        {

            if (fabs(a) < 1e-5)
            {
                t = -c / b;
                return t;
            }

            double t1 = (-b - sqrt(discriminant)) / (2 * a);
            double t2 = (-b + sqrt(discriminant)) / (2 * a);

            if (t2 < t1)
                swap(t1, t2);

            if (t1 > 0)
            {
                t = t1;
            }
            else if (t2 > 0)
            {
                t = t2;
            }
            else
            {
                t = -1;
            }
        }

        return t;

    }
};

#endif
