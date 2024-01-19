#ifndef MAIN
#define MAIN

#include <bits/stdc++.h>
using namespace std;
#include "PointClass.hpp"
#include "MatrixClass.hpp"
#include "Stage1MatrixGen.hpp"
#include "Stage2MatrixGen.hpp"
#include "Stage3MatrixGen.hpp"

int main()
{
    ifstream fin("./spec/IOs/1/scene.txt");
    ofstream fout("stage1.txt");

    Point eye, look, up;
    double fovY, ratio, near, far;
    fin >> eye.x >> eye.y >> eye.z;
    fin >> look.x >> look.y >> look.z;
    fin >> up.x >> up.y >> up.z;
    fin >> fovY >> ratio >> near >> far;

    Matrix currMatrix;
    stack<Matrix> s;

    string command;
    fout << std::fixed << std::setprecision(7);

    while (1)
    {
        //cout << 1;
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
        }
        else if (command == "translate")
        {
            double tx, ty, tz;
            fin >> tx >> ty >> tz;
            Matrix traslateMatrix = translationMatrixGen(tx, ty, tz);
            currMatrix = traslateMatrix.multiplication(currMatrix);
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            fin >> sx >> sy >> sz;
            Matrix scaleMatrix = scalingMatrixGen(sx, sy, sz);
            currMatrix = scaleMatrix.multiplication(currMatrix);
        }
        else if (command == "rotate")
        {
            double angle, ax, ay, az;
            fin >> angle >> ax >> ay >> az;
            Matrix rotateMatrix = rotationMatrixGen(ax, ay, az, angle);
            currMatrix = rotateMatrix.multiplication(currMatrix);
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
}

#endif