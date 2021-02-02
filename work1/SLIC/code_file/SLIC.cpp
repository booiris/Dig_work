#include <opencv.hpp>
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <time.h>
#include "../head_file/color_convert.h"

using namespace std;
using namespace cv;
int maxn, row, col, k_cnt, S;
double Lab[3][1024][1024];
struct Point
{
    int x, y, num;
} center[100005];
Mat pic;
double min_dis[1024][1024];
int belong[1024][1024];

inline double get_grad(int &x, int &y)
{
    double Lx, ax, bx, Ly, ay, by;
    Lx = (x - 1 >= 0 ? Lab[0][x - 1][y] : 0) - (x + 1 < row ? Lab[0][x + 1][y] : 0);
    ax = (x - 1 >= 0 ? Lab[1][x - 1][y] : 0) - (x + 1 < row ? Lab[1][x + 1][y] : 0);
    bx = (x - 1 >= 0 ? Lab[2][x - 1][y] : 0) - (x + 1 < row ? Lab[2][x + 1][y] : 0);

    Ly = (y - 1 >= 0 ? Lab[0][x][y - 1] : 0) - (y + 1 < col ? Lab[0][x][y + 1] : 0);
    ay = (y - 1 >= 0 ? Lab[0][x][y - 1] : 0) - (y + 1 < col ? Lab[0][x][y + 1] : 0);
    by = (y - 1 >= 0 ? Lab[0][x][y - 1] : 0) - (y + 1 < col ? Lab[0][x][y + 1] : 0);

    return Lx * Lx + ax * ax + bx * bx + Ly * Ly + ay * ay + by * by;
}

void get_center(int &x, int &y)
{
    double min_grad = 1e9, temp_grad;
    int temp_x, temp_y;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int nx = x + i, ny = y + j;
            if (nx < 0 || nx >= row || ny < 0 || ny >= col)
                continue;
            if (min_grad > (temp_grad = get_grad(nx, ny)))
            {
                min_grad = temp_grad;
                temp_x = nx, temp_y = ny;
            }
        }
    }
    x = temp_x, y = temp_y;
}

void init()
{
    int center_index = 0;
    int len = sqrt(1.0 * S);
    for (int i = len / 2; i < row; i += len)
    {
        for (int j = len / 2; j < col; j += len)
        {
            int x = i, y = j;
            get_center(x, y);
            center[center_index].x = x;
            center[center_index].y = y;
            center_index++;
        }
    }
    if (center_index != k_cnt)
    {
        cout << center_index << " " << k_cnt << endl;
        exit(-1);
    }
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            min_dis[i][j] = 1e9, belong[i][j] = -1;
}

double get_dis()
{
}

void SLIC()
{
    int len = sqrt(1.0 * S);
    for (int nowp = 0; nowp < k_cnt; nowp++)
    {
        int x = center[nowp].x, y = center[nowp].y;
        for (int i = -len; i <= len; i++)
        {
            for (int j = -len; j <= len; j++)
            {
                int nx = x + i, ny = y + j;
                if (nx < 0 || nx >= row || ny < 0 || ny > col)
                    continue;
                double dis = get_dis();
                if (dis < min_dis[nx][ny])
                {
                    min_dis[nx][ny] = dis;
                    belong[nx][ny] = nowp;
                }
            }
        }
    }
    
}

int main()
{
    clock_t start, end;
    start = clock();
    pic = imread("../cut.jpg", IMREAD_GRAYSCALE);
    row = pic.rows, col = pic.cols;
    maxn = row * col;

    // cout << "cin>>k" << endl;
    // cin >> k_cnt;
    k_cnt = 1024;
    S = maxn / k_cnt;
    cout << S << endl;

    color_convert(pic, Lab);
    init();

    end = clock();
    cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;
}