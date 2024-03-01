#ifndef TRIANGLEOB
#define TRIANGLEOB

#include "Object.hpp"
#include "Point.hpp"
#include "Color.hpp"

class Triangle: public Object
{
public:
    Point a, b, c;
    Color color;

    Triangle(Point a, Point b, Point c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    Triangle()
    {
        this->a = Point();
        this->b = Point();
        this->c = Point();
    }

    Point getNormal()
    {
        Point ba = b.subtract(a);
        Point ca = c.subtract(a);
        Point normal = ba.crossProduct(ca);
        normal.normalize();
        return normal;
    }

    void sortPoints()
    {
        if (b.y < a.y)
            swap(a, b);
        if (c.y < a.y)
            swap(a, c);
        if (c.y < b.y)
            swap(b, c);
    }

    virtual Ray getNormalAt(Point point, Ray incidentRay)
    {
        Point normal = getNormal();
        normal.normalize();

        if (incidentRay.direction.dotProduct(normal) < 0)
        {
            return Ray(point, normal.scalarMultiply(-1.0));
        }
        else
        {
            return Ray(point, normal);
        }
    }

    virtual void draw()
    {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    string returnType()
    {
        return "Triangle";
    }
    virtual double getIntersectingT(Ray ray, Color &color, int level)
    {

        double betaMat[3][3] = {
            {a.x - ray.origin.x, a.x - c.x, ray.direction.x},
            {a.y - ray.origin.y, a.y - c.y, ray.direction.y},
            {a.z - ray.origin.z, a.z - c.z, ray.direction.z}};
        double gammaMat[3][3] = {
            {a.x - b.x, a.x - ray.origin.x, ray.direction.x},
            {a.y - b.y, a.y - ray.origin.y, ray.direction.y},
            {a.z - b.z, a.z - ray.origin.z, ray.direction.z}};
        double tMat[3][3] = {
            {a.x - b.x, a.x - c.x, a.x - ray.origin.x},
            {a.y - b.y, a.y - c.y, a.y - ray.origin.y},
            {a.z - b.z, a.z - c.z, a.z - ray.origin.z}};
        double AMat[3][3]{
            {a.x - b.x, a.x - c.x, ray.direction.x},
            {a.y - b.y, a.y - c.y, ray.direction.y},
            {a.z - b.z, a.z - c.z, ray.direction.z}};

        double Adet = determinant(AMat);
        double beta = determinant(betaMat) / Adet;
        double gamma = determinant(gammaMat) / Adet;
        double t = determinant(tMat) / Adet;

        if (beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0)
        {
            return t;
        }
        else
        {
            return -1;
        }
    }
};

#endif