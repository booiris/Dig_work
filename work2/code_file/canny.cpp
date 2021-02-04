#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
using namespace std;
using namespace cv;

void rgb2gray(Mat &pic, Mat &gray)
{
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            int temp = 299 * pic.at<Vec3b>(i, j)[2] + 587 * pic.at<Vec3b>(i, j)[1] + 114 * pic.at<Vec3b>(i, j)[0] + 500;
            gray.at<uchar>(i, j) = (uchar)(1.0 * temp / 1000);
        }
    }
}

const double pi = 3.1415926;
int matsize = 5;
double sigma = 0.3 * ((matsize - 1) * 0.5 - 1) + 0.8;
double matsum;
inline double Gaussian(int &x, int &y)
{
    return exp(-0.5 * (x * x + y * y) / (sigma * sigma)) / (pi * 2 * sigma * sigma); // 高斯函数值
}

void GaussianTemplate(double t_mat[][10])
{
    // 生成高斯滤波小数模板
    matsum = 0;
    int center = matsize / 2;
    for (int i = 0; i < matsize; i++)
    {
        int x = i - center;
        for (int j = 0; j < matsize; j++)
        {
            int y = j - center;
            t_mat[i][j] = Gaussian(x, y);
            matsum += t_mat[i][j];
        }
    }
}

void smooth(Mat &pic, Mat &mat, double t_mat[][10])
{
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            double tempsum = 0;
            for (int x1 = 0; x1 < matsize; x1++)
            {
                for (int y1 = 0; y1 < matsize; y1++)
                {
                    int nx = i - (x1 - matsize / 2), ny = j - (y1 - matsize / 2); // 模版在图片中的位置

                    // 图片四周填充0，所以不用加到 sum 中
                    if (nx < 0 || nx >= mat.rows || ny < 0 || ny >= mat.cols)
                        continue;
                    tempsum += t_mat[x1][y1] * pic.at<uchar>(nx, ny);
                }
            }
            mat.at<uchar>(i, j) = (uchar)(tempsum / matsum); // 归一化计算模糊后的值
        }
    }
}

double grad[1500][1500], point_dir[1500][1500];
int row, col;

void sobel(Mat &mat)
{
    for (int i = 1; i < mat.rows - 1; i++)
    {
        for (int j = 1; j < mat.cols - 1; j++)
        {
            double gx = 2 * mat.at<uchar>(i, j + 1) + mat.at<uchar>(i - 1, j + 1) + mat.at<uchar>(i + 1, j + 1) - 2 * mat.at<uchar>(i, j - 1) - mat.at<uchar>(i - 1, j - 1) - mat.at<uchar>(i + 1, j - 1);

            double gy = 2 * mat.at<uchar>(i - 1, j) + mat.at<uchar>(i - 1, j + 1) + mat.at<uchar>(i - 1, j - 1) - 2 * mat.at<uchar>(i + 1, j) - mat.at<uchar>(i + 1, j - 1) - mat.at<uchar>(i + 1, j + 1);

            grad[i][j] = sqrt(gx * gx + gy * gy);
            if (gx == 0)
                gx = 1e-9;
            point_dir[i][j] = atan(gy / gx) + pi / 2;
        }
    }
}

void local_max_value()
{
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            double 
        }
    }
}

void double_threshold_link(Mat &out, uchar lowThreshold, uchar highThreshold)
{
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (out.at<uchar>(i, j) == 255)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if (out.at<uchar>(i + k, j + l) != 0)
                            out.at<uchar>(i + k, j + l) = 255;
                    }
                }
            }
        }
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (out.at<uchar>(i, j) != 255)
                out.at<uchar>(i, j) = 0;
        }
    }
}

void double_threshold(Mat &out, uchar lowThreshold, uchar highThreshold)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (out.at<uchar>(i, j) >= highThreshold)
                out.at<uchar>(i, j) = 255;
            if (out.at<uchar>(i, j) <= lowThreshold)
                out.at<uchar>(i, j) = 0;
        }
    }
    double_threshold_link(out, lowThreshold, highThreshold);
}

int main()
{
    double t_mat[10][10];
    GaussianTemplate(t_mat);
    Mat pic = imread("../data.jpg");
    row = pic.rows, col = pic.cols;
    Mat gray = Mat::zeros(pic.size(), CV_8UC1);
    rgb2gray(pic, gray);
    Mat out = gray.clone();
    smooth(gray, out, t_mat);
    // imshow("gray", gray);
    // imshow("smooth", out);
    // waitKey(0);
}