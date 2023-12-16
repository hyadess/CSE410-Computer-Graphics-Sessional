#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif
#include <cmath>
#include <iostream>


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
// global variables..................

Camera camera;
int numSegments = 50;
double maxTriangleSide = 2;
double curTriangleSide = 2;

double maxRadius = maxTriangleSide / sqrt(3.0);
double curRadius = 0;
// triangle side becomes 0 from 2.0 at 0.1 increase or decrease rate, so step size should be calculated  dividing 20
double stepSize = maxRadius / 20.0;

void initGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    camera = Camera();
}

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxis()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(2, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(2, 0, 0);

    glColor3f(0, 2, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 2, 0);

    glColor3f(0, 0, 2); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 2);
    glEnd();
}

void drawTriangle()
{

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    }
    glEnd();
}

void drawCylinderSegment(float height, float radius, int segments)
{
    Point points[segments + 1];

    float offset = 70.5287794 * M_PI / 180.0;

    for (int i = 0; i < segments + 1; i++)
    {
        float theta = -offset / 2 + i * offset / segments;
        points[i].x = radius * cos(theta);
        points[i].y = radius * sin(theta);
    }

    glBegin(GL_QUADS);
    for (int i = 0; i < segments; i++)
    {
        glVertex3f(points[i].x, points[i].y, height / 2);
        glVertex3f(points[i].x, points[i].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, height / 2);
    }
    glEnd();
}

void drawSphereQuad(float radius, int segments)
{

    struct Point points[segments + 1][segments + 1];
    int i, j;
    float x, y;
    // generate points
    for (i = 0; i <= segments; i++)
    {

        for (j = 0; j <= segments; j++)
        {
            x = -1 + (float)i / segments * 2;
            y = -1 + (float)j / segments * 2;
            points[i][j].x = x;
            points[i][j].y = y;
            points[i][j].z = 1;

            points[i][j] = points[i][j].normalize();
            points[i][j] = points[i][j].scalarMultiply(radius);
        }
    }
    // draw quads using generated points
    for (i = 0; i < segments; i++)
    {
        for (j = 0; j < segments; j++)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void drawCylinders()
{

    glColor3f(1.0f, 1.0f, 0.0f);

    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {

            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(curTriangleSide / sqrt(2), 0, 0);
            drawCylinderSegment(curTriangleSide * sqrt(2), curRadius, 100);
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            glRotatef(90, 1, 0, 0);
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(curTriangleSide / sqrt(2), 0, 0);
            drawCylinderSegment(curTriangleSide * sqrt(2), curRadius, 100);
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            glRotatef(90, 0, 0, 1);
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(curTriangleSide / sqrt(2), 0, 0);
            drawCylinderSegment(curTriangleSide * sqrt(2), curRadius, 100);
        }
        glPopMatrix();
    }
}

void drawSpheres()
{

    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            if (i % 2 == 1)
                glColor3f(0, 1, 0);

            else
                glColor3f(0, 0, 1);

            glRotatef(90 * i, 0, 1, 0);
            glTranslatef(0, 0, curTriangleSide);
            drawSphereQuad(curRadius, 100);
        }
        glPopMatrix();
    }

    for (int i = 0; i < 2; i++)
    {

        glPushMatrix();
        {
            glColor3f(1.0f, 0.0f, 0.0f); // red
            glRotatef(90 + 180 * i, 1, 0, 0);
            glTranslatef(0, 0, curTriangleSide);
            drawSphereQuad(curRadius, 100);
        }
        glPopMatrix();
    }
}

void drawPyramids()
{
    double diff = maxTriangleSide - curTriangleSide;
    diff = diff / 3.0;
    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            if (i % 2 == 1)
                glColor3f(0, 1, 1);

            else
                glColor3f(1, 0, 1);
            glRotatef(90 * i, 0, 1, 0);
            glTranslatef(diff, diff, diff);
            glScaled(curTriangleSide, curTriangleSide, curTriangleSide);
            drawTriangle();
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            if (i % 2 == 0)
                glColor3f(0, 1, 1);

            else
                glColor3f(1, 0, 1);
            glRotatef(90 * i, 0, 1, 0);
            glRotatef(180, 1, 0, 1);
            glTranslatef(diff, diff, diff);
            glScaled(curTriangleSide, curTriangleSide, curTriangleSide);
            drawTriangle();
        }
        glPopMatrix();
    }
}

void draw()
{
    drawPyramids();
    drawCylinders();
    drawSpheres();
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
{ // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
                                 /*if (width >= height) {
                                     // aspect >= 1, set the height from -1 to 1, with larger width
                                     gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
                                 } else {
                                     // aspect < 1, set the width to -1 to 1, with larger height
                                     gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
                                 }*/
                                 // Enable perspective projection with fovy, aspect, zNear and zFar
    // gluPerspective(45.0f, aspect, 0.1f, 100.0f);
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

    case ',':
        curTriangleSide -= 0.1;
        curRadius += stepSize;
        if (curTriangleSide < 0)
        {
            curTriangleSide = 0;
            curRadius = maxRadius;
        }
        break;
    case '.':
        curTriangleSide += 0.1;
        curRadius -= stepSize;
        if (curTriangleSide > maxTriangleSide)
        {
            curTriangleSide = maxTriangleSide;
            curRadius = 0;
        }
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
    glutCreateWindow("magic cube by sayem");
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    initGL();
    glutMainLoop();
    return 0;
}