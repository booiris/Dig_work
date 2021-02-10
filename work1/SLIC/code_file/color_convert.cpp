#include <opencv.hpp>
#include <cmath>
#include "../head_file/SLIC.h"
using namespace cv;
inline double gamma(double x)
{
    return x > 0.04045 ? pow((x + 0.055) / 1.055, 2.4) : (x / 12.92);
}
inline double F(double x)
{
    return x > 0.008856 ? pow(x, 0.333333) : 7.787 * x + 0.137931;
}
inline void RGB2Lab(uchar R, uchar G, uchar B, double &L, double &a, double &b)
{
    double RR = gamma(1.0 * R / 225);
    double GG = gamma(1.0 * G / 225);
    double BB = gamma(1.0 * B / 225);

    double x = 0.412453 * RR + 0.357580 * GG + 0.180423 * BB;
    double y = 0.212671 * RR + 0.715160 * GG + 0.072169 * BB;
    double z = 0.019334 * RR + 0.119193 * GG + 0.950227 * BB;

    x /= 0.950456;
    z /= 1.088754;

    double fx = F(x);
    double fy = F(y);
    double fz = F(z);

    L = y > 0.008856 ? (116.0 * fy - 16.0) : (903.3 * y);
    a = 500 * (fx - fy);
    b = 200 * (fy - fz);
}

void color_convert(Mat &pic, double Lab[][1500][1500])
{
    for (int i = 0; i < pic.rows; i++)
        for (int j = 0; j < pic.cols; j++)
            RGB2Lab(pic.at<Vec3b>(i, j)[2], pic.at<Vec3b>(i, j)[1], pic.at<Vec3b>(i, j)[0], Lab[0][i][j], Lab[1][i][j], Lab[2][i][j]);
}
