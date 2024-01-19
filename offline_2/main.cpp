#ifndef MAIN
#define MAIN

#include <bits/stdc++.h>
using namespace std;
#include "PointClass.hpp"
#include "MatrixClass.hpp"
#include "Stage1MatrixGen.hpp"
#include "Stage2MatrixGen.hpp"
#include "Stage3MatrixGen.hpp"
#include "bitmap_image.hpp"
#include "TriangleClass.hpp"

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

    fin.open("./outputs/stage3.txt");
    fout.open("./outputs/z_buffer.txt");
    ifstream fin2("./spec/IOs/1/config.txt");
    int screen_width, screen_height;
    fin2 >> screen_width >> screen_height;
    double box_left = -1, box_right = 1, box_bottom = -1, box_top = 1;
    int Z_min = -1, Z_max = 1;
    double dx = (box_right - box_left) / screen_width;
    double dy = (box_top - box_bottom) / screen_height;
    double Top_Y = box_top - dy / 2.0;
    double Left_X = box_left + dx / 2.0;

    // initialize z-buffer and frame buffer

    vector<vector<double>> z_buffer(screen_height, vector<double>(screen_width, Z_max));
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            z_buffer[i][j] = Z_max;
        }
    }

    bitmap_image image(screen_width, screen_height);
    image.set_all_channels(0, 0, 0);

    for (int i = 0; i < triangle; i++)
    {
        Triangle t;
        fin >> t.a.x >> t.a.y >> t.a.z;
        fin >> t.b.x >> t.b.y >> t.b.z;
        fin >> t.c.x >> t.c.y >> t.c.z;

        t.sort();
        t.recolor();

        // clipping
        double min_y = t.a.y;
        double max_y = t.b.y;


        min_y = max(min_y, box_bottom);
        max_y = min(max_y, box_top);

        // scan conversion

        for (double y = min_y; y <= max_y; y += dy)
        {

            // compute min_x and max_x for this row
            double min_x = 0;
            double max_x = -1;
            double min_z = 0;
            double max_z = -1;

            if (t.b.y != t.a.y and t.a.y != t.c.y)
            {
                min_x = t.a.x + (t.b.x - t.a.x) * (y - t.a.y) / (t.b.y - t.a.y);
                max_x = t.a.x + (t.c.x - t.a.x) * (y - t.a.y) / (t.c.y - t.a.y);

                min_z = t.a.z + (t.b.z - t.a.z) * (y - t.a.y) / (t.b.y - t.a.y);
                max_z = t.a.z + (t.c.z - t.a.z) * (y - t.a.y) / (t.c.y - t.a.y);

                if (min_x > max_x)
                {
                    swap(min_x, max_x);
                    swap(min_z, max_z);
                }
            }

            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);
         

            for (double x = min_x; x <= max_x; x += dx)
            {
                if (max_x == min_x)
                    continue;

                int i = (Top_Y - y) / dy;
                int j = (x - Left_X) / dx;

                double z = min_z + (max_z - min_z) * (x - min_x) / (max_x - min_x);
                if (z < z_buffer[i][j] and z > Z_min)
                {
                    z_buffer[i][j] = z;
                    image.set_pixel(j, i, t.color.r, t.color.g, t.color.b);
                }
            }
        }

        min_y = t.b.y;
        max_y = t.c.y;

       

        min_y = max(min_y, box_bottom);
        max_y = min(max_y, box_top);

      

        // scan conversion
        for (double y = min_y; y <= max_y; y += dy)
        {

      
            double min_x = 0;
            double max_x = -1;
            double min_z = 0;
            double max_z = -1;

            if (t.b.y != t.c.y and t.a.y != t.c.y)
            {
                min_x = t.c.x + (t.b.x - t.c.x) * (y - t.c.y) / (t.b.y - t.c.y);
                max_x = t.a.x + (t.c.x - t.a.x) * (y - t.a.y) / (t.c.y - t.a.y);

                min_z = t.c.z + (t.b.z - t.c.z) * (y - t.c.y) / (t.b.y - t.c.y);
                max_z = t.a.z + (t.c.z - t.a.z) * (y - t.a.y) / (t.c.y - t.a.y);

                if (min_x > max_x)
                {
                    swap(min_x, max_x);
                    swap(min_z, max_z);
                }
            }
            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);
         

            for (double x = min_x; x <= max_x; x += dx)
            {
                if (max_x == min_x)
                    continue;

                int i = (Top_Y - y) / dy;
                int j = (x - Left_X) / dx;

                double z = min_z + (max_z - min_z) * (x - min_x) / (max_x - min_x);
                if (z < z_buffer[i][j] and z > Z_min)
                {
                    z_buffer[i][j] = z;
                    image.set_pixel(j, i, t.color.r, t.color.g, t.color.b);
                }
            }
        }
    }

    image.save_image("./outputs/out.bmp");

    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            if (z_buffer[i][j] < Z_max)
            {
                fout << setprecision(6) << fixed << z_buffer[i][j] << "\t";
            }
        }
        fout << endl;
    }

    fin.close();
    fin2.close();
    fout.close();

    z_buffer.clear();
    z_buffer.shrink_to_fit();

    return 0;
}

#endif