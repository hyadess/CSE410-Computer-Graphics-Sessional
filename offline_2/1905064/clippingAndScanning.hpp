#ifndef BUFFER
#define BUFFER

#include <bits/stdc++.h>
using namespace std;
#include "bitmap_image.hpp"
#include "TriangleClass.hpp"

void buffering(int triangle)
{
    ifstream fin("./outputs/stage3.txt");
    ofstream fout("./outputs/z_buffer.txt");
    ifstream fin2("./spec/IOs/5/config.txt");




    int screen_width, screen_height;
    fin2 >> screen_width >> screen_height;
    double box_left = -1, box_right = 1, box_bottom = -1, box_top = 1;
    int Z_min = -1, Z_max = 1;
    double dx = (box_right - box_left) / screen_width;
    double dy = (box_top - box_bottom) / screen_height;
    double Top_Y = box_top - dy / 2.0;
    double Left_X = box_left + dx / 2.0;

    // initialize z-buffer and frame buffer

    vector<vector<double>> zBuffer(screen_height, vector<double>(screen_width, Z_max));
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            zBuffer[i][j] = Z_max;
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

        //we are sorting the points so that instead of considering 3 edges, we can do the whole scanning with only two edges.
        t.sortPoints();
        t.recolor();

        //sorted such that a.y < b.y < c.y


        // for the first edge AB.............we will calculate Xs and Zs .................................

        // clipping
        double min_y = t.a.y;
        double max_y = t.b.y;

        min_y = max(min_y, box_bottom);
        max_y = min(max_y, box_top);

        // scan conversion

        for (double y = min_y; y <= max_y; y += dy)
        {

            double min_x = 1;
            double max_x = -1;


            double min_z = 1;
            double max_z = -1;

            if (t.b.y != t.a.y && t.a.y != t.c.y) // to bypass divide by 0..................
            {
                // Xs = x1 + (x2-x1) * (Ys-y1) / (y2 -y1)
                // same way for Zs.....
                // here, we are working with AB edge... so the scanline will always intersect another edge which is AC.
                //but if the line is parallel to x axis, then it would not happen and min_x == max_x 

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

            //clipping.................

            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);

            for (double x = min_x; x <= max_x; x += dx)
            {
                if (max_x == min_x) // to avoid divide by 0....................
                    continue;

                int i = (Top_Y - y) / dy;
                int j = (x - Left_X) / dx;

                double z = min_z + (max_z - min_z) * (x - min_x) / (max_x - min_x);
                if (z < zBuffer[i][j] and z > Z_min)
                {
                    zBuffer[i][j] = z;
                    image.set_pixel(j, i, t.color.r, t.color.g, t.color.b);
                }
            }
        }

        min_y = t.b.y;
        max_y = t.c.y;

        //clipping...........

        min_y = max(min_y, box_bottom);
        max_y = min(max_y, box_top);

        // scan conversion
        for (double y = min_y; y <= max_y; y += dy)
        {

            double min_x = 0;
            double max_x = -1;
            double min_z = 0;
            double max_z = -1;

            if (t.b.y != t.c.y and t.a.y != t.c.y) // to bypass divide by 0..................
            {
                // Xs = x1 + (x2-x1) * (Ys-y1) / (y2 -y1)
                // same way for Zs.....
                // here, we are working with BC edge... so the scanline will always intersect another edge which is AC.
                //but if the line is parallel to x axis, then it would not happen and min_x == max_x 
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
            //clipping.................
            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);

            for (double x = min_x; x <= max_x; x += dx)
            {
                if (max_x == min_x)
                    continue;

                int i = (Top_Y - y) / dy;
                int j = (x - Left_X) / dx;

                double z = min_z + (max_z - min_z) * (x - min_x) / (max_x - min_x);
                if (z < zBuffer[i][j] and z > Z_min)
                {
                    zBuffer[i][j] = z;
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
            if (zBuffer[i][j] < Z_max)
            {
                fout << setprecision(6) << fixed << zBuffer[i][j] << "\t";
            }
        }
        fout << endl;
    }

    fin.close();
    fin2.close();
    fout.close();

    zBuffer.clear();
    zBuffer.shrink_to_fit();
}

#endif