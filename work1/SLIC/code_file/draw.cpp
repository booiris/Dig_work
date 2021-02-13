// 本文件负责实现超像素图像的绘制，步骤为先合并小像素，接下来搜索超像素边缘，绘制超像素边缘

#include <opencv.hpp>
#include <vector>
#include "../head_file/SLIC.h"
using namespace std;
using namespace cv;
bool vis[1500][1500];
int color[1500][1500]; // 合并像素后的超像素图像
int dir_4[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
void find_edge(int &nowx, int &nowy, int &part)
{
    // 该函数用于寻找超像素边缘，绘制超像素边缘
    vis[nowx][nowy] = 1;
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        int nx = nowx + dir_4[i][0], ny = nowy + dir_4[i][1];
        if (nx < 0 || nx >= pic.rows || ny < 0 || ny >= pic.cols || color[nx][ny] != part)
            continue;
        cnt++;
        if (vis[nx][ny])
            continue;
        find_edge(nx, ny, part);
    }
    if (cnt <= 3) // 如果该像素周围存在不同类的像素，说明它为超像素边缘
    {
        pic.at<Vec3b>(nowx, nowy)[0] = 0;
        pic.at<Vec3b>(nowx, nowy)[1] = 0;
        pic.at<Vec3b>(nowx, nowy)[2] = 0;
    }
}
vector<int> x_list, y_list;
void dfs(int &nowx, int &nowy, int &part, int &now_color)
{
    // 该函数递归寻找同类像素 
    x_list.push_back(nowx);
    y_list.push_back(nowy);
    color[nowx][nowy] = now_color;
    for (int i = 0; i < 4; i++)
    {
        int nx = nowx + dir_4[i][0], ny = nowy + dir_4[i][1];
        // 如果要寻找的像素与当前像素相同且未被赋予新的类，则递归寻找该像素
        if (nx < 0 || nx >= pic.rows || ny < 0 || ny >= pic.cols || color[nx][ny] != -1 || belong[nx][ny] != part)
            continue;
        dfs(nx, ny, part, now_color);
    }
}
void connect()
{
    // 合并小区域像素的主要功能函数
    int now_color = 0;
    int kkk = 0;
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            if (color[i][j] == -1) // 如果当前像素未被更新
            {
                int pre_color = -1; // 设置更新像素，从该像素的四领域寻找更新的超像素
                for (int k = 0; k < 4; k++)
                {
                    int nx = i + dir_4[k][0], ny = j + dir_4[k][1];
                    if (nx < 0 || nx >= pic.rows || ny < 0 || ny >= pic.cols)
                        continue;
                    if (color[nx][ny] != -1)
                    {
                        pre_color = color[nx][ny];
                        break;
                    }
                }
                x_list.clear();
                y_list.clear();
                // 递归寻找同类的像素，加入x和y值分别加入x_list和y_list中
                dfs(i, j, belong[i][j], now_color);

                // 如果同类像素数量小于理想超像素大小的一半，则需要进行像素合并
                if (x_list.size() < 1.0 * pic.rows * pic.cols / (k_cnt * 2) && pre_color != -1)
                {
                    for (int k = 0; k < (int)x_list.size(); k++)
                        color[x_list[k]][y_list[k]] = pre_color;
                }
                else
                {
                    if (x_list.size() < 1.0 * pic.rows * pic.cols / (k_cnt * 2) && pre_color != -1)
                        kkk++;
                    now_color++;
                }
            }
        }
    }
}

void draw()
{
    // 绘制超像素调用函数
    for (int i = 0; i < pic.rows; i++)
        for (int j = 0; j < pic.cols; j++)
            color[i][j] = -1;
    for (int i = 0; i < pic.rows; i++)
        for (int j = 0; j < pic.cols; j++)
            vis[i][j] = 0;
    connect();
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            if (!vis[i][j])
                find_edge(i, j, color[i][j]);
        }
    }
}