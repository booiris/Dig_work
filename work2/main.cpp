#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <windows.h>
#include <vector>
#include "../head_file/Hough.h"
#include "../head_file/canny.h"
using namespace std;
using namespace cv;
vector<point> line_p;
int x[100] = {160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710};
const double pi = 3.1415926;
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
vector<string> file_names;
void read_file_name(const char *dir_name)
{
    WIN32_FIND_DATAA file_data;
    char temp_dir_name[1000];
    sprintf(temp_dir_name, "%s/*", dir_name);
    HANDLE now_file_handle = FindFirstFileA(temp_dir_name, &file_data);
    char file_name[1024];
    do
    {
        if (*file_data.cFileName == '.')
            continue;
        sprintf(file_name, "%s/%s", dir_name, file_data.cFileName);
        WIN32_FILE_ATTRIBUTE_DATA file_info;
        GetFileAttributesExA(file_name, GetFileExInfoStandard, &file_info);
        if (file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            read_file_name(file_name);
        else
        {
            if (strcmp(file_data.cFileName, "20.jpg") == 0)
                file_names.push_back(file_name);
        }
    } while ((FindNextFileA(now_file_handle, &file_data) != 0));
}

vector<point> lp1, lp2, lp3;

inline double get_y(double &rou, double &sita, int x)
{
    return (rou - cos(pi * sita / 180) * x) / sin(pi * sita / 180);
}
struct temp_node
{
    double rou, sita, y;
};
vector<temp_node> kl;
vector<temp_node> bot;
vector<temp_node> top;
bool cmp(const temp_node &x, const temp_node &y)
{
    return x.y < y.y;
}
struct ans_node
{
    vector<temp_node> key;
};
vector<ans_node> f_line;
void key(Mat &img)
{
    f_line.clear();
    kl.clear();
    top.clear();
    bot.clear();
    for (int i = 0; i < lp1.size(); i++)
    {
        double y = get_y(lp1[i].x, lp1[i].y, 50);
        top.push_back(temp_node{lp1[i].x, lp1[i].y, y});
    }

    for (int i = 0; i < lp2.size(); i++)
    {
        double y = get_y(lp2[i].x, lp2[i].y, 250);
        kl.push_back(temp_node{lp2[i].x, lp2[i].y, y});
    }

    for (int i = 0; i < lp3.size(); i++)
    {
        double y = get_y(lp3[i].x, lp3[i].y, 250);
        bot.push_back(temp_node{lp3[i].x, lp3[i].y, y});
    }

    sort(kl.begin(), kl.end(), cmp);

    for (int i = 0; i < kl.size(); i++)
    {
        int p = i + 1;
        while (p < kl.size() && (kl[p].y - kl[p - 1].y < 50 || abs(get_y(kl[p - 1].rou, kl[p - 1].sita, 50) - get_y(kl[p].rou, kl[p].sita, 50)) < 50))
            p++;
        double rou = kl[i + (p - i - 1) / 2].rou, sita = kl[i + (p - i - 1) / 2].sita;
        double y1 = (rou - cos(pi * sita / 180) * 50) / sin(pi * sita / 180);
        double y2 = (rou - cos(pi * sita / 180) * 250) / sin(pi * sita / 180);
        double t1, t2;
        double nowx = 0;

        double top_rou = -1, top_sita = -1;
        double mind = 1e9;
        line(img, Point2d(y1, 50), Point2d(y2, 250), Scalar(255, 0, 0), 1);
        for (int j = 0; j < top.size(); j++)
        {
            if (abs(y1 - top[j].y) > 30 || abs(sita - top[j].sita) > 5)
                continue;
            double temp_num = sqrt((y1 - top[j].y) * (y1 - top[j].y) + (sita - top[j].sita) * (sita - top[j].sita));
            if (mind > temp_num)
            {
                mind = temp_num;
                top_rou = top[j].rou;
                top_sita = top[j].sita;
                t1 = (top_rou - cos(pi * top_sita / 180) * 0) / sin(pi * top_sita / 180);
                t2 = (top_rou - cos(pi * top_sita / 180) * 50) / sin(pi * top_sita / 180);
            }
        }
        ans_node temp;
        temp.key.clear();
        if (top_sita >= 0)
        {
            temp.key.push_back(temp_node{top_rou, top_sita, nowx});
            nowx = 50;
            line(img, Point2d(t1, 0), Point2d(t2, 50), Scalar(255, 0, 0), 1);
        }

        temp.key.push_back(temp_node{rou, sita, nowx});
        nowx = 250;

        double bot_rou = -1, bot_sita = -1;
        mind = 1e9;
        for (int j = 0; j < bot.size(); j++)
        {
            if (abs(y2 - bot[j].y) > 30 || abs(sita - bot[j].sita) > 5)
                continue;
            double temp_num = sqrt((y2 - bot[j].y) * (y2 - bot[j].y) + (sita - bot[j].sita) * (sita - bot[j].sita));
            if (mind > temp_num)
            {
                mind = temp_num;
                bot_rou = bot[j].rou;
                bot_sita = bot[j].sita;
                t1 = (bot_rou - cos(pi * bot_sita / 180) * 250) / sin(pi * bot_sita / 180);
                t2 = (bot_rou - cos(pi * bot_sita / 180) * img.rows) / sin(pi * bot_sita / 180);
            }
        }
        if (bot_sita >= 0)
        {
            temp.key.push_back(temp_node{bot_rou, bot_sita, nowx});
            line(img, Point2d(t1, 250), Point2d(t2, img.rows), Scalar(255, 0, 0), 1);
        }

        f_line.push_back(temp);

        i = p - 1;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    read_file_name("e:/Dig_work/work2/clips");

    for (int i = 0; i < file_names.size(); i++)
    {
        Mat pic = imread(file_names[i]);
        pic = cut(pic);
        Mat gray = canny(pic);
        lp1.clear();
        lp2.clear();
        lp3.clear();
        f_line.clear();
        hough(gray, pic, 0, 50, 0, 255, 0, 10, lp1);
        hough(gray, pic, 50, 250, 255, 255, 0, 10, lp2);
        hough(gray, pic, 250, pic.rows, 0, 255, 255, 15, lp3);
        key(pic);
        string file_name = "../out_file/";
        file_name += to_string(i);
        file_name += ".txt";
        const char *c = file_name.c_str();
        ofstream outfile;
        outfile.open(c, ios::out | ios::trunc);
        outfile << file_names[i] << "\n";
        for (int j = 0; j < (int)f_line.size(); j++)
        {
            for (int k = 0; k < 56; k++)
            {
                int y;
                if (x[k] < 250)
                {
                    y = -2;
                    outfile << y << "\n";
                    continue;
                }

                for (int l = f_line[j].key.size() - 1; l >= 0; l--)
                {
                    if (f_line[j].key[l].y < (x[k] - 250))
                    {
                        double rou = f_line[j].key[l].rou;
                        double sita = f_line[j].key[l].sita;
                        y = (rou - cos(pi * sita / 180) * (x[k] - 250)) / sin(pi * sita / 180);
                        break;
                    }
                }
                if (y < 0 || y >= pic.cols)
                    y = -2;
                outfile << y << "\n";
            }
            outfile << "#\n";
        }
        outfile.close();
        // imshow("gray", gray);
        // imshow("pic", pic);
    }
    waitKey(0);
}