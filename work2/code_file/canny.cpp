#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "../head_file/Hough.h"
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
Mat out;
void sobel(Mat &mat, Mat &out)
{
    for (int i = 1; i < mat.rows - 1; i++)
    {
        for (int j = 1; j < mat.cols - 1; j++)
        {
            double gx = 2.0 * mat.at<uchar>(i, j + 1) + mat.at<uchar>(i - 1, j + 1) + mat.at<uchar>(i + 1, j + 1) - 2.0 * mat.at<uchar>(i, j - 1) - mat.at<uchar>(i - 1, j - 1) - mat.at<uchar>(i + 1, j - 1);

            double gy = 2.0 * mat.at<uchar>(i - 1, j) + mat.at<uchar>(i - 1, j + 1) + mat.at<uchar>(i - 1, j - 1) - 2.0 * mat.at<uchar>(i + 1, j) - mat.at<uchar>(i + 1, j - 1) - mat.at<uchar>(i + 1, j + 1);

            grad[i][j] = sqrt(gx * gx + gy * gy);
            out.at<uchar>(i, j) = grad[i][j] > 255 ? 255 : grad[i][j];

            if (gx == 0)
                point_dir[i][j] = atan(gy / 1e-9);
            else
                point_dir[i][j] = atan(gy / gx);
        }
    }
}
void local_max_value(Mat &out)
{
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (point_dir[i][j] >= 0 && point_dir[i][j] < pi / 4)
            {
                double g_up = grad[i][j + 1] + (grad[i - 1][j + 1] - grad[i][j + 1]) * tan(point_dir[i][j]);
                double g_down = grad[i][j - 1] + (grad[i + 1][j - 1] - grad[i][j - 1]) * tan(point_dir[i][j]);
                if (grad[i][j] <= g_up || grad[i][j] <= g_down)
                    out.at<uchar>(i, j) = 0, grad[i][j] = 0;
            }
            if (point_dir[i][j] >= pi / 4 && point_dir[i][j] < pi / 2)
            {
                double g_up = grad[i - 1][j] + (grad[i - 1][j + 1] - grad[i - 1][j]) * tan(point_dir[i][j]);
                double g_down = grad[i + 1][j] + (grad[i + 1][j - 1] - grad[i + 1][j]) * tan(point_dir[i][j]);
                if (grad[i][j] <= g_up || grad[i][j] <= g_down)
                    out.at<uchar>(i, j) = 0, grad[i][j] = 0;
            }
            if (point_dir[i][j] > -pi / 2 && point_dir[i][j] <= -pi / 4)
            {
                double g_up = grad[i - 1][j] + (grad[i - 1][j - 1] - grad[i - 1][j]) * tan(point_dir[i][j]);
                double g_down = grad[i + 1][j] + (grad[i + 1][j + 1] - grad[i + 1][j]) * tan(point_dir[i][j]);
                if (grad[i][j] <= g_up || grad[i][j] <= g_down)
                    out.at<uchar>(i, j) = 0, grad[i][j] = 0;
            }
            if (point_dir[i][j] > -pi / 4 && point_dir[i][j] <= 0)
            {
                double g_up = grad[i][j - 1] + (grad[i - 1][j - 1] - grad[i][j - 1]) * tan(point_dir[i][j]);
                double g_down = grad[i][j + 1] + (grad[i + 1][j + 1] - grad[i][j + 1]) * tan(point_dir[i][j]);
                if (grad[i][j] <= g_up || grad[i][j] <= g_down)
                    out.at<uchar>(i, j) = 0, grad[i][j] = 0;
            }
        }
    }
}

int dir_8[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
vector<int> x_list;
vector<int> y_list;
bool vis[1500][1500], flag;
void dfs(int nowx, int nowy)
{
    vis[nowx][nowy] = 1;
    x_list.push_back(nowx);
    y_list.push_back(nowy);
    for (int i = 0; i < 8; i++)
    {
        int nx = nowx + dir_8[i][0], ny = nowy + dir_8[i][1];
        if (nx < 0 || nx >= row || ny < 0 || ny >= col || vis[nx][ny])
            continue;
        if (out.at<uchar>(nx, ny) == 255)
        {
            flag = 1;
            continue;
        }
        if (out.at<uchar>(nx, ny) == 0)
            continue;
        dfs(nx, ny);
    }
}

void double_threshold_link(uchar lowThreshold, uchar highThreshold)
{
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if (out.at<uchar>(i, j) > lowThreshold && out.at<uchar>(i, j) < highThreshold)
            {
                x_list.clear();
                y_list.clear();
                flag = 0;
                dfs(i, j);
                if (flag)
                {
                    for (int k = 0; k < (int)x_list.size(); k++)
                    {
                        vis[x_list[k]][y_list[k]] = 0;
                        out.at<uchar>(x_list[k], y_list[k]) = 255;
                    }
                }
                else
                {
                    for (int k = 0; k < (int)x_list.size(); k++)
                    {
                        vis[x_list[k]][y_list[k]] = 0;
                        out.at<uchar>(x_list[k], y_list[k]) = 0;
                    }
                }
            }
        }
    }
}

void double_threshold(uchar lowThreshold, uchar highThreshold)
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
    double_threshold_link(lowThreshold, highThreshold);
}

Mat wash(Mat &temp)
{
    Mat out = temp.clone();
    for (int i = 1; i < temp.rows - 1; i++)
    {
        for (int j = 0; j < temp.cols; j++)
        {
            int cnt = 0;
            int p = j;
            while (p < temp.cols && (temp.at<uchar>(i, p) == 255 || temp.at<uchar>(i + 1, p) == 255 || temp.at<uchar>(i - 1, p) == 255))
                cnt++, p++;
            if (cnt > 3)
            {
                for (int k = j; k < p; k++)
                    out.at<uchar>(i, k) = 0;
                out.at<uchar>(i, j + cnt / 2) = 255;
            }
        }
    }
    for (int j = 0; j < temp.cols; j++)
    {
        for (int i = 0; i < temp.rows; i++)
        {
            int cnt = 0;
            int p = i;
            while (p < temp.rows && temp.at<uchar>(p, j) == 255)
                cnt++, p++;
            if (cnt > 5)
            {
                for (int k = i; k < p; k++)
                    out.at<uchar>(k, j) = 0;
            }
        }
    }
    return out;
}

Mat canny(Mat &pic)
{
    double t_mat[10][10];
    GaussianTemplate(t_mat);
    row = pic.rows, col = pic.cols;
    Mat gray = Mat::zeros(pic.size(), CV_8UC1);
    rgb2gray(pic, gray);
    out = gray.clone();
    smooth(gray, out, t_mat);
    sobel(gray, out);
    local_max_value(out);
    double_threshold(50, 200);
    out = wash(out);
    return out;
}

// int main()
// {
//     double t_mat[10][10];
//     GaussianTemplate(t_mat);
//     Mat pic = imread("../data1.jpg");
//     pic = cut1(pic);
//     row = pic.rows, col = pic.cols;
//     Mat gray = Mat::zeros(pic.size(), CV_8UC1);
//     rgb2gray(pic, gray);
//     out = gray.clone();
//     smooth(gray, out, t_mat);
//     sobel(gray, out);
//     local_max_value(out);
//     double_threshold(50, 200);
//     out = wash(out);
//     // hough(out, pic, 0, pic.rows, 0, 255, 0, 50, lp);
//     line(pic, Point2d(0, 50), Point2d(pic.cols - 1, 50), Scalar(0, 255, 0), 1);
//     line(pic, Point2d(0, 250), Point2d(pic.cols - 1, 250), Scalar(255, 255, 0), 1);
//     lp1.clear();
//     lp2.clear();
//     lp3.clear();
//     hough(out, pic, 0, 50, 0, 255, 0, 10, lp1);
//     hough(out, pic, 50, 250, 255, 255, 0, 10, lp2);
//     hough(out, pic, 250, pic.rows, 0, 255, 255, 15, lp3);
//     key(pic);
//     imshow("pic", pic);
//     // imshow("out", out);
//     waitKey(0);
// }