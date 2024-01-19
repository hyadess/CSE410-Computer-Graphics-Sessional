#ifndef MAIN
#define MAIN

#include <bits/stdc++.h>
using namespace std;
#include "PointClass.hpp"
#include "MatrixClass.hpp"
#include "Stage1MatrixGen.hpp"
#include "Stage2MatrixGen.hpp"
#include "Stage3MatrixGen.hpp"
#include "clippingAndScanning.hpp"

int main()
{
    /// STAGE 1 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    ifstream fin("./spec/IOs/1/scene.txt");
    ofstream fout("./outputs/stage1.txt");

    Point eye, look, up;
    double fovY, ratio, near, far;
    fin >> eye.x >> eye.y >> eye.z;
    fin >> look.x >> look.y >> look.z;
    fin >> up.x >> up.y >> up.z;
    fin >> fovY >> ratio >> near >> far;

    Matrix currMatrix;
    stack<Matrix> s;
    s.push(currMatrix);

    string command;
    fout << std::fixed << std::setprecision(7);
    int triangle = 0;

    while (1)
    {
        // cout << 1;
        fin >> command;
        if (command == "triangle")
        {
            Point p, q, r;
            fin >> p.x >> p.y >> p.z;
            fin >> q.x >> q.y >> q.z;
            fin >> r.x >> r.y >> r.z;

            p = currMatrix.pointMultiplication(p);
            q = currMatrix.pointMultiplication(q);
            r = currMatrix.pointMultiplication(r);

            fout << p.x << " " << p.y << " " << p.z << endl;
            fout << q.x << " " << q.y << " " << q.z << endl;
            fout << r.x << " " << r.y << " " << r.z << endl;

            fout << endl;
            triangle++;
        }
        else if (command == "translate")
        {
            double tx, ty, tz;
            fin >> tx >> ty >> tz;
            // cout << tx << ty << tz;
            Matrix translateMatrix = translationMatrixGen(tx, ty, tz);
            currMatrix = currMatrix.multiplication(translateMatrix);
            cout << "after translation" << endl;
            currMatrix.print();
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            fin >> sx >> sy >> sz;
            Matrix scaleMatrix = scalingMatrixGen(sx, sy, sz);
            currMatrix = currMatrix.multiplication(scaleMatrix);
            cout << "after scaling" << endl;
            currMatrix.print();
        }
        else if (command == "rotate")
        {
            double angle, ax, ay, az;
            fin >> angle >> ax >> ay >> az;
            Matrix rotateMatrix = rotationMatrixGen(ax, ay, az, angle);
            currMatrix = currMatrix.multiplication(rotateMatrix);
            cout << "after rotation" << endl;
            currMatrix.print();
        }
        else if (command == "push")
        {
            s.push(currMatrix);
        }
        else if (command == "pop")
        {
            currMatrix = s.top();
            s.pop();
        }
        else if (command == "end")
        {
            break;
        }
    }

    fin.close();
    fout.close();

    /// STAGE 2 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    fin = ifstream("./outputs/stage1.txt");
    fout = ofstream("./outputs/stage2.txt");
    fout << std::fixed << std::setprecision(7);

    Matrix viewMatrix = viewTransform(eye, look, up);
    for (int i = 0; i < triangle; i++)
    {
        Point p, q, r;
        fin >> p.x >> p.y >> p.z;
        fin >> q.x >> q.y >> q.z;
        fin >> r.x >> r.y >> r.z;

        p = viewMatrix.pointMultiplication(p);
        q = viewMatrix.pointMultiplication(q);
        r = viewMatrix.pointMultiplication(r);

        fout << p.x << " " << p.y << " " << p.z << endl;
        fout << q.x << " " << q.y << " " << q.z << endl;
        fout << r.x << " " << r.y << " " << r.z << endl;

        fout << endl;
    }
    fin.close();
    fout.close();

    /// STAGE 3 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    fin = ifstream("./outputs/stage2.txt");
    fout = ofstream("./outputs/stage3.txt");
    fout << std::fixed << std::setprecision(7);

    Matrix projectMatrix = projectionTransform(fovY, ratio, near, far);
    for (int i = 0; i < triangle; i++)
    {
        Point p, q, r;
        fin >> p.x >> p.y >> p.z;
        fin >> q.x >> q.y >> q.z;
        fin >> r.x >> r.y >> r.z;

        p = projectMatrix.pointMultiplication(p);
        q = projectMatrix.pointMultiplication(q);
        r = projectMatrix.pointMultiplication(r);

        fout << p.x << " " << p.y << " " << p.z << endl;
        fout << q.x << " " << q.y << " " << q.z << endl;
        fout << r.x << " " << r.y << " " << r.z << endl;

        fout << endl;
    }
    fin.close();
    fout.close();

    /// STAGE 4 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    buffering(triangle);

    return 0;
}

#endif