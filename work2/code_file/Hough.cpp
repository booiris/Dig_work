#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "../head_file/Hough.h"
using namespace std;
using namespace cv;
double pi = 3.1415926;
int point_cnt[180][5000];
Mat pic;
struct node
{
    int x, y;
};
vector<node> res;
void hough(Mat &out, Mat &pic, int sx, int ex, int r, int g, int b, int e, vector<point> &ans)
{
    // line(pic, Point2d(400, 25), Point2d(pic.cols - 400, 75), Scalar(0, 255, 255), 2);

    for (int i = 0; i < 180; i++)
        for (int j = 0; j < 5000; j++)
            point_cnt[i][j] = 0;
    int cnt = 0;
    int max_cnt = 0;
    for (int i = sx; i < ex; i++)
    {
        for (int j = 0; j < out.cols; j++)
        {
            if (out.at<uchar>(i, j) == 255)
            {
                cnt++;
                for (int k = 0; k < 180; k++)
                {
                    int temp = int(cos(pi * k / 180) * i + sin(pi * k / 180) * j + 0.5) + 2500;
                    point_cnt[k][temp]++;
                    max_cnt = max(max_cnt, point_cnt[k][temp]);
                }
            }
        }
    }
    int res_cnt = 0;
    res.clear();
    for (int i = 20; i < 160; i++)
    {
        for (int j = 0; j < 5000; j++)
        {
            if (e < point_cnt[i][j])
            {
                // cout << "i==" << i << " j==" << j << "\n";
                if (i != 0)
                {
                    bool flag = 0;
                    for (int k = 25; k <= 75; k++)
                    {
                        int y = (1.0 * (j - 2500) - cos(pi * i / 180) * k) / sin(pi * i / 180);
                        if (y >= 450 && y <= pic.cols - 450)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if (!flag)
                        continue;
                    int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
                    for (int k = 0; k < pic.rows; k++)
                    {
                        int y = (1.0 * (j - 2500) - cos(pi * i / 180) * k) / sin(pi * i / 180);
                        if (y < pic.cols)
                        {
                            x1 = k;
                            y1 = y;
                            break;
                        }
                    }
                    for (int k = pic.rows - 1; k >= 0; k--)
                    {
                        int y = (1.0 * (j - 2500) - cos(pi * i / 180) * k) / sin(pi * i / 180);
                        if (y < pic.cols)
                        {
                            x2 = k;
                            y2 = y;
                            break;
                        }
                    }
                    if (x1 == -1 || x2 == -1 || x1 == x2)
                        continue;
                    // line(pic, Point2d(y1, x1), Point2d(y2, x2), Scalar(0, 255, 255), 1);
                    res.push_back({i, j - 2500});
                }
            }
        }
    }

    for (int i = 0; i < (int)res.size(); i++)
    {
        int p = i + 1;
        while (p < (int)res.size() && (abs(res[p].x - res[i].x) <= 5) && (abs(res[p].y - res[i].y) <= 50))
            p++;
        double sita = 0, rou = 0;
        for (int k = i; k < p; k++)
        {
            sita += res[k].x;
            rou += res[k].y;
        }
        sita /= p - i;
        rou /= p - i;

        int y1 = (rou - cos(pi * sita / 180) * sx) / sin(pi * sita / 180);
        int y2 = (rou - cos(pi * sita / 180) * ex) / sin(pi * sita / 180);
        line(pic, Point2d(y1, sx), Point2d(y2, ex), Scalar(r, g, b), 1);
        ans.push_back(point{rou, sita});
        res_cnt++;
        i = p - 1;
    }
    cout << res_cnt << " " << cnt << " " << max_cnt << endl;
}