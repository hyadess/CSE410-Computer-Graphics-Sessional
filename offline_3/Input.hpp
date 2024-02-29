#ifndef INPUT
#define INPUT

#include <iostream>
#include "Object.hpp"
#include "Point.hpp"
#include "Light.hpp"
#include "SpotLight.hpp"

#include "GeneralOb.hpp"
#include "Ray.hpp"
#include "SphereOb.hpp"
#include "TriangleOb.hpp"

class Input
{
public:
    void PointInput(istream &in, Point &p)
    {
        in >> p.x >> p.y >> p.z;
    }

    void LightInput(istream &in, Light &l)
    {
        in >> l.position.x >> l.position.y >> l.position.z;
        in >> l.color.r >> l.color.g >> l.color.b;
    }

    void SpotLightInput(istream &in, SpotLight &sl)
    {
        in >> sl.light.position.x >> sl.light.position.y >> sl.light.position.z;
        in >> sl.light.color.r >> sl.light.color.g >> sl.light.color.b;
        in >> sl.direction.x >> sl.direction.y >> sl.direction.z;
        in >> sl.cutoff;
    }

    void RayInput(istream &in, Ray &r)
    {
        in >> r.origin.x >> r.origin.y >> r.origin.z;
        in >> r.direction.x >> r.direction.y >> r.direction.z;
    }

    void GeneralObInput(istream &in, GeneralOb &ob)
    {
        in >> ob.A >> ob.B >> ob.C >> ob.D >> ob.E >> ob.F >> ob.G >> ob.H >> ob.I >> ob.J;
        in >> ob.reference_point.x >> ob.reference_point.y >> ob.reference_point.z;
        in >> ob.length >> ob.width >> ob.height;
        in >> ob.color.r >> ob.color.g >> ob.color.b;
        in >> ob.coefficients[0] >> ob.coefficients[1] >> ob.coefficients[2] >> ob.coefficients[3];
        in >> ob.shine;
    }

    void SphereObInput(istream &in, Sphere &ob)
    {
        in >> ob.reference_point.x >> ob.reference_point.y >> ob.reference_point.z;
        in >> ob.length;
        in >> ob.color.r >> ob.color.g >> ob.color.b;
        in >> ob.coefficients[0] >> ob.coefficients[1] >> ob.coefficients[2] >> ob.coefficients[3];
        in >> ob.shine;
    }

    void TriangleObInput(istream &in, Triangle &ob)
    {
        in >> ob.a.x >> ob.a.y >> ob.a.z;
        in >> ob.b.x >> ob.b.y >> ob.b.z;
        in >> ob.c.x >> ob.c.y >> ob.c.z;
        in >> ob.color.r >> ob.color.g >> ob.color.b;
        in >> ob.coefficients[0] >> ob.coefficients[1] >> ob.coefficients[2] >> ob.coefficients[3];
        in >> ob.shine;
    }
};
#endif