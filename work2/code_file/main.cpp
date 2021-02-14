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
        line_p.clear();
        hough(gray, pic, 0, pic.rows, 255, 255, 0, 60, line_p);
        string file_name = "../out_file/";
        file_name += to_string(i);
        file_name += ".txt";
        const char *c = file_name.c_str();
        ofstream outfile;
        outfile.open(c, ios::out | ios::trunc);
        outfile << file_names[i] << "\n";
        for (int j = 0; j < (int)line_p.size(); j++)
        {
            for (int k = 0; k < 56; k++)
            {
                if (x[k] < 250)
                {
                    outfile << -2 << "\n";
                    continue;
                }
                int y = (1.0 * line_p[j].x - cos(pi * line_p[j].y / 180) * (x[k] - 250)) / sin(pi * line_p[j].y / 180);
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