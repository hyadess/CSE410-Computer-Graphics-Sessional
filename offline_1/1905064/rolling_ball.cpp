#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif
#include <bits/stdc++.h>
using namespace std;

#include "Camera.h"
// global variables..................

Camera camera;

float rotateVectorX = 1;
float rotateVectorY = -1;
float rotationVectorChangeSpeed = 10; // in degree angle
float positionX = 0.0;
float positionY = 0.0;
float radius = 0.5;
float rotationSpeed = 20; // in degree angle
float angle = 0;
// the box border vectors and their normal vectors..........................
float centerX = 0.0;
float centerY = 0.0;
int noOfBorders = 6;
// float borderXcordinates[] = {-6.0, -6.0, 6.0, 6.0};
// float borderYcordinates[] = {-6.0, 6.0, 6.0, -6.0};
// float normalXs[] = {0, 0, 0, 0};
// float normalYs[] = {0, 0, 0, 0};

// float borderXcordinates[] = {-6.0, -6.0, 6.0, 6.0, 0.0};
// float borderYcordinates[] = {-6.0, 6.0, 6.0, -6.0, -12.0};
// float normalXs[] = {0, 0, 0, 0, 0};
// float normalYs[] = {0, 0, 0, 0, 0};

float borderXcordinates[] = {-6.0, -6.0, 0.0, 6.0, 6.0, 0.0};
float borderYcordinates[] = {-6.0, 6.0, 12.0, 6.0, -6.0, -12.0};
float normalXs[] = {0, 0, 0, 0, 0, 0};
float normalYs[] = {0, 0, 0, 0, 0, 0};

bool prevDir = true;
bool postDir = true;
bool simulationMode = false;
bool shouldCheck = false;

void calculateBoxNormals()
{
    for (int i = 0; i < noOfBorders; i++)
    {
        float x = borderXcordinates[(i + 1) % noOfBorders] - borderXcordinates[i];
        float y = borderYcordinates[(i + 1) % noOfBorders] - borderYcordinates[i];
        float normalized = sqrt(x * x + y * y);
        normalXs[i] = y / normalized;
        normalYs[i] = -x / normalized;
    }
}

// checkboard.................................

void drawCheckBoard()
{
    int u = 0;
    glPushMatrix();
    glTranslatef(0, 0, -radius);
    for (int i = -100; i <= 100; i += 3)
    {
        for (int j = -100; j <= 100; j += 3)
        {
            if (u == 0)
                glColor3f(0, 0, 0);
            else
                glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            {
                glVertex3f(i, j, 0);
                glVertex3f(i, j + 3, 0);
                glVertex3f(i + 3, j + 3, 0);
                glVertex3f(i + 3, j, 0);
            }
            glEnd();

            u = (u + 1) % 2;
        }
    }

    glPopMatrix();
}

void drawBorders()
{
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -radius);
    for (int i = 0; i < noOfBorders; i++)
    {
        glBegin(GL_QUADS);
        {
            glVertex3f(borderXcordinates[i], borderYcordinates[i], 0);
            glVertex3f(borderXcordinates[(i + 1) % noOfBorders], borderYcordinates[(i + 1) % noOfBorders], 0);
            glVertex3f(borderXcordinates[(i + 1) % noOfBorders], borderYcordinates[(i + 1) % noOfBorders], 0.8f);
            glVertex3f(borderXcordinates[i], borderYcordinates[i], 0.8f);
        }
        glEnd();
    }
    glPopMatrix();
}

// collision detection
float nextCollisionTime; // for event driven

void collisionChange()
{

    // px and py represents direction vector..................m represents the slope of that vector...
    float px = rotateVectorY;
    float py = -rotateVectorX;
    if (px == 0.0)
        px = 0.01;
    float m = py / px;

    // at first, find the collision border

    float length = 100000.0;
    int b = -1;

    for (int i = 0; i < noOfBorders; i++)
    {
        float x1 = borderXcordinates[i], y1 = borderYcordinates[i];
        float x2 = borderXcordinates[(i + 1) % noOfBorders], y2 = borderYcordinates[(i + 1) % noOfBorders];

        float x, y, m1;
        if (x1 != x2)
        {
            m1 = (y2 - y1) / (x2 - x1);

            x = (m1 * x1 - y1 - m * positionX + positionY) / (m1 - m);

            y = m1 * (x - x1) + y1;
        }
        else
        {
            x = x1;
            y = m * (x - positionX) + positionY;
        }

        // we calculated the intersecting point....

        if ((px > 0 && x < positionX) || (px < 0 && x > positionX) || (py > 0 && y < positionY) || (py < 0 && y > positionY))
            continue; // intersecting point is at the other direction of the vector, so, impossible to collide.

        // ensure that, the first coliding border is taken.
        if (length > sqrt((x - positionX) * (x - positionX) + (y - positionY) * (y - positionY)))
        {

            length = sqrt((x - positionX) * (x - positionX) + (y - positionY) * (y - positionY));
            b = i;
            // printf("%f %f %d\n", m, length, i);
        }
    }
    // predict next collision time.........................
    float movementLength = 2 * M_PI * radius * rotationSpeed / 360.0;
    nextCollisionTime = 20 * (length - radius) / movementLength; // refresh is done after every 20ms, so movementlength is done at 20ms
    // printf("%f\n", nextCollisionTime);
    if (b == -1 || length > 2.0 * radius)
    {
        return; // no collision found
    }

    // for reflection, we need a-2(a*n)n
    // a*n
    float dot = px * normalXs[b] + py * normalYs[b];
    px = px - 2 * dot * normalXs[b];
    py = py - 2 * dot * normalYs[b];
    rotateVectorX = -py;
    rotateVectorY = px;
}

// ball rotational movement........................( when the rotation axis is done calculating)

void rotationVectorChange(bool positive)
{
    float currentAngle = atanf(fabsf(rotateVectorY) / fabsf(rotateVectorX));
    if (rotateVectorX < 0 && rotateVectorY > 0)
        currentAngle = M_PI - currentAngle;
    else if (rotateVectorX < 0 && rotateVectorY < 0)
        currentAngle = M_PI + currentAngle;
    else if (rotateVectorX > 0 && rotateVectorY < 0)
        currentAngle = 2 * M_PI - currentAngle;

    if (positive)
        currentAngle += rotationVectorChangeSpeed * M_PI / 180.0;
    else
        currentAngle -= rotationVectorChangeSpeed * M_PI / 180.0;
    float length = sqrtf(rotateVectorX * rotateVectorX + rotateVectorY * rotateVectorY);
    rotateVectorX = length * cosf(currentAngle);
    rotateVectorY = length * sinf(currentAngle);
}

void sphereRotation()
{

    // Apply rotation using Rodrigues' formula
    glRotatef(angle, rotateVectorX, rotateVectorY, 0);
    // printf("%f\n",angle);
}

// ball linear movement..........................( when the rotation axis is done calculating)

float findDirection()
{
    float x = rotateVectorY;
    float y = -rotateVectorX;
    float directionAngle = atanf(fabsf(y) / fabsf(x));
    if (x < 0 && y > 0)
        directionAngle = M_PI - directionAngle;
    else if (x < 0 && y < 0)
        directionAngle = M_PI + directionAngle;
    else if (x > 0 && y < 0)
        directionAngle = 2 * M_PI - directionAngle;

    return directionAngle;
}

void movement() // update the position of the ball
{
    float directionAngle = findDirection();
    float movementLength = 2 * M_PI * radius * rotationSpeed / 360.0;
    float moveX = movementLength * cosf(directionAngle);
    float moveY = movementLength * sinf(directionAngle);
    positionX += moveX;
    positionY += moveY;
}

void sphereTranslation()
{

    glTranslatef(positionX, positionY, 0);
}

// draw the ball.........................

void drawSphere(double radius, double partCount)
{
    int stackCount = partCount;
    int sectorCount = partCount;

    // point generation..............................................................

    struct Point points[stackCount + 1][sectorCount + 1];

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep; // starting from pi/2 to -pi/2

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {

            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)

            points[i][j].z = radius * sinf(stackAngle); // r * sin(u)
            float xy = radius * cosf(stackAngle);       // r * cos(u)
            points[i][j].x = xy * cosf(sectorAngle);    // r * cos(u) * cos(v)
            points[i][j].y = xy * sinf(sectorAngle);    // r * cos(u) * sin(v)
        }
    }
    glColor3f(1, 0, 0);
    int cont = 10, f = 1;
    for (int i = 0; i < stackCount; i++)
    {
        for (int j = 0; j < sectorCount; j++)
        {
            cont--;
            if (cont == 0)
            {

                f = (f + 1) % 2;
                cont = 10;
            }
            if (i * 2 < stackCount)
            {
                if (f == 1)
                    glColor3f(0, 1, 0);
                else
                    glColor3f(1, 0, 0);
            }
            else
            {
                if (f == 1)
                    glColor3f(1, 0, 0);
                else
                    glColor3f(0, 1, 0);
            }

            if (i != 0)
            {
                glBegin(GL_TRIANGLES);
                {
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                }
                glEnd();
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                glBegin(GL_TRIANGLES);
                {
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                }
                glEnd();
            }
        }
    }
}

void sphere()
{
    collisionChange();

    glPushMatrix();
    sphereTranslation();

    glPushMatrix();
    sphereRotation();
    drawSphere(radius, 100);
    glPopMatrix();

    glColor3f(0, 0, 1);
    glLineWidth(5.0f);
    float px, py;
    if (postDir == true)
    {
        px = rotateVectorY;
        py = -rotateVectorX;
    }
    else
    {
        px = -rotateVectorY;
        py = rotateVectorX;
    }
    float normalized = sqrt(px * px + py * py);
    // the direction line..............
    glBegin(GL_LINES);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.5 * px / normalized, 1.5 * py / normalized, 0.0f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex3f(1.2 * px / normalized, 1.2 * py / normalized, 0.2f);
    glVertex3f(1.5 * px / normalized, 1.5 * py / normalized, 0.0f);
    glVertex3f(1.2 * px / normalized, 1.2 * py / normalized, -0.2f);

    glEnd();

    glPopMatrix();
}

void display()
{
    float cx = 1, cy = 1;
    if (prevDir != postDir) // k is just pressed..........
    {
        rotateVectorX = -rotateVectorX;
        rotateVectorY = -rotateVectorY;
        prevDir = postDir;
    }
    if (simulationMode)
    {
        angle = angle + rotationSpeed;
        if (angle >= 360.0)
            angle = angle - 360.0;
        movement();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z,
              camera.pos.x + camera.look.x, camera.pos.y + camera.look.y, camera.pos.z + camera.look.z,
              camera.up.x, camera.up.y, camera.up.z);
    // draw
    glRotatef(camera.cameraRotation, 0, 0, 1);

    // drawAxes();
    drawBorders();
    sphere();
    // drawGrid();

    drawCheckBoard();
    shouldCheck = false;

    glutSwapBuffers(); // Render now
}

void reshapeListener(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1; // To prevent divide by 0
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboardListener(unsigned char key, int x, int y)
{
    double v = 0.1;
    double rate = 0.1;

    switch (key)
    {
    case ' ':
        simulationMode = !simulationMode;
        break;
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
    case 'l':
        rotationVectorChange(true);
        break;
    case 'j':
        rotationVectorChange(false);
        break;
    case 'i':
        if (simulationMode)
            break;
        postDir = true;
        angle = angle + rotationSpeed;
        if (angle >= 360.0)
            angle = angle - 360.0;
        movement();
        shouldCheck = true;
        break;
    case 'k':
        if (simulationMode)
            break;
        postDir = false;
        angle = angle + rotationSpeed;
        if (angle >= 360.0)
            angle = angle - 360.0;
        movement();
        shouldCheck = true;
        break;

    case 27:
        exit(0);
        break;
    }

    glutPostRedisplay(); // Post a paint request to activate display()
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
    if (simulationMode || shouldCheck)
        collisionChange();

    glutTimerFunc(min(20.0f, nextCollisionTime), timer, 0);

    glutPostRedisplay();
}
void initGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    camera = Camera();

    calculateBoxNormals();
    rotationVectorChange(true);
}
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitWindowSize(840, 840);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("rolling ball by sayem");

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    initGL();
    glutMainLoop();
    return 0;
}