#ifndef CAMERA
#define CAMERA
#include "1905064_Point.hpp"

class Camera
{
public:
    Point pos;   // position of the eye
    Point look;  // look/forward direction
    Point right; // right direction
    Point up;    // up direction
    float cameraRotation = 45.0;
    Camera()
    {
        pos = Point(150, 150, 0);
        look = Point(-1 / sqrt(2), -1 / sqrt(2), 0);
        right = Point(-1 / sqrt(2), 1 / sqrt(2), 0);
        up = Point(0, 0, 1);
    }
};

#endif