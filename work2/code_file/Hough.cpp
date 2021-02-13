#include <opencv.hpp>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
using namespace std;
using namespace cv;
double pi = 3.1415926;
int point_cnt[180][5000];
Mat pic;
struct point
{
    int x, y;
};
vector<point> res;
void hough(Mat &out)
{
    for (int i = 0; i < 180; i++)
        for (int j = 0; j < 5000; j++)
            point_cnt[i][j] = 0;
    int cnt = 0;
    int max_cnt = 0;
    for (int i = 0; i < out.rows; i++)
    {
        for (int j = 0; j < out.cols; j++)
        {
            if (out.at<uchar>(i, j) == 255)
            {
                cnt++;
                for (int k = 0; k < 180; k++)
                {
                    int temp = int(cos(pi * k / 180) * i + sin(pi * k / 180) * j) + 2500;
                    point_cnt[k][temp]++;
                    max_cnt = max(max_cnt, point_cnt[k][temp]);
                }
            }
        }
    }
    int res_cnt = 0;
    // Mat img(180, 500, CV_8UC1);
    // for (int i = 0; i < 180; i++)
    // {
    //     for (int j = 0; j < 500; j++)
    //     {
    //         img.at<uchar>(i, j) = min(point_cnt[i][j + 2250], 255);
    //     }
    // }
    // imshow("res", img);
    // waitKey(0);
    vector<point> res;
    res.clear();
    for (int i = 0; i < 180; i++)
    {
        for (int j = 0; j < 5000; j++)
        {
            if ((max_cnt / 2) < point_cnt[i][j])
            {
                cout << "i==" << i << " j==" << j << "\n";
                if (i == 0)
                {
                    int x = j - 2500;
                    cout << "x==" << x << "\n";
                    line(pic, Point2d(0, x), Point2d(1000, x), Scalar(0, 255, 255), 2);
                }
                else
                {
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
        while (p < (int)res.size() && (abs(res[p].x - res[p - 1].x) <= 5) && (abs(res[p].y - res[p - 1].y) <= 50))
            p++;
        double sita = 0, rou = 0;
        for (int k = i; k < p; k++)
        {
            sita += res[k].x;
            rou += res[k].y;
        }
        sita /= p - i;
        rou /= p - i;
        if (sita == 0)
            line(pic, Point2d(0, rou), Point2d(1000, rou), Scalar(0, 255, 255), 2);
        else
        {
            int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
            for (int k = 0; k < pic.rows; k++)
            {
                int y = (1.0 * rou - cos(pi * sita / 180) * k) / sin(pi * sita / 180);
                if (y < pic.cols)
                {
                    x1 = k;
                    y1 = y;
                    break;
                }
            }
            for (int k = pic.rows - 1; k >= 0; k--)
            {
                int y = (1.0 * rou - cos(pi * sita / 180) * k) / sin(pi * sita / 180);
                if (y < pic.cols)
                {
                    x2 = k;
                    y2 = y;
                    break;
                }
            }
            if (x1 == -1 || x2 == -1 || x1 == x2)
                continue;
            line(pic, Point2d(y1, x1), Point2d(y2, x2), Scalar(0, 255, 255), 1);
        }
        res_cnt++;
        i = p - 1;
    }
    cout << res_cnt << " " << cnt << " " << max_cnt << endl;
    imshow("res", pic);
    imshow("out", out);
    waitKey(0);
}

int main()
{
    pic = imread("../data5.jpg");
    Mat gray = imread("../data5.jpg", IMREAD_GRAYSCALE);
    Rect rect(0, (int)(0.3 * pic.rows), pic.cols, (int)(0.7 * pic.rows));
    pic = pic(rect);
    Rect rect1(0, (int)(0.3 * gray.rows), gray.cols, (int)(0.7 * gray.rows));
    gray = gray(rect1);
    Mat temp = gray.clone();

    // temp.at<uchar>(75, 200) = 255;
    // temp.at<uchar>(75, pic.cols - 200) = 255;
    // imshow("asdas", temp);
    // waitKey(0);
    // return 0;

    // Point2f src_points[4];
    // Point2f dst_points[4];
    // src_points[0] = Point2f(pic.cols / 4, 35);
    // src_points[1] = Point2f(pic.cols / 2, 35);
    // src_points[2] = Point2f(0, 230);
    // src_points[3] = Point2f(pic.cols / 2, pic.rows);

    // dst_points[0] = Point2f(0.0, 0.0);
    // dst_points[1] = Point2f(pic.cols - 1, 0.0);
    // dst_points[2] = Point2f(0.0, pic.rows - 1);
    // dst_points[3] = Point2f(pic.cols - 1, pic.rows - 1);
    // Mat rotation;
    // rotation = getPerspectiveTransform(src_points, dst_points);
    // warpPerspective(gray, temp, rotation, pic.size());

    // Mat sharpen_op = (Mat_<char>(3, 3) << -1, -1, -1,
    //                   -1, 9, -1,
    //                   -1, -1, -1);

    // filter2D(temp, temp, CV_8U, sharpen_op);
    // imshow("temp", temp);

    Canny(temp, temp, 50, 200, 3, true);
    Mat tt = temp.clone();
    for (int i = 0; i < temp.rows; i++)
    {
        for (int j = 0; j < temp.cols; j++)
        {
            int cnt = 0;
            int p = j;
            while (p < temp.cols && temp.at<uchar>(i, p) == 255)
                cnt++, p++;
            if (cnt > 10)
            {
                for (int k = j; k < p; k++)
                    tt.at<uchar>(i, k) = 0;
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
            if (cnt > 10)
            {
                for (int k = i; k < p; k++)
                    tt.at<uchar>(k, j) = 0;
            }
        }
    }

    hough(tt);
    // imshow("pic", pic);
    // vector<Vec4i> lines;
    // HoughLinesP(tt, lines, 1, CV_PI / 180, 160, 20, 20);
    // for (size_t i = 0; i < lines.size(); i++)
    // {
    //     Vec4i I = lines[i];
    //     double x1 = I[0];
    //     double y1 = I[1];
    //     double x2 = I[2];
    //     double y2 = I[3];
    //     if (abs(x1 - x2) + abs(y1 - y2) > 50)
    //     {
    //         line(pic, Point2d(x1, y1), Point2d(x2, y2), Scalar(0, 255, 255), 1);
    //     }
    // }
    // imshow("pic", pic);
    // waitKey(0);
}