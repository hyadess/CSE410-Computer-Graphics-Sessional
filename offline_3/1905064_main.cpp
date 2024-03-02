#ifndef MAIN
#define MAIN

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif
#include "bitmap_image.hpp"
#include "1905064_Globals.hpp"

#include "1905064_Point.hpp"
#include "1905064_Color.hpp"
#include "1905064_Ray.hpp"
#include "1905064_Object.hpp"
#include "1905064_Light.hpp"
#include "1905064_SpotLight.hpp"
#include "1905064_Camera.hpp"
#include "1905064_Input.hpp"
#include "1905064_SphereOb.hpp"
#include "1905064_TriangleOb.hpp"
#include "1905064_GeneralOb.hpp"
#include "1905064_FloorOb.hpp"

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
    ifstream in("input/scene.txt");
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
            cout << objType << " is not a valid object" << endl;
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

    Object *floor;
    floor = new Floor(1000, 20);
    floor->setColor(Color(0.5, 0.5, 0.5));
    vector<double> coefficients = {0.4, 0.2, 0.2, 0.2};
    floor->setCoefficients(coefficients);
    objects.push_back(floor);
}

void capture()
{
    cout << "Image Capturing started" << endl;

    for (int i = 0; i < imageWidth; i++)
        for (int j = 0; j < imageHeight; j++)
            image.set_pixel(i, j, 0, 0, 0);

    double planeDistance = (windowHeight / 2.0) / tan((pi * viewAngle) / (360.0));

    // topLeft = pos + (look * planeDistance) + (up * (windowHeight / 2.0)) - (right * (windowWidth / 2.0));

    Point topLeft = camera.pos.add(camera.look.scalarMultiply(planeDistance)).add(camera.up.scalarMultiply(windowHeight / 2.0)).subtract(camera.right.scalarMultiply(windowWidth / 2.0));

    double du = windowWidth / (imageWidth * 1.0);
    double dv = windowHeight / (imageHeight * 1.0);
    int obIndex = -1;
    double t, min_t;

    // topLeft = topLeft + (right * du / 2.0) - (up * dv / 2.0);

    topLeft = topLeft.add(camera.right.scalarMultiply(du / 2.0)).subtract(camera.up.scalarMultiply(dv / 2.0));

    for (int i = 0; i < imageWidth; i++)
    {
        for (int j = 0; j < imageHeight; j++)
        {
            // calculate current pixel
            // pixel = topLeft + (right * du * i) - (up * dv * j);
            Point pixel = topLeft.add(camera.right.scalarMultiply(du * i)).subtract(camera.up.scalarMultiply(dv * j));

            Ray ray(camera.pos, pixel.subtract(camera.pos));
            Color color;

            // find nearest object
            min_t = -1;
            obIndex = -1;
            for (int k = 0; k < (int)objects.size(); k++)
            {

                //0 means I only need the t value
                t = objects[k]->intersect(ray, color, 0, objects, lights, spotlights, recursionLevel);
                if (t > 0 && (obIndex == -1 || t < min_t))
                    min_t = t, obIndex = k;
            }

            // if nearest object is found, then shade the pixel
            if (obIndex != -1)
            {

                color = Color(0, 0, 0);

                double t = objects[obIndex]->intersect(ray, color, 1, objects, lights, spotlights, recursionLevel);

                // normalize the color values...................
                colorNormallize(color);
                image.set_pixel(i, j, 255 * color.r, 255 * color.g, 255 * color.b);
            }
        }
    }

    image.save_image("output/Output_1" + to_string(imageCount) + ".bmp");
    imageCount++;
    cout << "Saving Image" << endl;
}

int numSegments = 100;

void initGL()
{

    draw_axis = true;
    draw_grid = true;
    loadData();
    image = bitmap_image(imageWidth, imageHeight);

    camera = Camera();

    // clear the screen
    glClearColor(0, 0, 0, 0);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
}

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
        glColor3f(0.6, 0.6, 0.6);
        glBegin(GL_LINES);
        {
            for (i = -8; i <= 8; i++)
            {

                if (i == 0)
                    continue;

                glVertex3f(i * 10, -90, 0);
                glVertex3f(i * 10, 90, 0);

                glVertex3f(-90, i * 10, 0);
                glVertex3f(90, i * 10, 0);
            }
        }
        glEnd();
    }
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z,
              camera.pos.x + camera.look.x, camera.pos.y + camera.look.y, camera.pos.z + camera.look.z,
              camera.up.x, camera.up.y, camera.up.z);
    // draw
    glRotatef(camera.cameraRotation, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

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
        // camera.cameraRotation -= 5.0f;
        break;
    case 'a':
        // camera.cameraRotation += 5.0f;
        break;
    case 'w':
        // camera.pos.z += v;
        break;
    case 's':
        // camera.pos.z -= v;
        break;
    case '0':
        capture();
        break;

    case '1':
        camera.look = camera.look.scalarMultiply(cos(rate)).subtract(camera.right.scalarMultiply(sin(rate)));
        camera.right = camera.right.scalarMultiply(cos(rate)).add(camera.look.scalarMultiply(sin(rate)));

        break;

    case '2':
        camera.look = camera.look.scalarMultiply(cos(-rate)).subtract(camera.right.scalarMultiply(sin(-rate)));
        camera.right = camera.right.scalarMultiply(cos(-rate)).add(camera.look.scalarMultiply(sin(-rate)));

        break;

    case '4':
        camera.look = camera.look.scalarMultiply(cos(rate)).subtract(camera.up.scalarMultiply(sin(rate)));
        camera.up = camera.up.scalarMultiply(cos(rate)).add(camera.look.scalarMultiply(sin(rate)));
        break;

    case '3':
        camera.look = camera.look.scalarMultiply(cos(-rate)).subtract(camera.up.scalarMultiply(sin(-rate)));
        camera.up = camera.up.scalarMultiply(cos(-rate)).add(camera.look.scalarMultiply(sin(-rate)));
        break;

    case '5':
        camera.up = camera.up.scalarMultiply(cos(rate)).subtract(camera.right.scalarMultiply(sin(rate)));
        camera.right = camera.right.scalarMultiply(cos(rate)).add(camera.up.scalarMultiply(sin(rate)));
        break;

    case '6':
        camera.up = camera.up.scalarMultiply(cos(-rate)).subtract(camera.right.scalarMultiply(sin(-rate)));
        camera.right = camera.right.scalarMultiply(cos(-rate)).add(camera.up.scalarMultiply(sin(-rate)));

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
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Ray Tracing");

    initGL();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    // glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

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