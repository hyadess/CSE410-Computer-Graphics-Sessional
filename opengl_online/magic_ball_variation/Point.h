#ifndef POINT 
#define POINT
#include <cmath>

class Point
{
public:
    float x, y, z;
    Point(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }
    void set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point add(Point b)
    {
        return Point(x + b.x, y + b.y, z + b.z);
    }
    Point subtract(Point b)
    {
        return Point(x - b.x, y - b.y, z - b.z);
    }
    Point scalarMultiply(float b)
    {
        return Point(x * b, y * b, z * b);
    }
    Point scalarDivide(float b)
    {
        return Point(x / b, y / b, z / b);
    }
    Point crossMultiply(Point b)
    {
        return Point(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    }
    float dotMUltiply(Point b)
    {
        return x * b.x + y * b.y + z * b.z;
    }
    float length()
    {
        return sqrt(x * x + y * y + z * z);
    }
    Point normalize()
    {
        return Point(x / this->length(), y / this->length(), z / this->length());
    }
};


#endif