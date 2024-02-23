#define GENERALOB
#ifdef GENERALOB
#include "Object.hpp"
#include "Ray.hpp"

class GeneralOb : public Object
{
    double A, B, C, D, E, F, G, H, I, J;

public:
    GeneralOb(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j)
    {
        A = a;
        B = b;
        C = c;
        D = d;
        E = e;
        F = f;
        G = g;
        H = h;
        I = i;
        J = j;
    }
    virtual void draw()
    {
        cout << "GeneralOb" << endl;
    }
    virtual Ray getNormalAt(Point p, Ray incident)
    {
        double x = p.x;
        double y = p.y;
        double z = p.z;
        double nx = 2 * A * x + D * y + E * z + G;
        double ny = 2 * B * y + D * x + F * z + H;
        double nz = 2 * C * z + E * x + F * y + I;
        Point normal = Point(nx, ny, nz);
        normal.normalize();
        Ray normalRay = Ray(p, normal);
        return normalRay;
    }
    bool ok(Point point)
    {
        if (fabs(length) > 1e-5)
        {
            if (point.x < reference_point.x)
                return false;
            if (point.x > reference_point.x + length)
                return false;
        }

        if (fabs(width) > 1e-5)
        {
            if (point.y < reference_point.y)
                return false;
            if (point.y > reference_point.y + width)
                return false;
        }

        if (fabs(height) > 1e-5)
        {
            if (point.z < reference_point.z)
                return false;
            if (point.z > reference_point.z + height)
                return false;
        }

        return true;
    }

    virtual double intersectHelper(Ray ray, Color &color, int level)
    {

        double X0 = ray.origin.x;
        double Y0 = ray.origin.y;
        double Z0 = ray.origin.z;

        double X1 = ray.direction.x;
        double Y1 = ray.direction.y;
        double Z1 = ray.direction.z;

        double C0 = A * X1 * X1 + B * Y1 * Y1 + C * Z1 * Z1 + D * X1 * Y1 + E * X1 * Z1 + F * Y1 * Z1;
        double C1 = 2 * A * X0 * X1 + 2 * B * Y0 * Y1 + 2 * C * Z0 * Z1 + D * (X0 * Y1 + X1 * Y0) + E * (X0 * Z1 + X1 * Z0) + F * (Y0 * Z1 + Y1 * Z0) + G * X1 + H * Y1 + I * Z1;
        double C2 = A * X0 * X0 + B * Y0 * Y0 + C * Z0 * Z0 + D * X0 * Y0 + E * X0 * Z0 + F * Y0 * Z0 + G * X0 + H * Y0 + I * Z0 + J;

        double discriminant = C1 * C1 - 4 * C0 * C2;
        if (discriminant < 0)
            return -1;
        if (fabs(C0) < 1e-5)
        {
            return -C2 / C1;
        }
        double t1 = (-C1 - sqrt(discriminant)) / (2 * C0);
        double t2 = (-C1 + sqrt(discriminant)) / (2 * C0);

        if (t1 < 0 && t2 < 0)
            return -1;

        // cout<<"t1 "<<t1<<" t2 "<<t2<<endl;

        if (t2 < t1)
            swap(t1, t2);

        if (t1 > 0)
        {
            // cout<<"t1 "<<t1<<endl;
            Point intersectionPoint = ray.origin.add(ray.direction.scalarMultiply(t1));
            if (ok(intersectionPoint))
            {
                return t1;
            }
        }
        if (t2 > 0)
        {
            // cout<<"t2 "<<t2<<endl;
            Point intersectionPoint = ray.origin.add(ray.direction.scalarMultiply(t2));
            if (ok(intersectionPoint))
            {
                return t2;
            }
        }

        return -1;
    }
};

#endif