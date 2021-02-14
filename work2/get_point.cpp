#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include "../head_file/Hough.h"
using namespace std;
using namespace cv;
void convert_color(Mat &out, Mat &pic)
{
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            out.at<uchar>(i, j) = pic.at<Vec3b>(i, j)[1];
        }
    }
}
struct point
{
    int x, y;
};

int gray_cnt[1500], a_cnt[1500];
void plot_point(Mat &gray)
{
    int len = 20;
    int nowx = 0;

    Mat res = gray.clone();
    for (int i = 0; i < gray.rows; i++)
    {
        for (int j = 0; j < gray.cols; j++)
        {
            int sum = 0;
            for (int k = -6; k <= 6; k++)
            {
                if (j - k < 0 || j - k >= gray.cols)
                    continue;
                sum += gray.at<uchar>(i, j - k);
            }
            int temp = min(255, sum / 13);
            res.at<uchar>(i, j) = temp;
        }
    }

    for (int l = 0; l < 9; l++)
    {
        for (int i = 0; i < gray.cols; i++)
            gray_cnt[i] = 0;
        for (int i = nowx; i < nowx + len; i++)
        {
            for (int j = 0; j < gray.cols; j++)
            {
                gray_cnt[j] += gray.at<uchar>(i, j);
            }
        }
        string file_name = "../plot_file/";
        file_name += to_string(l);
        file_name += ".txt";
        const char *c = file_name.c_str();
        freopen(c, "w", stdout);
        for (int i = 0; i < gray.cols; i++)
            printf("%d\n", gray_cnt[i]);
        fclose(stdout);
        nowx += len;
        len += 5;
    }
    for (int j = 0; j < gray.cols; j++)
        gray_cnt[j] = 0;
    for (int i = nowx; i < gray.rows; i++)
    {
        for (int j = 0; j < gray.cols; j++)
            gray_cnt[j] += gray.at<uchar>(i, j);
    }
    freopen("../plot_file/9.txt", "w", stdout);
    for (int i = 0; i < gray.cols; i++)
        printf("%d\n", gray_cnt[i]);
    fclose(stdout);
}

void get_point(Mat &gray, Mat &pic)
{
    int len = 3;
    int nowx = 0;
    equalizeHist(gray, gray);
    Mat res = gray.clone();
    for (int i = 0; i < gray.rows; i++)
    {
        for (int j = 0; j < gray.cols; j++)
        {
            int sum = 0;
            for (int k = -5; k <= 5; k++)
            {
                if (j - k < 0 || j - k >= gray.cols)
                    continue;
                sum += gray.at<uchar>(i, j - k);
            }
            int temp = min(255, (int)(1.0 * sum / 11));
            res.at<uchar>(i, j) = temp;
        }
    }
    int cnt = 0;
    Mat key = gray.clone();
    for (int i = 0; i < key.rows; i++)
        for (int j = 0; j < key.cols; j++)
            key.at<uchar>(i, j) = 0;
    while (nowx + len < gray.rows)
    {
        for (int i = 0; i < gray.cols; i++)
            gray_cnt[i] = 0, a_cnt[i] = 0;
        for (int i = nowx; i < nowx + len; i++)
            for (int j = 0; j < gray.cols; j++)
                gray_cnt[j] += gray.at<uchar>(i, j), a_cnt[j] += res.at<uchar>(i, j);

        for (int i = 6; i < gray.cols - 6; i++)
        {
            int p = i;
            int minnum, maxnum;
            int e;
            switch (cnt / 50)
            {
            case 0:
                minnum = 5, maxnum = 7, e = 20;
                break;
            case 1:
                minnum = 6, maxnum = 12, e = 12;
                break;
            default:
                minnum = 7, maxnum = 15, e = 1;
                break;
            }
            int maxn = 0;
            while (p < gray.cols - 6 && gray_cnt[p] - a_cnt[p] > e)
                maxn = max(maxn, gray_cnt[p]), p++;
            if (p - i >= minnum && p - i <= maxnum)
            {
                key.at<uchar>(nowx + len / 2, i + (p - i) / 2) = 255;
                circle(pic, Point2d(i + (p - i) / 2, nowx + len / 2), 1, Scalar(0, 0, 255));
            }
            i = p;
        }
        cnt++;
        if (cnt % 50 == 0)
            len++;
        nowx += len;
    }

    line(pic, Point2d(0, 40), Point2d(pic.cols - 1, 40), Scalar(0, 255, 0), 1);
    line(pic, Point2d(0, 250), Point2d(pic.cols - 1, 250), Scalar(255, 0, 0), 1);
    hough(key, pic, 0, 40, 0, 255, 0);
    hough(key, pic, 50, 265, 255, 0, 0);
    hough(key, pic, 265, pic.rows, 0, 255, 255);
}

// int main()
// {
//     Mat pic = imread("../data1.jpg");
//     Rect rect(0, (int)(0.37 * pic.rows), pic.cols, (int)(0.63 * pic.rows));
//     pic = pic(rect);
//     Mat gray(pic.rows, pic.cols, CV_8UC1);
//     convert_color(gray, pic);
//     // plot_point(gray);
//     get_point(gray, pic);
//     cout << gray.rows << " " << gray.cols << "\n";
//     imshow("pic", pic);
//     // imshow("gray", gray);
//     waitKey(0);
// }