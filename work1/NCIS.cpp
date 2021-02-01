#include <opencv2/opencv.hpp>

#include <string>
#include <cstdio>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

double mat[1024][1024];
double sita_I = 0.1, sita_X = 4, r = 5;

void cal_weight(int x, int y, Mat &pic)
{
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            ;
        }
    }
}

void init(double mat[][1024], Mat &pic)
{
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            ;
        }
    }
    cout << "asdf" << endl;
}

int main()
{
    Mat pic = imread("data.PNG", IMREAD_GRAYSCALE);
}