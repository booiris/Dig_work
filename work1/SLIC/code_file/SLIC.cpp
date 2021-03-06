// 本文件负责实现聚类过程

#include <opencv.hpp>
#include <string>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <time.h>
#include "../head_file/color_convert.h"
#include "../head_file/draw.h"

using namespace std;
using namespace cv;
int maxn, row, col;
double S;
double Lab[3][1500][1500];
struct Point
{
    double L, a, b, x, y;
    int cnt;
} center[100005];
Mat pic;
double min_dis[1500][1500];
int belong[1500][1500];
int group_sum[5][100005];
int param_m = 40, k_cnt = 700; // 设置参数，param_m为计算距离时比例参数，k_cnt为超像素数量

inline double get_grad(int &x, int &y)
{
    // 该函数为超像素中心初始化时计算梯度函数
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
    // 该函数为初始化超像素中心
    double min_grad = 1e9, temp_grad;
    int temp_x, temp_y;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int nx = x + i, ny = y + j;
            if (nx < 0 || nx >= row || ny < 0 || ny >= col)
                continue;
            // 获取梯度后，选取最小梯度作为中心
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
    // 算法初始化
    int center_index = 0;
    int len = S;

    // 获取超像素中心
    for (int i = len / 2; i < row && center_index < k_cnt; i += len)
    {
        for (int j = len / 2; j < col && center_index < k_cnt; j += len)
        {
            int x = i, y = j;
            get_center(x, y);
            center[center_index].L = Lab[0][x][y];
            center[center_index].a = Lab[1][x][y];
            center[center_index].b = Lab[2][x][y];
            center[center_index].x = x;
            center[center_index].y = y;
            center_index++;
        }
    }
    if (center_index != k_cnt)
    {
        cout << "wrong: " << center_index << " " << k_cnt << endl;
        exit(-1);
    }
    // 像素到类中心的最小距离，像素属于的类进行初始化
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            min_dis[i][j] = 1e9, belong[i][j] = -1;
}

double get_dis(int &x, int &y, int &centre_index)
{
    // 计算像素点到类中心的距离函数

    // 计算颜色特征距离
    double d_c = (center[centre_index].L - Lab[0][x][y]) * (center[centre_index].L - Lab[0][x][y]);
    d_c += (center[centre_index].a - Lab[1][x][y]) * (center[centre_index].a - Lab[1][x][y]);
    d_c += (center[centre_index].b - Lab[2][x][y]) * (center[centre_index].b - Lab[2][x][y]);

    // 计算空间特征距离
    double d_s = (center[centre_index].x - x) * (center[centre_index].x - x);
    d_s = (center[centre_index].y - y) * (center[centre_index].y - y);
    d_s = d_s * k_cnt / maxn * param_m * param_m;
    return sqrt(d_c + d_s);
}

double SLIC()
{
    // 聚类过程函数
    int len = S + 1;
    for (int nowp = 0; nowp < k_cnt; nowp++)
    {
        center[nowp].cnt = 0;
        double x = center[nowp].x, y = center[nowp].y;
        for (int i = 0; i < 5; i++)
            group_sum[i][nowp] = 0;

        for (int i = -len; i <= len; i++)
        {
            for (int j = -len; j <= len; j++)
            {
                int nx = x + i, ny = y + j;
                if (nx < 0 || nx >= row || ny < 0 || ny > col)
                    continue;
                double dis = get_dis(nx, ny, nowp);
                // 当当前像素点到类中心距离小于最小距离时，说明该像素点属于该类，更新最小距离和该点属于的类
                if (dis < min_dis[nx][ny])
                {
                    min_dis[nx][ny] = dis;
                    belong[nx][ny] = nowp;
                }
            }
        }
    }

    double E = 0;
    // 更新类中心
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            // 对于每一个类，计算它的子像素特征的平均值，更新类中心
            group_sum[0][belong[i][j]] += Lab[0][i][j];
            group_sum[1][belong[i][j]] += Lab[1][i][j];
            group_sum[2][belong[i][j]] += Lab[2][i][j];
            group_sum[3][belong[i][j]] += i;
            group_sum[4][belong[i][j]] += j;
            center[belong[i][j]].cnt++;
        }
    }
    for (int i = 0; i < k_cnt; i++)
    {
        double L = 1.0 * group_sum[0][i] / center[i].cnt;
        double a = 1.0 * group_sum[1][i] / center[i].cnt;
        double b = 1.0 * group_sum[2][i] / center[i].cnt;
        double x = 1.0 * group_sum[3][i] / center[i].cnt;
        double y = 1.0 * group_sum[4][i] / center[i].cnt;
        double temp = (L - center[i].L) * (L - center[i].L);
        temp += (a - center[i].a) * (a - center[i].a);
        temp += (b - center[i].b) * (b - center[i].b);
        temp += (x - center[i].x) * (x - center[i].x);
        temp += (y - center[i].y) * (y - center[i].y);
        center[i].L = L;
        center[i].a = a;
        center[i].b = b;
        center[i].x = x;
        center[i].y = y;
        E += sqrt(temp);
    }
    return E;
}

int main()
{
    clock_t start, end;
    start = clock();
    pic = imread("../cut.jpg");
    row = pic.rows, col = pic.cols;
    maxn = row * col;

    S = sqrt(1.0 * maxn / k_cnt);
    color_convert(pic, Lab);
    init();
    cout << row << " " << col << endl;
    for (int i = 0; i < 40; i++)
        SLIC();
    draw();
    imwrite("../res-40.png", pic);
    end = clock();
    cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;
}