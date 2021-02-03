#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
using namespace std;
using namespace cv;

int main()
{
    Mat pic = imread("../data.jpg");
    Rect rect(0, (int)(0.3 * pic.rows), pic.cols, (int)(0.7 * pic.rows));
    Mat aim = pic(rect);

    imshow("asd", aim);
    waitKey(0);
    
}