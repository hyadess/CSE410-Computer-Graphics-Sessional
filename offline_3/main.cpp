#ifndef MAIN
#define MAIN

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif
#include "bitmap_image.hpp"
#include "Globals.hpp"

#include "Point.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "SpotLight.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "SphereOb.hpp"
#include "TriangleOb.hpp"
#include "GeneralOb.hpp"
#include "FloorOb.hpp"

// global variables..................
vector<Light *> lights;
vector<SpotLight *> spotlights;
vector<Object *> objects;
int recursionLevel;
Camera camera;
int imageWidth, imageHeight;
bitmap_image image;
double windowWidth = 500, windowHeight = 500;
double viewAngle = 80;
int imageCount = 1;
bool draw_axis = false;
bool draw_grid = false;

void loadData()
{
    ifstream in("scene.txt");
    in >> recursionLevel >> imageHeight;

    imageWidth = imageHeight;
    Input input;

    int objCount;
    in >> objCount;

    for (int i = 0; i < objCount; i++)
    {
        string objType;
        in >> objType;

        Object *obj;

        if (objType == "sphere")
        {
            obj = new Sphere();
            input.SphereObInput(in, *((Sphere *)obj));
        }
        else if (objType == "triangle")
        {
            obj = new Triangle();
            input.TriangleObInput(in, *((Triangle *)obj));
        }
        else if (objType == "general")
        {
            obj = new GeneralOb();
            input.GeneralObInput(in, *((GeneralOb *)obj));
        }
        else
        {
            cout << objType << " is not a valid object type" << endl;
        }
        objects.push_back(obj);
    }

    int lightCount;
    in >> lightCount;

    for (int i = 0; i < lightCount; i++)
    {
        Light *light = new Light();
        input.LightInput(in, *light);
        lights.push_back(light);
    }

    int spotlightCount;
    in >> spotlightCount;

    for (int i = 0; i < spotlightCount; i++)
    {
        SpotLight *spotlight = new SpotLight();
        input.SpotLightInput(in, *spotlight);
        spotlights.push_back(spotlight);
    }

    // for(int i=0;i<spotlights.size();i++){
    // 	SpotLight* l = spotlights[i];
    // 	// cout<<"Spotlight "<<spotlight<<endl;

    // 	cout << fixed << setprecision(7) << l->pointLight.pos.x << " " << l->pointLight.pos.y << " " << l->pointLight.pos.z << " " << l->pointLight.color.r << " " << l->pointLight.color.g << " " << l->pointLight.color.b << " " << l->dir.x << " " << l->dir.y << " " << l->dir.z << " " << l->cutoffAngle<<endl;

    // 	// cout<<"Spotlight pos "<<spotlights[i]->pointLight.pos.x<<" "<<spotlights[i]->pointLight.pos.y<<" "<<spotlights[i]->pointLight.pos.z<<endl;
    // 	// cout<<"Spotlight dir "<<spotlights[i]->pointLight.dir.x<<" "<<spotlights[i]->pointLight.dir.y<<" "<<spotlights[i]->pointLight.dir.z<<endl;
    // 	// cout<<"Spotlight cutOff "<<spotlights[i]->pointLight.cutOff<<endl;
    // 	// spotlights[i]->draw();
    // }

    Object *floor;
    floor = new Floor(1000, 20);
    floor->setColor(Color(0.5, 0.5, 0.5));
    // vector <double> coefficients = {1, 1, 1, 1};
    vector<double> coefficients = {0.4, 0.2, 0.2, 0.2};
    // vector <double> coefficients = {0.2, 0.2, 0.2, 0.2};
    floor->setCoefficients(coefficients);
    objects.push_back(floor);
}

void capture()
{
    cout << "Capturing Image" << endl;

    // initialize bitmap image and set background color to black
    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
            image.set_pixel(i, j, 0, 0, 0);

    // image.save_image("black.bmp");

    double planeDistance = (windowHeight / 2.0) / tan((pi * viewAngle) / (360.0));

    // topLeft = pos + (look * planeDistance) + (up * (windowHeight / 2.0)) - (right * (windowWidth / 2.0));

    Point topLeft = camera.pos.add(camera.look.scalarMultiply(planeDistance)).add(camera.up.scalarMultiply(windowHeight / 2.0)).subtract(camera.right.scalarMultiply(windowWidth / 2.0));

    double du = windowWidth / (imageWidth * 1.0);
    double dv = windowHeight / (imageHeight * 1.0);

    // Choose middle of the grid cell

    // topLeft = topLeft + (right * du / 2.0) - (up * dv / 2.0);

    topLeft = topLeft.add(camera.right.scalarMultiply(du / 2.0)).subtract(camera.up.scalarMultiply(dv / 2.0));

    int nearestObjectIndex = -1;
    double t, tMin;

    for (int i = 0; i < imageWidth; i++)
    {
        for (int j = 0; j < imageHeight; j++)
        {
            // calculate current pixel
            // pixel = topLeft + (right * du * i) - (up * dv * j);
            Point pixel = topLeft.add(camera.right.scalarMultiply(du * i)).subtract(camera.up.scalarMultiply(dv * j));

            // cast ray from EYE to (curPixel-eye) direction ; eye is the position of the camera
            Ray ray(camera.pos, pixel.subtract(camera.pos));
            Color color;

            // cout<<"Ray direction "<<ray.dir<<endl;

            // find nearest object
            tMin = -1;
            nearestObjectIndex = -1;
            for (int k = 0; k < (int)objects.size(); k++)
            {
                t = objects[k]->intersect(ray, color, 0, objects, lights, spotlights, recursionLevel);
                if (t > 0 && (nearestObjectIndex == -1 || t < tMin))
                    tMin = t, nearestObjectIndex = k;
            }

            // if nearest object is found, then shade the pixel
            if (nearestObjectIndex != -1)
            {
                // cout<<"Object "<<nearestObjectIndex<<" intersected"<<endl;
                // color = objects[nearestObjectIndex]->color;
                color = Color(0, 0, 0);
                // cout<<"Before Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
                double t = objects[nearestObjectIndex]->intersect(ray, color, 1, objects, lights, spotlights, recursionLevel);

                if (color.r > 1)
                    color.r = 1;
                if (color.g > 1)
                    color.g = 1;
                if (color.b > 1)
                    color.b = 1;

                if (color.r < 0)
                    color.r = 0;
                if (color.g < 0)
                    color.g = 0;
                if (color.b < 0)
                    color.b = 0;

                // cout<<"After Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
                image.set_pixel(i, j, 255 * color.r, 255 * color.g, 255 * color.b);
            }
        }
    }

    image.save_image("Output_" + to_string(imageCount) + ".bmp");
    imageCount++;
    cout << "Saving Image" << endl;
}

int numSegments = 50;

void initGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    draw_axis = true;
    draw_grid = true;
    loadData();
    image = bitmap_image(imageWidth, imageHeight);

    camera = Camera();
}

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxis()
{
    if (draw_axis)
    {
        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(2, 0, 0); // Red
        // X axis
        glVertex3f(100.0, 0, 0);
        glVertex3f(-100.0, 0, 0);

        glColor3f(0, 2, 0); // Green
        // Y axis
        glVertex3f(0, -100.0, 0);
        glVertex3f(0, 100.0, 0);

        glColor3f(0, 0, 2); // Blue
        // Z axis
        glVertex3f(0, 0, 100.0);
        glVertex3f(0, 0, -100.0);
        glEnd();
    }
}

void drawGrid()
{
    int i;
    if (draw_grid)
    {
        glColor3f(0.6, 0.6, 0.6); // grey
        glBegin(GL_LINES);
        {
            for (i = -8; i <= 8; i++)
            {

                if (i == 0)
                    continue; // SKIP the MAIN axes

                // lines parallel to Y-axis
                glVertex3f(i * 10, -90, 0);
                glVertex3f(i * 10, 90, 0);

                // lines parallel to X-axis
                glVertex3f(-90, i * 10, 0);
                glVertex3f(90, i * 10, 0);
            }
        }
        glEnd();
    }
}

void display()
{
    float cx = 1, cy = 1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix

    gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z,
              camera.pos.x + camera.look.x, camera.pos.y + camera.look.y, camera.pos.z + camera.look.z,
              camera.up.x, camera.up.y, camera.up.z);
    // draw
    glRotatef(camera.cameraRotation, 0, 0, 1);

    drawAxis();
    drawGrid();
    for (int i = 0; i < objects.size(); i++)
    {
        Object *object = objects[i];
        object->draw();
    }

    for (int i = 0; i < lights.size(); i++)
    {
        lights[i]->draw();
    }

    for (int i = 0; i < spotlights.size(); i++)
    {
        SpotLight *l = spotlights[i];
        // cout<<"Spotlight "<<spotlight<<endl
        spotlights[i]->draw();
    }

    glutSwapBuffers();
}

void reshapeListener(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
}

void keyboardListener(unsigned char key, int x, int y)
{
    double v = 0.1;
    double rate = 0.1;

    double s;

    switch (key)
    {

    case 'd':
        camera.cameraRotation -= 5.0f;
        break;
    case 'a':
        camera.cameraRotation += 5.0f;
        break;
    case 'w':
        camera.pos.z += v;
        break;
    case 's':
        camera.pos.z -= v;
        break;
    case '0':
        capture();
        break;

    case '1':
        camera.right = camera.right.scalarMultiply(cos(rate)).add(camera.look.scalarMultiply(sin(rate)));
        camera.look = camera.look.scalarMultiply(cos(rate)).subtract(camera.right.scalarMultiply(sin(rate)));
        break;

    case '2':
        camera.right = camera.right.scalarMultiply(cos(-rate)).add(camera.look.scalarMultiply(sin(-rate)));
        camera.look = camera.look.scalarMultiply(cos(-rate)).subtract(camera.right.scalarMultiply(sin(-rate)));
        break;

    case '3':
        camera.look = camera.look.scalarMultiply(cos(rate)).add(camera.up.scalarMultiply(sin(rate)));
        camera.up = camera.up.scalarMultiply(cos(rate)).subtract(camera.look.scalarMultiply(sin(rate)));
        break;

    case '4':
        camera.look = camera.look.scalarMultiply(cos(-rate)).add(camera.up.scalarMultiply(sin(-rate)));
        camera.up = camera.up.scalarMultiply(cos(-rate)).subtract(camera.look.scalarMultiply(sin(-rate)));
        break;

    case '5':
        camera.up = camera.up.scalarMultiply(cos(rate)).add(camera.right.scalarMultiply(sin(rate)));
        camera.right = camera.right.scalarMultiply(cos(rate)).subtract(camera.up.scalarMultiply(sin(rate)));
        break;

    case '6':
        camera.up = camera.up.scalarMultiply(cos(-rate)).add(camera.right.scalarMultiply(sin(-rate)));
        camera.right = camera.right.scalarMultiply(cos(-rate)).subtract(camera.up.scalarMultiply(sin(-rate)));

        break;

    case ',':
        break;
    case '.':
        break;

    case 27:
        exit(0);
        break;
    }

    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: // down arrow key
        camera.pos = camera.pos.add(camera.look);
        break;
    case GLUT_KEY_DOWN: // up arrow key
        camera.pos = camera.pos.subtract(camera.look);
        break;

    case GLUT_KEY_RIGHT:
        camera.pos = camera.pos.add(camera.right);
        break;
    case GLUT_KEY_LEFT:
        camera.pos = camera.pos.subtract(camera.right);
        break;

    case GLUT_KEY_PAGE_UP:
        camera.pos = camera.pos.add(camera.up);
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.pos = camera.pos.subtract(camera.up);
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitWindowSize(840, 840);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Ray Tracing");
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    initGL();
    glutMainLoop();
    objects.clear();
    objects.shrink_to_fit();

    lights.clear();
    lights.shrink_to_fit();

    spotlights.clear();
    spotlights.shrink_to_fit();
    return 0;
}

#endif