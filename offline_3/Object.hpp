#ifndef OBJECT
#define OBJECT
#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif
#include <bits/stdc++.h>
using namespace std;
#define pi (2 * acos(0.0))
#include "Point.hpp"
#include "Ray.hpp"
#include "Color.hpp"
#include "Light.hpp"
#include "SpotLight.hpp"
#include "CommonOP.hpp"
class Object;
class Object
{
public:
    Point reference_point;
    double height, width, length;
    Color color;
    vector<double> coefficients; // ambient, diffuse, specular, reflection coefficients
    int shine;                   // exponent term of specular component

    Object()
    {
        reference_point = Point();
        height = 0;
        width = 0;
        length = 0;
        color = Color();
        coefficients = vector<double>(4, 0);
        shine = 0;
    }
    Object(Point reference_point, double height, double width, double length, Color color, vector<double> coefficients, int shine)
    {
        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
        this->color = color;
        this->coefficients = coefficients;
        this->shine = shine;
    }
    void setCoefficients(vector<double> coefficients)
    {
        this->coefficients = coefficients;
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    void setColor(Color color)
    {
        this->color = color;
    }

    virtual string returnType()
    {
        return "Object";
    }
    virtual Color getColorAt(Point p)
    {

        return Color(this->color.r, this->color.g, this->color.b);
    }
    virtual void draw() = 0;
    virtual double getIntersectingT(Ray r, Color &color, int level) = 0;
    virtual Ray getNormalAt(Point p, Ray incient) = 0;

    virtual double intersect(Ray ray, Color &color, int level,
                             vector<Object *> &objects, vector<Light *> &lights, vector<SpotLight *> &spotlights, int recursionLevel = 3)
    {
        double t = getIntersectingT(ray, color, level);

        // less than one mean the object is behind the origin of the ray..................so no intersection
        if (t < 0)
            return -1;

        if (level == 0)
            return t;

        // get color and point of the intersection
        //  p=p0+td
        Point intersectionPoint = ray.origin.add(ray.direction.scalarMultiply(t));

        Color colorAtIntersection = getColorAt(intersectionPoint);



        // update color with ambience (thing will become dimmer)
        color.r = colorAtIntersection.r * coefficients[0];
        color.g = colorAtIntersection.g * coefficients[0];
        color.b = colorAtIntersection.b * coefficients[0];

        // cout<< " Lights size " << lights.size() << endl;

        for (int i = 0; i < lights.size(); i++)
        {

            Point lightPosition = lights[i]->position;
            // td = p-p0
            Point lightDirection = intersectionPoint.subtract(lightPosition);
            // td --> d
            lightDirection.normalize();

            // cast incident ray, from light position to intersection point
            Ray lightRay = Ray(lightPosition, lightDirection);

            // calculate normal at intersectionPoint
            Ray normal = getNormalAt(intersectionPoint, lightRay);

            /**
             * @brief check if incedent ray is not obstructed by any other object
             *
             */

            double t2 = intersectionPoint.subtract(lightPosition).value();
            if (t2 < 1e-5)
                continue;

            bool obscured = false;

            for (Object *obj : objects)
            {
                double t3 = obj->getIntersectingT(lightRay, color, 0);
                if (t3 > 0 && t3 + 1e-5 < t2)
                {
                    obscured = true;
                    break;
                }
            }

            if (!obscured)
            {

                // lambert value
                double val = max(0.0, -lightRay.direction.dotProduct(normal.direction));

                // find reflected ray
                Ray reflection = Ray(intersectionPoint, Reflection(normal.direction, lightRay.direction));
                double phong = max(0.0, -ray.direction.dotProduct(reflection.direction));

                // update diffuse and specular components
                // lights[i]->color works as the source intensity, Is here

                color.r += lights[i]->color.r * coefficients[1] * val * colorAtIntersection.r;
                color.r += lights[i]->color.r * coefficients[2] * pow(phong, shine) * colorAtIntersection.r;

                color.g += lights[i]->color.g * coefficients[1] * val * colorAtIntersection.g;
                color.g += lights[i]->color.g * coefficients[2] * pow(phong, shine) * colorAtIntersection.g;

                color.b += lights[i]->color.b * coefficients[1] * val * colorAtIntersection.b;
                color.b += lights[i]->color.b * coefficients[2] * pow(phong, shine) * colorAtIntersection.b;
            }
        }

        /**
         * @brief same calculation as above, but for spotlights
         * Do until ray cast from light_pos to intersectionPoint exceeds cutoff-angle for the light source
         */

        for (int i = 0; i < spotlights.size(); i++)
        {

            Point lightPosition = spotlights[i]->light.position;
            Point lightDirection = intersectionPoint.subtract(lightPosition);
            lightDirection.normalize();

            double dot = lightDirection.dotProduct(spotlights[i]->direction);
            double angle = acos(dot / (lightDirection.value() * spotlights[i]->direction.value())) * (180.0 / pi);

            if (fabs(angle) <= spotlights[i]->cutoff)
            {

                Ray lightRay = Ray(lightPosition, lightDirection);
                Ray normal = getNormalAt(intersectionPoint, lightRay);

                Ray reflection = Ray(intersectionPoint, Reflection(normal.direction, lightRay.direction));

                double t2 = intersectionPoint.subtract(lightPosition).value();
                if (t2 < 1e-5)
                    continue;

                bool obscured = false;

                for (Object *obj : objects)
                {
                    double t3 = obj->getIntersectingT(lightRay, color, 0);
                    if (t3 > 0 && t3 + 1e-5 < t2)
                    {
                        obscured = true;
                        break;
                    }
                }

                if (!obscured)
                {

                    double phong = max(0.0, -ray.direction.dotProduct(reflection.direction));
                    double val = max(0.0, -lightRay.direction.dotProduct(normal.direction));

                    color.r += spotlights[i]->light.color.r * coefficients[1] * val * colorAtIntersection.r;
                    color.r += spotlights[i]->light.color.r * coefficients[2] * pow(phong, shine) * colorAtIntersection.r;

                    color.g += spotlights[i]->light.color.g * coefficients[1] * val * colorAtIntersection.g;
                    color.g += spotlights[i]->light.color.g * coefficients[2] * pow(phong, shine) * colorAtIntersection.g;

                    color.b += spotlights[i]->light.color.b * coefficients[1] * val * colorAtIntersection.b;
                    color.b += spotlights[i]->light.color.b * coefficients[2] * pow(phong, shine) * colorAtIntersection.b;
                }
            }
        }

        /**
         * @brief RECURSIVE REFLECTION
         *
         */

        if (level < recursionLevel)
        {
            // if(level > 1) cout << "Recursion level " << level << endl;

            // find normal at intersectionPoint
            Ray normal = getNormalAt(intersectionPoint, ray);

            // find reflected ray
            Ray reflectionRay = Ray(intersectionPoint, Reflection(normal.direction, ray.direction));

            /**
             * @brief slightly forward from the point
             * (by moving the start a little bit towards the reflection direction)
             * to avoid self intersection
             *
             */
            reflectionRay.origin = reflectionRay.origin.add(reflectionRay.direction.scalarMultiply(1e-5));

            // find nearest intersection object and do recursive call

            int nearestObjectIndex = -1;
            double t = -1, tMin = 1e9;

            for (int k = 0; k < (int)objects.size(); k++)
            {
                t = objects[k]->intersect(reflectionRay, color, 0, objects, lights, spotlights, recursionLevel);
                if (t > 0 && t < tMin)
                    tMin = t, nearestObjectIndex = k;
            }

            if (nearestObjectIndex != -1)
            {
                // cout<<"Object "<<nearestObjectIndex<<" intersected"<<endl;

                Color colorTemp(0, 0, 0); // refelction color
                // cout<<"Before Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
                double t = objects[nearestObjectIndex]->intersect(reflectionRay, colorTemp, level + 1, objects, lights, spotlights, recursionLevel);

                // colorTemp will be updated while in the subsequent call
                // update color using the impact of reflection

                color.r += colorTemp.r * coefficients[3];
                color.g += colorTemp.g * coefficients[3];
                color.b += colorTemp.b * coefficients[3];
            }

            // PT reflection = lightDirection - 2*(lightDirection*normal)*normal;
            // reflection.normalize();
            // double diffuse = max(0.0, lightDirection*normal);
            // double specular = pow(max(0.0, reflection*ray.dir), shine);
            // color.r += colorAtIntersection.r * coefficients[1] * diffuse + colorAtIntersection.r * coefficients[2] * specular;
            // color.g += colorAtIntersection.g * coefficients[1] * diffuse + colorAtIntersection.g * coefficients[2] * specular;
            // color.b += colorAtIntersection.b * coefficients[1] * diffuse + colorAtIntersection.b * coefficients[2] * specular;
        }

        return t;
    }
    // destructor
    virtual ~Object()
    {
        coefficients.clear();
        coefficients.shrink_to_fit();
    }
};

#endif