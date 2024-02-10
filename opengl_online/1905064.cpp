#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif
#include <bits/stdc++.h>
using namespace std;
#include "Camera.h"

void drawAxis()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(2, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(0.02, 0, 0);

    glColor3f(0, 2, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0.02, 0);

    glColor3f(0, 0, 2); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 0.02);
    glEnd();
}

Camera camera;
void initGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    camera = Camera();
}

float angle1 = 0;
float speed1 = 0.05;
float angle2 = 0;
float speed2 = 0.08;
float angle3 = 0;
float speed3 = 0.1;

pair<float, float> drawCircle(float cx, float cy, float r, int num, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex3f(x + cx, 0, y + cy);
    }
    glEnd();
    float x, y;

    if (num == 1)
    {
        y = r * sinf(angle1);
        x = r * cosf(angle1);
        angle1 += speed1;
    }
    else if (num == 2)
    {
        y = r * sinf(angle2);
        x = r * cosf(angle2);
        angle2 += speed2;
    }
    else
    {
        y = r * sinf(angle3);
        x = r * cosf(angle3);
        angle3 += speed3;
    }

    glBegin(GL_LINES);   // Begin drawing lines
    glVertex3f(cx, 0, cy); // Start point of the line
    glVertex3f(cx+x, 0, cy+y); // End point of the line
    glEnd();

    return {cx+x, cy+y};
}



void draw()
{
    glColor3f(0.0f, 0.0f, 1.0f);
    pair<float, float> p1 = drawCircle(0, 0, 3, 1, 100);
    glColor3f(0.5f, 0.0f, 0.0f);
    pair<float, float> p2 = drawCircle(p1.first, p1.second, 1.5, 2, 100);
    glColor3f(0.4f, 0.4f, 0.0f);
    pair<float, float> p3 = drawCircle(p2.first, p2.second, 0.7, 3, 100);
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

    draw();

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

        // Control center (location where the eye is looking at)

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
void timer(int value)
{
    // Schedule the next call to timer

    glutTimerFunc(30, timer, 0);

    glutPostRedisplay();
}
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitWindowSize(840, 840);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("magic cube by sayem");
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    initGL();
    glutMainLoop();
    return 0;
}
