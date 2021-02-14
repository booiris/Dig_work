#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "../head_file/Hough.h"
#include "../head_file/canny.h"
using namespace std;
using namespace cv;
vector<point> line_p;
Mat cut(Mat &pic)
{
    Mat res = Mat::zeros(470, 1280, CV_8UC3);
    for (int i = 250; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            res.at<Vec3b>(i - 250, j) = pic.at<Vec3b>(i, j);
        }
    }
    return res;
}
int main()
{
    Mat pic = imread("../data3.jpg");
    pic = cut(pic);
    Mat gray = canny(pic);
    hough(gray, pic, 0, pic.rows, 255, 255, 0, 60, line_p);
    
    imshow("gray", gray);
    imshow("pic", pic);
    waitKey(0);
}