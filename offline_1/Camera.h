#ifndef CAMERA 
#define CAMERA
#include "Point.h"

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
        pos = Point(5, 0, 2);
        look = Point(-1, 0, 0);
        right = Point(0, 1, 0);
        up = Point(0, 0, 1);
    }
};

#endif