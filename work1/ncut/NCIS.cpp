#include <opencv.hpp>
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

double mat[1024][1024];
double sita_I = 0.1, sita_X = 4, r = 5;
struct node
{
    int from, to;
    double w;
};
vector<int> p[300000];
double D[300000];
double ans[300000];
vector<node> edges;
int row, col, maxn;
void cal_weight(int x, int y)
{
    int nowp = x * row + y;
    double sum = 0;
    for (int i = x - r; i <= x + r; i++)
    {
        for (int j = y - r; j <= y + r; j++)
        {
            if (i < 0 || i >= row || j < 0 || j >= col)
                continue;
            double d = sqrt((i - x) * (i - x) + (j - y) * (j - y));
            if (d >= r)
                continue;
            double v1 = exp(-(mat[i][j] - mat[x][y]) * (mat[i][j] - mat[x][y]) / sita_I);
            double v2 = exp(-1.0 * ((i - x) * (i - x) + (j - y) * (j - y)) / sita_X);
            edges.push_back(node{nowp, i * row + j, v1 * v2});
            p[nowp].push_back(edges.size() - 1);
            sum += v1 * v2;
        }
    }
    D[nowp] = sum;
}

void init(Mat &pic)
{
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            mat[i][j] = 1.0 * pic.at<Vec3b>(i, j)[0] / 225;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            cal_weight(i, j);
}





int main()
{
    Mat pic = imread("data.PNG", IMREAD_GRAYSCALE);
    row = pic.rows, col = pic.cols;
    maxn = row * col;
    init(pic);
    

}