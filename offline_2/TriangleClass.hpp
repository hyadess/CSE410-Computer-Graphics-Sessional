#ifndef TRIANGLE
#define TRIANGLE

#include "PointClass.hpp"
#include "Color.hpp"

class Triangle
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

    void sort()
    {
        if (b.y < a.y)
            swap(a, b);
        if (c.y < a.y)
            swap(a, c);
        if (c.y < b.y)
            swap(b, c);
    }

    void recolor()
    {
        color.setColor();
    }
};

#endif