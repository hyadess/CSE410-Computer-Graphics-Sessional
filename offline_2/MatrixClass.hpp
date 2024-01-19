#ifndef MATRIX
#define MATRIX

#include <bits/stdc++.h>
using namespace std;
#include "PointClass.hpp"
class Matrix
{
public:
    double arr[4][4];
    Matrix()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                this->arr[i][j] = 0;
                if (i == j && i != 3)
                {
                    arr[i][j] = 1;
                }
            }
        }
    }
    Matrix(double arr[4][4])
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                this->arr[i][j] = arr[i][j];
            }
        }
    }

    Matrix multiplication(Matrix p)
    {
        double result[4][4];
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result[i][j] = 0;
                for (int k = 0; k < 4; ++k)
                {
                    result[i][j] += this->arr[i][k] * p.arr[k][j];
                }
            }
        }
        Matrix newMatrix=Matrix(result);
        return newMatrix;
    }

    Point pointMultiplication(Point p)
    {
        double point[4];
        point[0] = p.x;
        point[1] = p.y;
        point[2] = p.z;
        point[3] = 1;

        double resultPoint[4];
        for (int i = 0; i < 4; ++i)
        {
            resultPoint[i] = 0.0;
            for (int j = 0; j < 4; ++j)
            {
                resultPoint[i] += arr[i][j] * point[j];
            }
        }

        Point newP = Point(resultPoint[0], resultPoint[1], resultPoint[2]);
        newP.setScale(resultPoint[3]);
        return newP;
    }
};

#endif