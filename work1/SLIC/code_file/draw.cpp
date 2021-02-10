#include <opencv.hpp>
#include <vector>
#include "../head_file/SLIC.h"
using namespace std;
using namespace cv;
bool vis[1500][1500];
int color[1500][1500];
int dir_4[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
void find_edge(int &nowx, int &nowy, int &part)
{
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
    if (cnt <= 3)
    {
        pic.at<Vec3b>(nowx, nowy)[0] = 0;
        pic.at<Vec3b>(nowx, nowy)[1] = 0;
        pic.at<Vec3b>(nowx, nowy)[2] = 0;
    }
}
vector<int> x_list, y_list;
void dfs(int &nowx, int &nowy, int &part, int &now_color)
{
    x_list.push_back(nowx);
    y_list.push_back(nowy);
    color[nowx][nowy] = now_color;
    for (int i = 0; i < 4; i++)
    {
        int nx = nowx + dir_4[i][0], ny = nowy + dir_4[i][1];
        if (nx < 0 || nx >= pic.rows || ny < 0 || ny >= pic.cols || color[nx][ny] != -1 || belong[nx][ny] != part)
            continue;
        dfs(nx, ny, part, now_color);
    }
}
void connect()
{
    int now_color = 0;
    int kkk = 0;
    for (int i = 0; i < pic.rows; i++)
    {
        for (int j = 0; j < pic.cols; j++)
        {
            if (color[i][j] == -1)
            {
                int pre_color = -1;
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
                dfs(i, j, belong[i][j], now_color);
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
    cout << now_color << " " << kkk << "\n";
}

void draw()
{
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